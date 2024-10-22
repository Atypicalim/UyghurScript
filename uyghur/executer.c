// executer

#include "others/header.h"

jmp_buf jump_buffer;

typedef struct {
    int value;
    bool freeable;
} UG_INDEX;

UG_INDEX ug_index;

typedef struct {
    char *value;
    bool freeable;
} UG_LOCATION;

UG_LOCATION ug_location;

Value *Executer_getValueByToken(Executer *, Token *, bool);
void Executer_consumeLeaf(Executer *, Leaf *);
void Executer_consumeTree(Executer *, Leaf *);

void Executer_reset(Executer *this)
{
    this->uyghur = NULL;
    this->machine = NULL;
    this->debug = NULL;
    this->bridge = NULL;
    this->tree = NULL;
    this->leaf = NULL;
    this->globalScope = NULL;
    this->callStack = NULL;
    this->isReturn = false;
    this->isCatch = false;
    this->errorMsg = NULL;
}

Executer *Executer_new(Uyghur *uyghur)
{
    Executer *executer = malloc(sizeof(Executer));
    Executer_reset(executer);
    executer->uyghur = uyghur;
    executer->machine = uyghur->machine;
    executer->debug = uyghur->debug;
    executer->bridge = uyghur->bridge;
    //
    Machine *machine = executer->machine;
    machine->holders = Stack_new();
    machine->calls = Stack_new();
    machine->globals = Holdable_newScope("global", NULL);
    //
    Machine_initKinds(machine);
    executer->callStack = machine->calls;
    executer->globalScope = machine->globals;
    // 
    ug_location.value = NULL;
    ug_location.freeable = false;
    //
    return executer;
}

void Executer_error(Executer *this, Token *token, char *msg)
{
    char *m = msg != NULL ? msg : LANG_ERR_EXECUTER_EXCEPTION;
    char *s = token == NULL ? UG_TAG_UNKNOWN : format_token_place(token);
    char *err = tools_format("%s, %s", m, s);
    if (this->isCatch) {
        this->errorMsg = err;
        longjmp(jump_buffer, 1);
    } else {
        log_error("Executer: %s, %s", m, s);
        Debug_writeTrace(this->uyghur->debug);
        exit(1);
    }
}

void Executer_assert(Executer *this, bool value, Token *token, char *msg)
{
    if (value == true) return;
    Executer_error(this, token, msg);
}

void Runtime_error(char *msg) {
    Executer_error(__uyghur->executer, NULL, msg);
}
void Runtime_assert(bool value, char *msg) {
    Executer_assert(__uyghur->executer, value, NULL, msg);
}

void Executer_pushScope(Executer *this, CString name)
{
    Holdable *current = this->machine->currHoldable;
    Holdable *holdable = Holdable_newScope(name, current);
    Machine_pushHolder(this->machine, holdable);
}

void Executer_popScope(Executer *this)
{
    Holdable *holdable = Machine_popHolder(this->machine);
    tools_assert(holdable != NULL && holdable->type == UG_TYPE_SCP, LANG_ERR_EXECUTER_INVALID_STATE);
    Machine_releaseObj(holdable);
}

Holdable *Executer_getCurrentScope(Executer *this, Token *token)
{
    Holdable *holdable = this->machine->currHoldable;
    Executer_assert(this, holdable != NULL, token, LANG_ERR_EXECUTER_CONTAINER_NOT_FOUND);
    return holdable;
}

void Executer_findValueByLocation(Executer *this, char *key, Value **rContainer, Value **rValue)
{
    *rContainer = NULL;
    *rValue = NULL;
    //
    Holdable *holder = this->machine->currHoldable;
    if (holder) {
        while (holder != NULL) {
            Value *value = Dictable_getLocation(holder, key);
            if (value != NULL) {
                *rContainer = holder;
                *rValue = value;
                return;
            }
            holder = holder->linka;
        }
    }
    //
    Holdable *global = this->globalScope;
    if (global) {
        Value *value = Dictable_getLocation(global, key);
        if (value != NULL) {
            *rContainer = global;
            *rValue = value;
            return;
        }
        return;
    }
}

void Executer_findValueByToken(Executer *this, Token *token, Value **rContainer, Value **rValue)
{
    *rContainer = NULL;
    *rValue = NULL;
    // empty
    if (token == NULL || Token_isEmpty(token)) {
        return;
    }
    // name
    if (Token_isName(token)) {
        Executer_findValueByLocation(this, token->value, rContainer, rValue);
        return;
    }
    // word
    if (Token_isWord(token)) {
        if (is_eq_string(token->value, TVALUE_THIS)) {
            *rValue = Machine_getCurrentSelf(this->machine);
            return;
        } else if (is_eq_string(token->value, TVALUE_MODULE)) {
            *rValue = Machine_getCurrentModule(this->machine);
            return;
        } else if (is_eq_string(token->value, TVALUE_GLOBAL)) {
            *rValue = this->globalScope;
            return;
        }
    }
    // keys
    Executer_assert(this, Token_isKey(token), token, LANG_ERR_EXECUTER_INVALID_KEY);
    Token *extra = (Token *)token->extra;
    if (is_eq_string(extra->value, SCOPE_ALIAS_GLB) || is_eq_string(extra->value, TVALUE_GLOBAL)) {
        *rContainer = this->globalScope;
        Executer_assert(this, *rContainer != NULL, token, LANG_ERR_EXECUTER_CONTAINER_NOT_FOUND);
        Executer_assert(this, Holdable_isScope(*rContainer), token, LANG_ERR_EXECUTER_CONTAINER_NOT_VALID);
    } else if (is_eq_string(extra->value, SCOPE_ALIAS_MDL) || is_eq_string(extra->value, TVALUE_MODULE)) {
        *rContainer = Machine_getCurrentModule(this->machine);
        Executer_assert(this, *rContainer != NULL, token, LANG_ERR_EXECUTER_CONTAINER_NOT_FOUND);
        Executer_assert(this, Holdable_isModule(*rContainer), token, LANG_ERR_EXECUTER_CONTAINER_NOT_VALID);
    } else if (is_eq_string(extra->value, SCOPE_ALIAS_SLF) || is_eq_string(extra->value, TVALUE_THIS)) {
        *rContainer = Machine_getCurrentSelf(this->machine);
        Executer_assert(this, *rContainer != NULL, token, LANG_ERR_EXECUTER_CONTAINER_NOT_FOUND);
        if (!Objective_isCtr(*rContainer) && !Objective_isObj(*rContainer)) {
            Executer_error(this, token, LANG_ERR_EXECUTER_CONTAINER_NOT_VALID);
        }
    } else {
        Value *value = NULL;
        Executer_findValueByLocation(this, extra->value, &INVALID_CTN, &value);
        if (value != NULL) *rContainer = value;
    }
    // container
    Executer_assert(this, *rContainer != NULL, extra, LANG_ERR_EXECUTER_INVALID_BOX);
    // if (*rContainer == NULL) return;
    // parse keys
    bool further = false;
    int index = -1;
    char *key = NULL;
    if (Token_isKeyOfNumber(token)) {
        index = atof(token->value);
    } else if (Token_isKeyOfString(token)) {
        key = token->value;
    } else if (Token_isKeyOfName(token)) {
        Token *_extra = (Token *)token->extra;
        Token *_token = Token_getTemporary();
        _token->type = _extra->type;
        _token->value = token->value;
        Value *value;
        Executer_findValueByToken(this, _token, &INVALID_CTN, &value);
        if (Value_isNumber(value)) {
            index = value->number;
        } else if (Value_isString(value)) {
            key = value->string;
        } else {
            Executer_error(this, token, LANG_ERR_EXECUTER_INVALID_KEY);
        }
    }
    // num
    if (Value_isNumber(*rContainer) || *rContainer == (Value *)this->machine->kindNum) {
        Executer_assert(this, key != NULL, token, LANG_ERR_EXECUTER_INVALID_KEY);
        *rValue = Dictable_getLocation(this->machine->kindNum, key);
        return;
    }
    // str
    if (Value_isString(*rContainer) || *rContainer == (Value *)this->machine->kindStr) {
        Executer_assert(this, key != NULL, token, LANG_ERR_EXECUTER_INVALID_KEY);
        *rValue = Dictable_getLocation(this->machine->kindStr, key);
        return;
    }
    // list
    if (Value_isListable(*rContainer)) {
        if (index >= 0) {
            *rValue = Listable_getIndex(*rContainer, index);
            return;
        } else {
            further = true;
        }
    }
    if (further ||  *rContainer == (Value *)this->machine->kindList) {
        Executer_assert(this, key != NULL, token, LANG_ERR_EXECUTER_INVALID_KEY);
        *rValue = Dictable_getLocation(this->machine->kindList, key);
        return;
    }
    // dict
    if (Value_isDictable(*rContainer)) {
        Executer_assert(this, key != NULL, token, LANG_ERR_EXECUTER_INVALID_KEY);
        *rValue = Dictable_getLocation(*rContainer, key);
        further = *rValue == NULL;
    }
    if (further || *rContainer == (Value *)this->machine->kindDict) {
        Executer_assert(this, key != NULL, token, LANG_ERR_EXECUTER_INVALID_KEY);
        *rValue = Dictable_getLocation(this->machine->kindDict, key);
        return;
    }
    // holdable
    if (Value_isHoldable(*rContainer)) {
        Executer_assert(this, key != NULL, token, LANG_ERR_EXECUTER_INVALID_KEY);
        *rValue = Dictable_getLocation(*rContainer, key);
        return;
    }
    // objective
    if (Value_isObjective(*rContainer)) {
        Executer_assert(this, key != NULL, token, LANG_ERR_EXECUTER_INVALID_KEY);
        *rValue = Dictable_getLocation(*rContainer, key);
        if (*rValue == NULL && Objective_isObj(*rContainer)) {
            Queue *parents = (*rContainer)->extra;
            Queue_RESTE(parents);
            Objective *parent = Queue_NEXT(parents);
            while (*rValue == NULL && parent != NULL) {
                *rValue = Dictable_getLocation(parent, key);
                parent = Queue_NEXT(parents);
            }
        }
    }
}

Value *Executer_searchValueOfNameKey(Executer *this, Token *token, bool checkValue, bool checkHolder) {
    tools_assert(token != NULL, "invalid token for search value");
    Token *_extra = (Token *)token->extra;
    Token *_token = Token_getTemporary();
    _token->type = _extra->type;
    _token->value = token->value;
    //
    Holdable *holdable = NULL;
    Value *value = NULL;
    Executer_findValueByLocation(this, token->value, &holdable, &value);
    // Executer_findValueByToken(this, _token, &holdable, &value);
    //
    if (checkValue) Executer_assert(this, value!= NULL, token, LANG_ERR_EXECUTER_INVALID_KEY);
    if (checkHolder) Executer_assert(this, holdable!= NULL, token, LANG_ERR_EXECUTER_INVALID_KEY);
    return value;
}

void Executer_delIndexOfToken(Executer *this, int index) {
}

int Executer_genIndexOfToken(Executer *this, Token *token) {
    if (ug_index.freeable) {
        ug_index.value = -1;
        ug_index.freeable = false;
    }
    ug_index.value = -1;
    tools_assert(token != NULL, "invalid token for get index");
    if (Token_isKeyOfNumber(token) || Token_isNumber(token) || Token_isName(token)) {
        ug_index.value = atof(token->value);
        ug_index.freeable = false;
    } else if (Token_isKeyOfName(token)) {
        Value *value = Executer_searchValueOfNameKey(this, token, true, true);
        if (Value_isNumber(value)) {
            ug_index.value = value->number;
            ug_index.freeable = true;
        }
    }
    return ug_index.value;
}

void Executer_delLocationOfToken(Executer *this, char *key) {
}

char *Executer_genLocationOfToken(Executer *this, Token *token) {
    if (ug_location.freeable) {
        pct_free(ug_location.value);
        ug_location.freeable = false;
    }
    ug_location.value = NULL;
    tools_assert(token != NULL, "invalid token for get location");
    if (Token_isKeyOfString(token) || Token_isString(token) || Token_isName(token)) {
        ug_location.value = token->value;
        ug_location.freeable = false;
    } else if (Token_isKeyOfName(token)) {
        Value *value = Executer_searchValueOfNameKey(this, token, true, true);
        if (Value_isString(value)) {
            ug_location.value = Value_toString(value);
            ug_location.freeable = true;
        }
    }
    return ug_location.value;
}

// 

// need release
Value *Executer_getValueFromContainer(Executer *this, Holdable *holdable, Token *token)
{
    Executer_assert(this, holdable != NULL, token, LANG_ERR_EXECUTER_INVALID_VARIABLE);
    char *location = Executer_genLocationOfToken(this, token);
    Executer_assert(this, location != NULL, token, "getting invalid location from dict");
    Value *value = Dictable_getLocation(holdable, location);
    if (value != NULL) Machine_retainObj(value);
    return value;
}

void Executer_setValueToContainer(Executer *this, Value *container, Token *token, Value *value)
{
    Executer_assert(this, container != NULL, token, LANG_ERR_EXECUTER_INVALID_VARIABLE);
    if (Value_isListable(container)) {
        // list set
        int index = Executer_genIndexOfToken(this, token);
        Executer_assert(this, index >= 0, token, "setting invalid index to list");
        Listable_setIndex(container, index, value);
        return;
    } else {
        // dict set
        char *location = Executer_genLocationOfToken(this, token);
        Executer_assert(this, location != NULL, token, "setting invalid location to dict");
        Dictable_setLocation(container, location, value);
    }
}

// 

// need release
Value *Executer_getValueByToken(Executer *this, Token *token, bool withEmpty)
{
    Value *value = convert_token_to_value(token);
    if (value != NULL) return value;
    //
    value = Machine_readKind(this->machine, token->value);
    if (value != NULL) return value;
    //
    Executer_findValueByToken(this, token, &INVALID_CTN, &value);
    if (value != NULL) {
        Machine_retainObj(value);
    } else if (withEmpty) {
        value = Value_newEmpty(token);
    }
    return value;
}

void *Executer_setValueByToken(Executer *this, Token *token, Value *value, bool withDeclare)
{
    Value *container = NULL;
    Executer_findValueByToken(this, token, &container, &INVALID_VAL);
    if (withDeclare && container == NULL) container = this->machine->currHoldable;
    Executer_assert(this, container != NULL, token, LANG_ERR_EXECUTER_INVALID_BOX);
    Executer_setValueToContainer(this, container, token, value);
    Machine_releaseObj(value);
}

// TODO: remove to utils or value

double Executer_calculateNumbers(Executer *this, double left, char *sign, double right, Token *token)
{
    if (is_eq_string(sign, TVALUE_SIGN_ADD)) return left + right;
    if (is_eq_string(sign, TVALUE_SIGN_SUB)) return left - right;
    if (is_eq_string(sign, TVALUE_SIGN_POW)) return pow(left, right);
    if (is_eq_string(sign, TVALUE_SIGN_PER)) return fmod(left, right);
    if (is_eq_string(sign, TVALUE_SIGN_MUL)) return left * right;
    if (is_eq_string(sign, TVALUE_SIGN_DIV)) {
        Executer_assert(this, right != 0, token, LANG_ERR_EXECUTER_INVALID_DEVIDE);
        return left / right;
    }
    int lInt = (int)left;
    int rInt = (int)right;
    bool isInt = lInt == left && rInt == right;
    Executer_assert(this, isInt, token, LANG_ERR_CANNOT_BE_FLOAT);
    if (is_eq_string(sign, TVALUE_SIGN_NOT)) return lInt ^ rInt;
    if (is_eq_string(sign, TVALUE_SIGN_AND)) return lInt & rInt;
    if (is_eq_string(sign, TVALUE_SIGN_ORR)) return lInt | rInt;
    Executer_error(this, token, LANG_ERR_EXECUTER_CALCULATION_INVALID_SIGN);
    return 0;
}

bool Executer_calculateBooleans(Executer *this, bool left, char *sign, bool right, Token *token)
{
    if (is_eq_string(sign, TVALUE_SIGN_NOT)) return left != right;
    if (is_eq_string(sign, TVALUE_SIGN_AND)) return left && right;
    if (is_eq_string(sign, TVALUE_SIGN_ORR)) return left || right;
    Executer_error(this, token, LANG_ERR_EXECUTER_CALCULATION_INVALID_SIGN);
    return NULL;
}

String* Executer_calculateStrings(Executer *this, String *left, char *sign, String *right, Token *token)
{
    if (is_eq_string(sign, TVALUE_SIGN_LNK)) {
        return String_format("%s%s", left->data, right->data);
    }
    Executer_error(this, token, LANG_ERR_EXECUTER_CALCULATION_INVALID_SIGN);
    return NULL;
}

Value *Executer_calculateValues(Executer *this, Value *left, Token *token, Value *right)
{
    tools_assert(left != NULL, "invalid values to calculate");
    tools_assert(right != NULL, "invalid values to calculate");
    Value *result = NULL;
    char lType = left->type;
    char rType = right->type;
    char *sign = token->value;
    int compCode = Value_compareTo(left, right);
    int sameType = compCode != CODE_FAIL;
    if (is_eq_strings(sign, TVAUE_GROUP_CALCULATION_ALL)) {
        if (is_eq_string(sign, TVALUE_SIGN_EQUAL)) {
            bool r = sameType && compCode == CODE_NONE;
            result = Value_newBoolean(r, token);
        } else if (sameType && is_eq_string(sign, TVALUE_SIGN_MORE)) {
            bool r = compCode == CODE_TRUE;
            result = Value_newBoolean(r, token);
        } else if (sameType && is_eq_string(sign, TVALUE_SIGN_LESS)) {
            bool r = compCode == CODE_FALSE;
            result = Value_newBoolean(r, token);
        } else if (is_eq_string(sign, TVALUE_SIGN_PER)) {
            if (is_type_objective(rType)) {
                bool r = Objective_isInstanceOf(left, right);
                result = Value_newBoolean(r, token);
            } else if (is_type_holdable(rType)) {
                bool r = Holdable_isKindOf(right, left);
                result = Value_newBoolean(r, token);
            } else if (rType == UG_TYPE_NIL) {
                bool r = lType == UG_TYPE_NIL;
                result = Value_newBoolean(r, token);
            } else if (lType == UG_TYPE_NUM && rType == UG_TYPE_NUM) {
                double r = Executer_calculateNumbers(this, left->number, sign, right->number, token);
                result = Value_newNumber(r, token);
            }
        }
    } else if (sameType) {
        if (is_eq_strings(sign, TVAUE_GROUP_CALCULATION_NUM) && lType == UG_TYPE_NUM) {
            double r = Executer_calculateNumbers(this, left->number, sign, right->number, token);
            result = Value_newNumber(r, token);
        } else if (is_eq_strings(sign, TVAUE_GROUP_CALCULATION_BOL) && lType == UG_TYPE_NUM) {
            double r = Executer_calculateNumbers(this, left->number, sign, right->number, token);
            result = Value_newNumber(r, token);
        } else if (is_eq_strings(sign, TVAUE_GROUP_CALCULATION_BOL) && lType == UG_TYPE_BOL) {
            bool r = Executer_calculateBooleans(this, left->boolean, sign, right->boolean, token);
            result = Value_newBoolean(r, token);
        } else if (is_eq_strings(sign, TVAUE_GROUP_CALCULATION_STR) && lType == UG_TYPE_STR) {
            String *r = Executer_calculateStrings(this, left->string, sign, right->string, token);
            result = Value_newString(r, token);
        }
    } else {
        bool bLeftStr = lType == UG_TYPE_STR;
        bool bRightStr = rType == UG_TYPE_STR;
        bool bLeftNum = lType == UG_TYPE_NUM;
        bool bRightNum = rType == UG_TYPE_NUM;
        bool hasStr = bLeftStr || bRightStr;
        bool hasNum = bLeftNum || bRightNum;
        if (hasStr && hasNum && is_eq_string(sign, TVALUE_SIGN_RPT)) {
            if (bLeftStr) {
                String *r = String_clone(left->string);
                String_repeat(r, right->number);
                result = Value_newString(r, token);
            }
            if (bRightStr) {
                String *r = String_clone(right->string);
                String_repeat(r, left->number);
                result = Value_newString(r, token);
            }
        }
    }
    Executer_assert(this, result != NULL, token, LANG_ERR_EXECUTER_CALCULATION_INVALID_SIGN);
    return result;
}

// ending

void Executer_consumeVariable(Executer *this, Leaf *leaf)
{
    Stack_RESTE(leaf->tokens);
    Token *token = Stack_NEXT(leaf->tokens);
    Token *name = Stack_NEXT(leaf->tokens);
    // 
    Value *new = NULL;
    if (Token_isEmpty(token)) {
        new = Value_newEmpty(token);
        new->fixed = false;
    } else if (Token_isWord(token) && is_eq_string(token->value, TVALUE_BOL)) {
        new = Value_newBoolean(false, token);
        new->fixed = true;
    } else if (Token_isWord(token) && is_eq_string(token->value, TVALUE_NUM)) {
        new = Value_newNumber(0, token);
        new->fixed = true;
    } else if (Token_isWord(token) && is_eq_string(token->value, TVALUE_STR)) {
        new = Value_newString(String_new(), token);
        new->fixed = true;
    } else if (Token_isWord(token) && is_eq_string(token->value, TVALUE_LST)) {
        new = Listable_newLst(name);
        new->fixed = true;
    } else if (Token_isWord(token) && is_eq_string(token->value, TVALUE_DCT)) {
        new = Dictable_newDct(name);
        new->fixed = true;
    } else {
        new = Executer_getValueByToken(this, token, true); // todo
    }
    // 
    Value *old = Executer_getValueFromContainer(this, this->machine->currHoldable, name);
    Executer_assert(this, old == NULL, name, LANG_ERR_EXECUTER_VARIABLE_ALREADY_DEFINED);
    Executer_assert(this, new != NULL, name, LANG_ERR_EXECUTER_VARIABLE_FAILURE_DEFINED);
    Executer_setValueToContainer(this, this->machine->currHoldable, name, new);
    Machine_releaseObj(new);
}

void Executer_consumeCommand(Executer *this, Leaf *leaf)
{
    Stack_RESTE(leaf->tokens);
    Token *action = Stack_NEXT(leaf->tokens);
    Token *name = Stack_NEXT(leaf->tokens);
    if (is_eq_string(action->value, TVALUE_CMD_OUTPUT))
    {
        Value *value = NULL;
        if (!Token_isString(name) && helper_token_is_values(name, TVAUES_GROUP_UTYPES)) {
            value = Executer_getValueFromContainer(this, this->globalScope, name);
        } else {
            value = Executer_getValueByToken(this, name, true);
            Executer_assert(this, value != NULL, name, LANG_ERR_EXECUTER_VARIABLE_NOT_FOUND);
        }
        if (Value_isListable(value)) {
            Listable_print(value);
        } else if (Value_isDictable(value)) {
            Dictable_print(value);
        } else if (Value_isHoldable(value)) {
            Holdable_print(value);
        } else if (Value_isObjective(value)) {
            Objective_print(value);
        } else if (Value_isRunnable(value)) {
            Runnable_print(value);
        } else {
            char *content = Value_toString(value);
            printf("%s", content);
            pct_free(content);
        }
        if (value != NULL) Machine_releaseObj(value);
    }
    else if (is_eq_string(action->value, TVALUE_CMD_INPUT))
    {
        char line[1024];
        scanf(" %[^\n]", line);
        Executer_setValueByToken(this, name, Value_newString(String_format("%s", line), NULL), false);
    }
}

void Executer_consumeConvert(Executer *this, Leaf *leaf)
{
    Stack_RESTE(leaf->tokens);
    Token *action = Stack_NEXT(leaf->tokens);
    Token *target = Stack_NEXT(leaf->tokens);
    Value *value = Executer_getValueByToken(this, target, true);
    Value *r = NULL;
    char *act = action->value;
    //
    if (is_eq_string(action->type, UG_TTYPE_WRD))
    {
        if (is_eq_string(act, TVALUE_NIL))
        {
            // TODO free object
            r = Value_newEmpty(NULL);
        }
        else if (is_eq_string(act, TVALUE_OPPOSITE))
        {
            if (value->type == UG_TYPE_NUM)
            {
                r = Value_newBoolean(value->number <= 0, NULL);
            }
            else if (value->type == UG_TYPE_STR)
            {
                r = Value_newBoolean(!is_eq_string(String_get(value->string), TVALUE_TRUE), NULL);
            }
            else if (value->type == UG_TYPE_NIL)
            {
                r = Value_newBoolean(true, NULL);
            }
            else if (value->type == UG_TYPE_BOL)
            {
                r = Value_newBoolean(!value->boolean, NULL);
            }
            else
            {
                r = Value_newBoolean(false, NULL);
            }
        }
        else if (is_eq_string(act, TVALUE_NUM))
        {
            r = Value_toNumber(value);
        }
        else if (is_eq_string(act, TVALUE_STR))
        {
            char *content = Value_toString(value);
            r = Value_newString(String_format("%s", content), NULL);
            pct_free(content);
        }
        else if (is_eq_string(act, TVALUE_BOL))
        {
            r = Value_toBoolean(value);
        }
        else if (is_eq_string(act, TVALUE_WORKER) || is_eq_string(act, TVALUE_CREATOR) || is_eq_string(act, TVALUE_ASSISTER))
        {
            char *funcName = String_get(value->string);
            Token *funcToken = Token_name(funcName);
            // TODO: memory leak
            Value *funcValue = Executer_getValueByToken(this, funcToken, true);
            r = Value_isRunnable(funcValue) ? funcValue : Value_newEmpty(NULL);
        }
    }
    else
    {
        r = Executer_getValueByToken(this, action, true);
    }
    tools_assert(r != NULL, "%s:[%s]", LANG_ERR_EXECUTER_CALCULATION_INVALID_ARGS, act);
    Executer_setValueByToken(this, target, r, false);
    Machine_releaseObj(value);
}

bool Executer_checkJudge(Executer *this, Leaf *leaf)
{
    //
    Stack_RESTE(leaf->tokens);
    Token *judge = Stack_NEXT(leaf->tokens);
    Token *first = Stack_NEXT(leaf->tokens);
    Token *clcltn = Stack_NEXT(leaf->tokens);
    Token *second = Stack_NEXT(leaf->tokens);
    //
    Value *firstV = first == NULL ? NULL : Executer_getValueByToken(this, first, true);
    Value *secondV = second == NULL ? NULL : Executer_getValueByToken(this, second, true);
    Executer_assert(this, firstV != NULL, first, LANG_ERR_EXECUTER_EXCEPTION);
    Value *resultV = NULL;
    //
    if (clcltn == NULL) {
        Executer_assert(this, secondV == NULL, first, LANG_ERR_EXECUTER_EXCEPTION);
        Machine_retainObj(firstV);
        resultV = firstV;
    } else {
        Executer_assert(this, secondV != NULL, first, LANG_ERR_EXECUTER_EXCEPTION);
        resultV = Executer_calculateValues(this, firstV, clcltn, secondV);
    }
    // 
    bool shouldOk = is_eq_string(judge->value, TVALUE_THEN);
    bool isOk = Value_isTrue(resultV);
    // 
    if (firstV != NULL) Machine_releaseObj(firstV);
    if (secondV != NULL) Machine_releaseObj(secondV);
    if (resultV != NULL) Machine_releaseObj(resultV);
    return isOk == shouldOk;
}

void Executer_consumeIf(Executer *this, Leaf *leaf)
{
    //
    bool isFinish = false;
    Queue_RESTE(leaf->leafs);
    // if
    Leaf *ifNode = Queue_NEXT(leaf->leafs);
    tools_assert(ifNode != NULL, LANG_ERR_EXECUTER_INVALID_IF);
    tools_assert(ifNode->type == UG_ATYPE_IF_F, LANG_ERR_EXECUTER_INVALID_IF);
    if (!isFinish && Executer_checkJudge(this, ifNode))
    {
        Executer_pushScope(this, TVALUE_IF);
        Executer_consumeTree(this, ifNode);
        Executer_popScope(this);
        isFinish = true;
    }
    // elseif
    ifNode = Queue_NEXT(leaf->leafs);
    while (ifNode->type == UG_ATYPE_IF_M)
    {
        if (!isFinish && Executer_checkJudge(this, ifNode))
        {
            Executer_pushScope(this, TVALUE_IF_ELSE);
            Executer_consumeTree(this, ifNode);
            Executer_popScope(this);
            isFinish = true;
        }
        ifNode = Queue_NEXT(leaf->leafs);
        tools_assert(ifNode != NULL, LANG_ERR_EXECUTER_INVALID_IF);
    }
    // else
    if (ifNode->type == UG_ATYPE_IF_L)
    {
        Stack_RESTE(ifNode->tokens);
        Token *token = Stack_NEXT(ifNode->tokens);
        tools_assert(is_eq_string(token->value, TVALUE_ELSE), LANG_ERR_EXECUTER_INVALID_IF);
        if (!isFinish)
        {
            Executer_pushScope(this, TVALUE_ELSE);
            Executer_consumeTree(this, ifNode);
            Executer_popScope(this);
            isFinish = true;
        }
        ifNode = Queue_NEXT(leaf->leafs);
        tools_assert(ifNode != NULL, LANG_ERR_EXECUTER_INVALID_IF);
    }
    // end
    tools_assert(ifNode->type == UG_ATYPE_END, LANG_ERR_EXECUTER_INVALID_IF);
    Leaf *nullValue = Queue_NEXT(leaf->leafs);
    tools_assert(nullValue == NULL, LANG_ERR_EXECUTER_INVALID_IF);
}

void Executer_consumeWhile(Executer *this, Leaf *leaf)
{
    while (Executer_checkJudge(this, leaf))
    {
        Executer_pushScope(this, TVALUE_WHILE);
        Executer_consumeTree(this, leaf);
        Executer_popScope(this);
        if (this->errorMsg != NULL) break;
    }
}

void Executer_consumeSpread(Executer *this, Leaf *leaf)
{
    //
    Stack_RESTE(leaf->tokens);
    Token *target = Stack_NEXT(leaf->tokens);
    Token *iter1 = Stack_NEXT(leaf->tokens);
    Token *iter2 = Stack_NEXT(leaf->tokens);
    //
    Value *value = NULL;
    if (!Token_isWord(target)) {
        value = Executer_getValueByToken(this, target, true);
    } else if (is_eq_string(target->value, TVALUE_THIS)) {
        value = Machine_getCurrentSelf(this->machine);
    } else if (is_eq_string(target->value, TVALUE_MODULE)) {
        value = Machine_getCurrentModule(this->machine);
    } else if (is_eq_string(target->value, TVALUE_GLOBAL)) {
        value = this->globalScope;
    } else if (
        is_eq_string(target->value, TVALUE_NUM)
        || is_eq_string(target->value, TVALUE_STR)
        || is_eq_string(target->value, TVALUE_LST)
        || is_eq_string(target->value, TVALUE_DCT)
    ) {
        value = Dictable_getLocation(this->globalScope, target->extra);
        if (value != NULL) Machine_retainObj(value);
    }
    // 
    Value *current1;
    Value *current2;
    if (Value_isInt(value)) {
        int num = (int)value->number;
        for (size_t i = 0; i < num; i++)
        {
            Executer_pushScope(this, TVALUE_SPREAD);
            current1 = Value_newNumber(i, iter1);
            Executer_setValueByToken(this, iter1, current1, true);
            current2 = Value_newNumber(i + 1, iter2);
            Executer_setValueByToken(this, iter2, current2, true);
            Executer_consumeTree(this, leaf);
            Executer_popScope(this);
            if (this->errorMsg != NULL) break;
        }
    } else if (Value_isString(value)) {
        utf8_iter iterator;
        utf8_init(&iterator, String_get(value->string));
        while (utf8_next(&iterator)) {
            Executer_pushScope(this, TVALUE_SPREAD);
            current1 = Value_newNumber(iterator.position, iter1);
            Executer_setValueByToken(this, iter1, current1, true);
            current2 = Value_newString(String_format(utf8_getchar(&iterator)), iter2);
            Executer_setValueByToken(this, iter2, current2, true);
            Executer_consumeTree(this, leaf);
            Executer_popScope(this);
            if (this->errorMsg != NULL) break;
        }
    } else if (Value_isListable(value)) {
        for (int i = 0; i < value->arr->length; i++) {
            Value *val = Array_get(value->arr, i);
            //
            Executer_pushScope(this, TVALUE_SPREAD);
            current1 = Value_newNumber(i, iter1);
            Executer_setValueByToken(this, iter1, current1, true);
            current2 = val;
            Executer_setValueByToken(this, iter2, current2, true);
            Executer_consumeTree(this, leaf);
            Executer_popScope(this);
            if (this->errorMsg != NULL) break;
        }
    } else if (Value_isDictable(value) || Value_isHoldable(value) || Value_isObjective(value)) {
        Value *box = value;
        Hashmap *map = box->map;
        Hashkey *ptr;
        for (int i = 0; i < HASHMAP_DEFAULT_CAPACITY; ++i) {
            ptr = map->bucket[i];
            while (ptr != NULL) {
                String *_key = String_clone(ptr->key);
                Value *val = ptr->value;
                //
                Executer_pushScope(this, TVALUE_SPREAD);
                current1 = Value_newString(_key, iter1);
                Executer_setValueByToken(this, iter1, current1, true);
                current2 = val;
                Executer_setValueByToken(this, iter2, current2, true);
                Executer_consumeTree(this, leaf);
                Executer_popScope(this);
                if (this->errorMsg != NULL) break;
                // 
                ptr = ptr->next;
            }
            if (this->errorMsg != NULL) break;
        }
    } else {
        Executer_error(this, target, LANG_ERR_EXECUTER_INVALID_SPREAD);
    }
    //
    if (value != NULL) Machine_releaseObj(value);
}

void Executer_consumeException(Executer *this, Leaf *leaf)
{
    
    Stack_RESTE(leaf->tokens);
    Token *name = Stack_NEXT(leaf->tokens);
    this->isCatch = true;
    // 
    Executer_pushScope(this, TVALUE_EXCEPTION);
    Executer_consumeTree(this, leaf);
    Executer_popScope(this);
    // 
    this->isCatch = false;
    Value *error = NULL;
    if (this->errorMsg != NULL) {
        String *message = String_format("%s", this->errorMsg);
        error = Value_newString(message, NULL);
    } else {
        error = Value_newEmpty(NULL);
    }
    this->errorMsg = NULL;
    Executer_setValueByToken(this, name, error, true);
}

void _Executer_parseAppliable(Executer *this, Leaf *leaf, char type, Token **func, Leaf **code) {
    // func name
    Stack_RESTE(leaf->tokens);
    *func = Stack_NEXT(leaf->tokens);
    // func body
    Queue_RESTE(leaf->leafs);
    *code = Queue_NEXT(leaf->leafs);
    // with key
    if (!Token_isKey(*func)) return;
    // validate place
    Value *place = NULL;
    Executer_findValueByToken(this, *func, &place, &INVALID_VAL);
    Executer_assert(this, place != NULL, *func, LANG_ERR_EXECUTER_CONTAINER_NOT_FOUND);
    if (type == UG_TYPE_WKR && !Holdable_isProxy(place)) {
        return;
    } else if (type == UG_TYPE_CTR && Holdable_isModule(place)) {
        return;
    } else if (type == UG_TYPE_ATR && Holdable_isModule(place)) {
        return;
    }
    Executer_assert(this, Token_isName(*func), *func, LANG_ERR_EXECUTER_INVALID_NAME);
}

void Executer_consumeWorker(Executer *this, Leaf *leaf)
{
    Token *func; Leaf *code;
    _Executer_parseAppliable(this, leaf, UG_TYPE_WKR, &func, &code);
    //
    Holdable *env = this->machine->currHoldable;
    Runnable *wkr = Runnable_newWorker(code, func, env);
    Executer_setValueByToken(this, func, wkr, true);
}

void Executer_consumeCreator(Executer *this, Leaf *leaf)
{
    Token *func; Leaf *code;
    _Executer_parseAppliable(this, leaf, UG_TYPE_CTR, &func, &code);
    //
    Objective *self = Objective_newCtr(func);
    Holdable *env = this->machine->currHoldable;
    Value *funV = Runnable_newWorker(code, func, env);
    Executer_setValueToContainer(this, self, Token_function(), funV);
    Machine_releaseObj(funV);
    //
    Executer_setValueByToken(this, func, self, true);
}

void Executer_consumeAssister(Executer *this, Leaf *leaf)
{
    Token *func; Leaf *code;
    _Executer_parseAppliable(this, leaf, UG_TYPE_ATR, &func, &code);
    //
    Objective *self = Objective_newAtr(func);
    Holdable *env = this->machine->currHoldable;
    Value *funV = Runnable_newWorker(code, func, env);
    Executer_setValueToContainer(this, self, Token_function(), funV);
    Machine_releaseObj(funV);
    //
    Executer_setValueByToken(this, func, self, true);
}

void Executer_consumeCode(Executer *this, Leaf *leaf)
{
    Stack_reverse(this->callStack);
    Stack_RESTE(this->callStack);
    Stack_RESTE(leaf->tokens);
    Token *funcName = Stack_NEXT(leaf->tokens);
    Token *arg = Stack_NEXT(leaf->tokens);
    while(arg != NULL)
    {
        Value *value = Stack_NEXT(this->callStack);
        Executer_setValueToContainer(this, this->machine->currHoldable, arg, value);
        arg = Stack_NEXT(leaf->tokens);
    }
    //
    Stack_clear(this->callStack);
    Executer_consumeTree(this, leaf);
}

Value *Executer_executeFunctions(Executer *this, Runnable *func, Value *self, CString name)
{
    Holdable *environment = func->linka;
    tools_assert(environment != NULL, LANG_ERR_EXECUTER_INVALID_STATE);
    Machine_pushHolder(this->machine, environment);
    Executer_pushScope(this, name);
    Dictable_setLocation(this->machine->currHoldable, SCOPE_ALIAS_SLF, self);
    Executer_consumeLeaf(this, func->obj);
    Executer_popScope(this);
    Holdable *_environment = Machine_popHolder(this->machine);
    tools_assert(environment == _environment, LANG_ERR_EXECUTER_INVALID_STATE);
    Value *r = Stack_pop(this->callStack);
    this->isReturn = false;
    return r;
}

Value *Executer_applyWorker(Executer *this, Token *token, Value *workerValue, Value *container)
{
    Holdable *self = NULL;
    if (Objective_isAtr(container)) {
        self = Machine_getCurrentSelf(this->machine);
    } else if (Objective_isObj(container) || Dictable_isDct(container) || Listable_isLst(container)) {
        self = container;
    }
    //
    Value *r = Executer_executeFunctions(this, workerValue, self, TVALUE_WORKER);
    if (r == NULL) r = Value_newEmpty(NULL);
    return r;
}

Value *Executer_applyCreator(Executer *this, Token *token, Value *creatorValue, Value *container)
{
    Value *func = Executer_getValueFromContainer(this, creatorValue, Token_function());
    Executer_assert(this, Runnable_isWorker(func), token, LANG_ERR_EXECUTER_APPLY_NOT_VALID);
    //
    Queue *parents = Queue_new();
    Queue_push(parents, creatorValue);
    Objective *self = Objective_newObj(NULL, parents);
    //
    Value *r = Executer_executeFunctions(this, func, self, TVALUE_CREATOR);
    if (r == NULL) r = self;
    return r;
}

Value *Executer_applyAssister(Executer *this, Token *token, Value *assisterValue, Value *container)
{
    Value *func = Executer_getValueFromContainer(this, assisterValue, Token_function());
    Executer_assert(this, Runnable_isWorker(func), token, LANG_ERR_EXECUTER_APPLY_NOT_VALID);
    //
    Objective *self = Machine_getCurrentSelf(this->machine);
    Executer_assert(this, Objective_isObj(self), token, LANG_ERR_EXECUTER_APPLY_NOT_VALID);
    Queue *parent = self->extra;
    Queue_push(parent, assisterValue);
    //
    Value *r = Executer_executeFunctions(this, func, self, TVALUE_ASSISTER);
    if (r == NULL) r = self;
    return r;
}

Value *Executer_applyNative(Executer *this, Token *token, Value *nativeValue, Holdable *container)
{
    Bridge *bridge = this->uyghur->bridge;
    Bridge_startArgument(bridge);
    // 
    Stack_RESTE(this->callStack);
    Value *value = Stack_NEXT(this->callStack);
    while (value != NULL)
    {
        Executer_assert(this, is_bridge_type(value->type), token, "invalid type not available in c");
        Machine_retainObj(value);
        Bridge_pushValue(bridge, value);
        value = Stack_NEXT(this->callStack);
    }
    if (container!= NULL && (is_type_simple(container->type) || is_type_complex(container->type))) {
        Bridge_pushValue(bridge, container);
    }
    Bridge_send(bridge);
    //
    Bridge_run(bridge, nativeValue);
    //
    tools_assert(bridge->type == BRIDGE_STACK_TP_RES, LANG_ERR_EXECUTER_BRIDGE_INVALID_RESULT);
    Value *r = Bridge_popValue(bridge);
    if (r == NULL) r = Value_newEmpty(NULL);
    return r;
}

Value *Executer_clearStack(Executer *this)
{
    Stack_clear(this->callStack);
}

Value *Executer_pushStack(Executer *this, Value *value)
{
    Stack_push(this->callStack, value);
}

void Executer_consumeApply(Executer *this, Leaf *leaf)
{
    Stack_clear(this->callStack);
    Stack_RESTE(leaf->tokens);
    // get runnable name and result name
    Token *runnableName = Stack_NEXT(leaf->tokens);
    Token *resultName = Stack_NEXT(leaf->tokens);
    // push args
    Token *arg = Stack_NEXT(leaf->tokens);
    while(arg != NULL)
    {
        Value *value = Executer_getValueByToken(this, arg, true);
        Executer_pushStack(this, value);
        Machine_releaseObj(value);
        arg = Stack_NEXT(leaf->tokens);
    }
    // get runable
    Value *runnableValue = NULL;
    Value *runnableContainer = NULL;
    Executer_findValueByToken(this, runnableName, &runnableContainer, &runnableValue);
    Executer_assert(this, runnableContainer != NULL, runnableName, LANG_ERR_EXECUTER_RUNNABLE_NOT_FOUND);
    Executer_assert(this, runnableValue != NULL, runnableName, LANG_ERR_EXECUTER_RUNNABLE_NOT_FOUND);
    // run runnable
    Value *r = NULL;
    Debug_pushTrace(this->uyghur->debug, runnableName);
    if (runnableValue->type == UG_TYPE_WKR) {
        r = Executer_applyWorker(this, runnableName, runnableValue, runnableContainer);
    } else if (runnableValue->type == UG_TYPE_CTR) {
        r = Executer_applyCreator(this, runnableName, runnableValue, runnableContainer);
    } else if (runnableValue->type == UG_TYPE_ATR) {
        r = Executer_applyAssister(this, runnableName, runnableValue, runnableContainer);
    } else if (runnableValue->type == UG_TYPE_NTV) {
        r = Executer_applyNative(this, runnableName, runnableValue, runnableContainer);
    } else {
        Executer_error(this, runnableName, "LANG_ERR_EXECUTER_RUNNABLE_NOT_VALID");
    }
    Debug_popTrace(this->uyghur->debug, NULL);
    // return result
    if (!Token_isEmpty(resultName)) {
        Executer_setValueByToken(this, resultName, r, true);
    } else {
        Machine_releaseObj(r);
    }
    // release objects
    // Machine_releaseObj(runnableValue);
    Stack_clear(this->callStack);
}

void Executer_consumeResult(Executer *this, Leaf *leaf)
{
    Stack_RESTE(leaf->tokens);
    Token *result = Stack_NEXT(leaf->tokens);
    Value *value = Executer_getValueByToken(this, result, true);
    Stack_clear(this->callStack);
    Executer_pushStack(this, value);
    this->isReturn = true;
}

Value *Executer_calculateBTree(Executer *this, Foliage *);
Value *Executer_calculateBTree(Executer *this, Foliage *foliage)
{
    Value *result = NULL;
    Token *sign = NULL;
    Token *token = foliage->data;
    if (foliage->left != NULL && foliage->right != NULL) {
        Value *leftR = Executer_calculateBTree(this, foliage->left);
        Value *rightR = Executer_calculateBTree(this, foliage->right);
        result = Executer_calculateValues(this, leftR, token, rightR);
        Machine_releaseObj(leftR);
        Machine_releaseObj(rightR);
    } else if (foliage->left != NULL) {
        result = Executer_calculateBTree(this, foliage->left);
    } else if (foliage->right != NULL) {
        result = Executer_calculateBTree(this, foliage->right);
    } else if (is_eq_string(token->type, UG_TTYPE_WRD)) {
        Executer_assert(this, is_eq_strings(token->value, TVAUES_GROUP_UTYPES), token, LANG_ERR_EXECUTER_CALCULATION_INVALID_ARGS);
        result = Executer_getValueByToken(this, token, true);
    } else {
        Executer_assert(this, is_eq_strings(token->type, TTYPES_GROUP_VALUES), token, LANG_ERR_EXECUTER_CALCULATION_INVALID_ARGS);
        result = Executer_getValueByToken(this, token, true);
    }
    return result;
}

void Executer_consumeCalculator(Executer *this, Leaf *leaf)
{
    Stack_RESTE(leaf->tokens);
    Token *body = Stack_NEXT(leaf->tokens);
    Token *target = Stack_NEXT(leaf->tokens);
    Foliage *root = (Foliage *)body->value;
    //
    // TODO:free r object
    Value *r = Executer_calculateBTree(this, root);
    // 
    Executer_assert(this, r != NULL, target, LANG_ERR_EXECUTER_CALCULATION_INVALID);
    Executer_setValueByToken(this, target, r, false);
}

Value *Executer_generateContainer(Executer *this, Object *, Token *);
Value *Executer_generateContainer(Executer *this, Object *object, Token *token)
{
    bool isArr = object->objType == PCT_OBJ_QUEUE;
    bool isMap = object->objType == PCT_OBJ_STACK;
    Block *block = NULL;
    if (isArr) {
        Queue_RESTE(object);
        block = Queue_NEXT(object);
    } else if (isMap) {
        Stack_reverse(object);
        Stack_RESTE(object);
        block = Stack_NEXT(object);
    }
    //
    Value *result = isArr ? (Value *)Listable_newLst(token) : (Value *)Dictable_newDct(token);
    while(block != NULL)
    {
        Token *key = block->next;
        Object *val = block->data;
        bool noKey = key == NULL;
        Executer_assert(this, isArr == noKey, NULL, LANG_ERR_EXECUTER_GENERATION_INVALID_KEY);
        //
        Value *value = NULL;
        if (val->objType == PCT_OBJ_QUEUE) {
            value = Executer_generateContainer(this, val, NULL);
        } else if (val->objType == PCT_OBJ_STACK) {
            value = Executer_generateContainer(this, val, NULL);
        } else {
            value = Executer_getValueByToken(this, val, false);
        }
        //
        if (value == NULL) continue;
        //
        if (isArr) {
            Listable_push(result, value);
        } else if (isMap) {
            char *location = Executer_genLocationOfToken(this, key);
            Executer_assert(this, location != NULL, key, "generating invalid location to dict");
            Dictable_setLocation(result, location, value);
        } else {
            Executer_error(this, NULL, LANG_ERR_EXECUTER_GENERATION_INVALID);
        }
        //
        block = Stack_NEXT(object);
    }
    return result;
}

void Executer_consumeGenerator(Executer *this, Leaf *leaf)
{
    Stack_RESTE(leaf->tokens);
    Token *body = Stack_NEXT(leaf->tokens);
    Token *target = Stack_NEXT(leaf->tokens);
    Object *root = (Foliage *)body->value;
    //
    // TODO:free r object
    Value *r = Executer_generateContainer(this, root, target);
    //
    Executer_assert(this, r != NULL, target, LANG_ERR_EXECUTER_GENERATION_INVALID);
    Executer_setValueByToken(this, target, r, false);
}

void Executer_consumeLeaf(Executer *this, Leaf *leaf)
{
    char tp = leaf->type;
    // log_debug("executer.next: %c", tp);
    // throwing
    if (setjmp(jump_buffer) != 0 || (this->errorMsg != NULL && tp != UG_ATYPE_EXC)) {
        return;
    }
    // variable
    if (tp == UG_ATYPE_VAR)
    {
        Executer_consumeVariable(this, leaf);
        return;
    }
    // command
    if (tp == UG_ATYPE_CMD)
    {
        Executer_consumeCommand(this, leaf);
        return;
    }
    // expression
    if (tp == UG_ATYPE_CVT)
    {
        Executer_consumeConvert(this, leaf);
        return;
    }
    // if
    if (tp == UG_ATYPE_IF)
    {
        Executer_consumeIf(this, leaf);
        return;
    }
    // while
    if(tp == UG_ATYPE_WHL)
    {
        Executer_consumeWhile(this, leaf);
        return;
    }
    // spread
    if(tp == UG_ATYPE_SPR)
    {
        Executer_consumeSpread(this, leaf);
        return;
    }
    // exception
    if(tp == UG_ATYPE_EXC)
    {
        Executer_consumeException(this, leaf);
        return;
    }
    // worker
    if(tp == UG_ATYPE_WRKR) {
        Executer_consumeWorker(this, leaf);
        return;
    }
    // creator
    if(tp == UG_ATYPE_CRTR) {
        Executer_consumeCreator(this, leaf);
        return;
    }
    // assister
    if(tp == UG_ATYPE_ASTR) {
        Executer_consumeAssister(this, leaf);
        return;
    }
    // apply
    if(tp == UG_ATYPE_APPLY)
    {
        Executer_consumeApply(this, leaf);
        return;
    }
    // code
    if(tp == UG_ATYPE_CODE)
    {
        Executer_consumeCode(this, leaf);
        return;
    }
    // result
    if(tp == UG_ATYPE_RSLT)
    {
        Executer_consumeResult(this, leaf);
        return;
    }
    // calculator
    if (tp == UG_ATYPE_CLC)
    {
        Executer_consumeCalculator(this, leaf);
        return;
    }
    // generator
    if (tp == UG_ATYPE_GNR)
    {
        Executer_consumeGenerator(this, leaf);
        return;
    }
    // end
    if(tp == UG_ATYPE_END)
    {
        return;
    }
    //
    log_error("executer.error: %c", tp);
    helper_print_leaf(leaf, " ");
    tools_error("%s:[%c]", LANG_ERR_EXECUTER_NOT_IMPLEMENTED, tp);
}

void Executer_consumeTree(Executer *this, Leaf *tree)
{
    Queue_RESTE(tree->leafs);
    Leaf *leaf = Queue_NEXT(tree->leafs);
    while (leaf != NULL)
    {
        Machine_tryGC(this->machine);
        Executer_consumeLeaf(this, leaf);
        if (this->isReturn) break;
        leaf = Queue_NEXT(tree->leafs);
    }
}

Value *Executer_executeTree(Executer *this, char *path, Leaf *tree)
{
    //
    Holdable *module = Holdable_newModule(path);
    Machine_pushHolder(this->machine, module);
    //
    Executer_consumeTree(this, tree);
    if (this->machine->rootModule == this->machine->currHoldable) {
        timer_loop();
        return NULL;
    }
    //
    Holdable *holdable = Machine_popHolder(this->machine);
    tools_assert(holdable != NULL && holdable->type == UG_TYPE_MDL, LANG_ERR_EXECUTER_INVALID_STATE);
    //
    Dictable_setLocation(this->globalScope, path, holdable);
    return holdable;
}

void Executer_free(Executer *this)
{
    Executer_reset(this);
    free(this);
}
