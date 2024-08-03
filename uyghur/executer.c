// executer

#include "others/header.h"

jmp_buf jump_buffer;
struct Executer {
    Uyghur *uyghur;
    Leaf *tree;
    Leaf *leaf;
    Container *globalScope;
    Container *rootModule;
    Container *currContainer;
    Stack *containerStack;
    Stack *callStack;
    bool isReturn;
    bool isCatch;
    char *errorMsg;
};

void Executer_consumeLeaf(Executer *, Leaf *);
bool Executer_consumeTree(Executer *, Leaf *);
char *Executer_getLocationOfToken(Executer *, Token *);

void Executer_reset(Executer *this)
{
    this->uyghur = NULL;
    this->tree = NULL;
    this->leaf = NULL;
    this->globalScope = Container_newScope();
    this->rootModule = NULL;
    this->currContainer = NULL;
    this->containerStack = Stack_new();
    this->callStack = Stack_new();
    this->isReturn = false;
    this->isCatch = false;
    this->errorMsg = NULL;
    Object_retain(this->globalScope);
}

Executer *Executer_new(Uyghur *uyghur)
{
    Executer *executer = malloc(sizeof(Executer));
    Executer_reset(executer);
    executer->uyghur = uyghur;
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

void Executer_pushContainer(Executer *this, char type)
{
    Executer_assert(this, this->containerStack->size < MAX_STACK_SIZE, NULL, LANG_ERR_EXECUTER_STACK_OVERFLOW);
    Container *container = Container_new(type);
    Stack_push(this->containerStack, container);
    this->currContainer = (Container *)this->containerStack->tail->data;
    this->rootModule = (Container *)this->containerStack->head->data;
}

Container *Executer_popContainer(Executer *this, char type)
{
    Container *container = Stack_pop(this->containerStack);
    this->currContainer = (Container *)this->containerStack->tail->data;
    tools_assert(container != NULL && container->type == type, LANG_ERR_EXECUTER_INVALID_STATE);
    if (type == UG_CTYPE_SCP) {
        Object_release(container);
        return NULL;
    } else {
        return container;
    }
}

Container *Executer_getCurrentGlobal(Executer *this, Token *token)
{
    Container *container = this->globalScope;
    Executer_assert(this, container != NULL, token, LANG_ERR_EXECUTER_CONTAINER_NOT_FOUND);
    return container;
}

Container *Executer_getCurrentModule(Executer *this, Token *token)
{
    Cursor *cursor = Stack_reset(this->containerStack);
    Container *container = NULL;
    while ((container =  Stack_next(this->containerStack, cursor)) != NULL)
    {
        if (Container_isModule(container)) break;
    }
    Cursor_free(cursor);
    Executer_assert(this, Container_isModule(container), token, LANG_ERR_EXECUTER_CONTAINER_NOT_FOUND);
    return container;
}

Container *Executer_getCurrentBox(Executer *this, Token *token)
{
    Cursor *cursor = Stack_reset(this->containerStack);
    Container *container = NULL;
    while ((container =  Stack_next(this->containerStack, cursor)) != NULL)
    {
        if (!Container_isScope(container)) break;
        Value *self = Container_getLocation(container, SCOPE_ALIAS_BOX);
        if (self != NULL) {
            container = self->object;
            break;
        }
    }
    Cursor_free(cursor);
    Executer_assert(this, Container_isBox(container), token, LANG_ERR_EXECUTER_CONTAINER_NOT_FOUND);
    return container;
}

Container *Executer_getCurrentScope(Executer *this, Token *token)
{
    Container *container = this->currContainer;
    Executer_assert(this, container != NULL, token, LANG_ERR_EXECUTER_CONTAINER_NOT_FOUND);
    return container;
}

void Executer_findValueByLocation(Executer *this, char *key, Container **rContainer, Value **rValue)
{
    *rContainer = NULL;
    *rValue = NULL;
    Block *block = this->containerStack->tail;
    while (block != NULL) {
        Value *value = Container_getLocation(block->data, key);
        if (value != NULL) {
            *rContainer = block->data;
            *rValue = value;
            break;
        } else if (Container_isModule(block->data)) {
            break;
        }
        block = block->last;
    }
    if (*rContainer == NULL || *rValue == NULL) {
        Value *value = Container_getLocation(this->globalScope, key);
        if (value != NULL) {
            *rContainer = this->globalScope;
            *rValue = value;
        }
    }
}

void Executer_findValueByToken(Executer *this, Token *token, Container **rContainer, Value **rValue)
{
    *rContainer = NULL;
    *rValue = NULL;
    // name
    if (Token_isName(token)) {
        char *location = convert_string_to_location(token->value, UG_TYPE_STR);
        Executer_findValueByLocation(this, location, rContainer, rValue);
        pct_free(location);
        return;
    }
    // const
    Executer_assert(this, Token_isKey(token), token, LANG_ERR_EXECUTER_INVALID_KEY);
    Token *extra = (Token *)token->extra;
    if (is_eq_string(extra->value, SCOPE_ALIAS_PRG)) *rContainer = Executer_getCurrentGlobal(this, token);
    if (is_eq_string(extra->value, SCOPE_ALIAS_MDL)) *rContainer = Executer_getCurrentModule(this, token);
    if (is_eq_string(extra->value, SCOPE_ALIAS_BOX)) *rContainer = Executer_getCurrentBox(this, token);
    // box
    if (*rContainer == NULL) {
        Value *value;
        char *location = convert_string_to_location(extra->value, UG_TYPE_STR);
        Executer_findValueByLocation(this, location, &INVALID_CTN, &value);
        pct_free(location);
        if (value != NULL) *rContainer = value->object;
    }
    if (*rContainer == NULL) return;
    // key
    char *location = convert_string_to_location(token->value, UG_TYPE_STR);
    *rValue = Container_getLocation(*rContainer, location);
    pct_free(location);
}

// need free
char *Executer_getLocationOfToken(Executer *this, Token *token)
{
    char *key;
    if (Token_isKeyOfName(token)) {
        char *location = convert_string_to_location(token->value, UG_TYPE_STR);
        Container *container;
        Value *value;
        Executer_findValueByLocation(this, location, &container, &value);
        pct_free(location);
        Executer_assert(this, container!= NULL && value!= NULL, token, LANG_ERR_EXECUTER_INVALID_BOX);
        char *text = Value_toString(value);
        key = convert_string_to_location(text, value->type);
        pct_free(text);
    } else if (Token_isKeyOfNumber(token) || Token_isKeyOfString(token)) {
        Token *extra = (Token *)token->extra;
        char tp = convert_ttype_to_rtype(extra->type);
        key = convert_string_to_location(token->value, tp);
    } else {
        char tp = convert_ttype_to_rtype(token->type);
        key = convert_string_to_location(token->value, tp);
    }
    return key;
}

// 

// need release
Value *Executer_getValueFromContainer(Executer *this, Container *container, Token *token)
{
    Executer_assert(this, container != NULL, token, LANG_ERR_EXECUTER_INVALID_VARIABLE);
    char *location = Executer_getLocationOfToken(this, token);
    Value *value = Container_getLocation(container, location);
    if (value != NULL) Object_retain(value);
    pct_free(location);
    return value;
}

void Executer_setValueToContainer(Executer *this, Container *container, Token *token, Value *value)
{
    Executer_assert(this, container != NULL, token, LANG_ERR_EXECUTER_INVALID_VARIABLE);
    char *location = Executer_getLocationOfToken(this, token);
    Container_setLocation(container, location, value);
    pct_free(location);
}

// 

// need release
Value *Executer_getValueByToken(Executer *this, Token *token, bool withEmpty)
{
    Value *value = convert_token_to_value(token);
    if (value != NULL) return value;
    Executer_findValueByToken(this, token, &INVALID_CTN, &value);
    if (value != NULL) {
        Object_retain(value);
    } else if (withEmpty) {
        value = Value_newEmpty(token);
    }
    return value;
}

void *Executer_setValueByToken(Executer *this, Token *token, Value *value, bool withDeclare)
{
    Container *container;
    Executer_findValueByToken(this, token, &container, &INVALID_VAL);
    if (withDeclare && container == NULL) container = this->currContainer;
    Executer_assert(this, container != NULL, token, LANG_ERR_EXECUTER_INVALID_VARIABLE);
    Executer_setValueToContainer(this, container, token, value);
    Object_release(value);
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
    Cursor *cursor = Stack_reset(leaf->tokens);
    Token *token = Stack_next(leaf->tokens, cursor);
    Token *name = Stack_next(leaf->tokens, cursor);
    Cursor_free(cursor);
    // 
    Value *new = NULL;
    if (Token_isEmpty(token)) {
        new = Value_newEmpty(token);
    } else if (Token_isWord(token) && is_eq_string(token->value, TVALUE_LOGIC)) {
        new = Value_newBoolean(false, token);
    } else if (Token_isWord(token) && is_eq_string(token->value, TVALUE_NUM)) {
        new = Value_newNumber(0, token);
    } else if (Token_isWord(token) && is_eq_string(token->value, TVALUE_STR)) {
        new = Value_newString(String_new(), token);
    } else if (Token_isWord(token) && is_eq_string(token->value, TVALUE_BOX)) {
        new = Value_newContainer(Container_newBox(), token);
    } else {
        new = Executer_getValueByToken(this, token, true); // todo
    }
    // 
    Value *old = Executer_getValueFromContainer(this, this->currContainer, name);
    Executer_assert(this, old == NULL, name, LANG_ERR_EXECUTER_VARIABLE_ALREADY_DEFINED);
    Executer_assert(this, new != NULL, name, LANG_ERR_EXECUTER_VARIABLE_FAILURE_DEFINED);
    Executer_setValueToContainer(this, this->currContainer, name, new);
    Object_release(new);
}

void Executer_consumeCommand(Executer *this, Leaf *leaf)
{
    Cursor *cursor = Stack_reset(leaf->tokens);
    Token *action = Stack_next(leaf->tokens, cursor);
    Token *name = Stack_next(leaf->tokens, cursor);
    Cursor_free(cursor);
    if (is_eq_string(action->value, TVALUE_CMD_OUTPUT))
    {
        Value *value = NULL;
        if (!Token_isString(name) && helper_token_is_values(name, TVAUES_GROUP_UTYPES)) {
            printf("<Type n:%s>\n", name->value);
            if (is_eq_string(name->value, TVALUE_EMPTY)) {
                Container_print(this->globalScope);
            } else {
                value = Executer_getValueFromContainer(this, this->globalScope, name);
            }
        } else {
            value = Executer_getValueByToken(this, name, true);
            Executer_assert(this, value != NULL, name, LANG_ERR_EXECUTER_VARIABLE_NOT_FOUND);
        }
        if (Value_isContainer(value)) {
            Container_print(value->object);
        } else {
            char *content = Value_toString(value);
            printf("%s", content);
            pct_free(content);
        }
        if (value != NULL) Object_release(value);
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
    Cursor *cursor = Stack_reset(leaf->tokens);
    Token *action = Stack_next(leaf->tokens, cursor);
    Token *target = Stack_next(leaf->tokens, cursor);
    Cursor_free(cursor);
    Value *value = Executer_getValueByToken(this, target, true);
    Value *r = NULL;
    char *act = action->value;
    //
    if (is_eq_string(action->type, UG_TTYPE_WRD))
    {
        if (is_eq_string(act, TVALUE_EMPTY))
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
        else if (is_eq_string(act, TVALUE_LOGIC))
        {
            r = Value_toBoolean(value);
        }
        else if (is_eq_string(act, TVALUE_WORKER) || is_eq_string(act, TVALUE_CREATOR))
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
    tools_assert(r != NULL, "%s:[%s]", LANG_ERR_EXECUTER_CALCULATION_INVALID, act);
    Executer_setValueByToken(this, target, r, false);
    Object_release(value);
}

bool Executer_checkJudge(Executer *this, Leaf *leaf)
{
    //
    Cursor *cursor = Stack_reset(leaf->tokens);
    Token *judge = Stack_next(leaf->tokens, cursor);
    Token *first = Stack_next(leaf->tokens, cursor);
    Token *clcltn = Stack_next(leaf->tokens, cursor);
    Token *second = Stack_next(leaf->tokens, cursor);
    Cursor_free(cursor);
    //
    Value *firstV = first == NULL ? NULL : Executer_getValueByToken(this, first, true);
    Value *secondV = second == NULL ? NULL : Executer_getValueByToken(this, second, true);
    Executer_assert(this, firstV != NULL, first, LANG_ERR_EXECUTER_EXCEPTION);
    Value *resultV = NULL;
    //
    if (clcltn == NULL) {
        Executer_assert(this, secondV == NULL, first, LANG_ERR_EXECUTER_EXCEPTION);
        Object_retain(firstV);
        resultV = firstV;
    } else {
        Executer_assert(this, secondV != NULL, first, LANG_ERR_EXECUTER_EXCEPTION);
        resultV = Executer_calculateValues(this, firstV, clcltn, secondV);
    }
    // 
    bool shouldOk = is_eq_string(judge->value, TVALUE_THEN);
    bool isOk = Value_isTrue(resultV);
    // 
    if (firstV != NULL) Object_release(firstV);
    if (secondV != NULL) Object_release(secondV);
    if (resultV != NULL) Object_release(resultV);
    return isOk == shouldOk;
}

void Executer_consumeIf(Executer *this, Leaf *leaf)
{
    //
    bool isFinish = false;
    Cursor *cursor1 = Queue_reset(leaf->leafs);
    // if
    Leaf *ifNode = Queue_next(leaf->leafs, cursor1);
    tools_assert(ifNode != NULL, LANG_ERR_EXECUTER_INVALID_IF);
    tools_assert(ifNode->type == UG_ATYPE_IF_F, LANG_ERR_EXECUTER_INVALID_IF);
    if (!isFinish && Executer_checkJudge(this, ifNode))
    {
        Executer_pushContainer(this, UG_CTYPE_SCP);
        Executer_consumeTree(this, ifNode);
        Executer_popContainer(this, UG_CTYPE_SCP);
        isFinish = true;
    }
    // elseif
    ifNode = Queue_next(leaf->leafs, cursor1);
    while (ifNode->type == UG_ATYPE_IF_M)
    {
        if (!isFinish && Executer_checkJudge(this, ifNode))
        {
            Executer_pushContainer(this, UG_CTYPE_SCP);
            Executer_consumeTree(this, ifNode);
            Executer_popContainer(this, UG_CTYPE_SCP);
            isFinish = true;
        }
        ifNode = Queue_next(leaf->leafs, cursor1);
        tools_assert(ifNode != NULL, LANG_ERR_EXECUTER_INVALID_IF);
    }
    // else
    if (ifNode->type == UG_ATYPE_IF_L)
    {
        Cursor *cursor2 = Stack_reset(ifNode->tokens);
        Token *token = Stack_next(ifNode->tokens, cursor2);
        Cursor_free(cursor2);
        tools_assert(is_eq_string(token->value, TVALUE_ELSE), LANG_ERR_EXECUTER_INVALID_IF);
        if (!isFinish)
        {
            Executer_pushContainer(this, UG_CTYPE_SCP);
            Executer_consumeTree(this, ifNode);
            Executer_popContainer(this, UG_CTYPE_SCP);
            isFinish = true;
        }
        ifNode = Queue_next(leaf->leafs, cursor1);
        tools_assert(ifNode != NULL, LANG_ERR_EXECUTER_INVALID_IF);
    }
    // end
    tools_assert(ifNode->type == UG_ATYPE_END, LANG_ERR_EXECUTER_INVALID_IF);
    Leaf *nullValue = Queue_next(leaf->leafs, cursor1);
    tools_assert(nullValue == NULL, LANG_ERR_EXECUTER_INVALID_IF);
    Cursor_free(cursor1);
}

void Executer_consumeWhile(Executer *this, Leaf *leaf)
{
    while (Executer_checkJudge(this, leaf))
    {
        Executer_pushContainer(this, UG_CTYPE_SCP);
        Executer_consumeTree(this, leaf);
        Executer_popContainer(this, UG_CTYPE_SCP);
        if (this->errorMsg != NULL) break;
    }
}

void Executer_consumeSpread(Executer *this, Leaf *leaf)
{
    //
    Cursor *cursor = Stack_reset(leaf->tokens);
    Token *target = Stack_next(leaf->tokens, cursor);
    Token *iter1 = Stack_next(leaf->tokens, cursor);
    Token *iter2 = Stack_next(leaf->tokens, cursor);
    Cursor_free(cursor);
    Token_print(target);
    Token_print(iter1);
    Token_print(iter2);
    //
    Value *value = NULL;
    if (!Token_isWord(target)) {
        value = Executer_getValueByToken(this, target, true);
    } else if (
        is_eq_string(target->value, TVALUE_NUM)
        || is_eq_string(target->value, TVALUE_STR)
        || is_eq_string(target->value, TVALUE_BOX)
    ) {
        char *location = convert_string_to_location(target->extra, UG_TYPE_STR);
        value = Container_getLocation(this->globalScope, location);
        pct_free(location);
        if (value != NULL) Object_retain(value);
    }
    // 
    Value *current1;
    Value *current2;
    if (Value_isInt(value)) {
        for (size_t i = 0; i < value->number; i++)
        {
            Executer_pushContainer(this, UG_CTYPE_SCP);
            current1 = Value_newNumber(i, iter1);
            Executer_setValueByToken(this, iter1, current1, true);
            current2 = Value_newNumber(i + 1, iter2);
            Executer_setValueByToken(this, iter2, current2, true);
            Executer_consumeTree(this, leaf);
            Executer_popContainer(this, UG_CTYPE_SCP);
            if (this->errorMsg != NULL) break;
        }
    } else if (Value_isString(value)) {
        utf8_iter iterator;
        utf8_init(&iterator, String_get(value->string));
        while (utf8_next(&iterator)) {
            Executer_pushContainer(this, UG_CTYPE_SCP);
            current1 = Value_newNumber(iterator.position, iter1);
            Executer_setValueByToken(this, iter1, current1, true);
            current2 = Value_newString(String_format(utf8_getchar(&iterator)), iter2);
            Executer_setValueByToken(this, iter2, current2, true);
            Executer_consumeTree(this, leaf);
            Executer_popContainer(this, UG_CTYPE_SCP);
            if (this->errorMsg != NULL) break;
        }
    } else if (Value_isContainer(value)) {
        Container *box = value->object;
        Hashmap *map = box->map;
        Hashkey *ptr;
        for (int i = 0; i < HASHMAP_DEFAULT_CAPACITY; ++i) {
            ptr = map[i].position;
            while (ptr != NULL) {
                String *_key = ptr->key;
                Value *val = ptr->value;
                //
                Executer_pushContainer(this, UG_CTYPE_SCP);
                current1 = Value_newString(_key, iter1);
                Executer_setValueByToken(this, iter1, current1, true);
                current2 = val;
                Executer_setValueByToken(this, iter2, current2, true);
                Executer_consumeTree(this, leaf);
                Executer_popContainer(this, UG_CTYPE_SCP);
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
    if (value != NULL) Object_release(value);
}

void Executer_consumeException(Executer *this, Leaf *leaf)
{
    
    Cursor *cursor = Stack_reset(leaf->tokens);
    Token *name = Stack_next(leaf->tokens, cursor);
    Cursor_free(cursor);
    this->isCatch = true;
    // 
    Executer_pushContainer(this, UG_CTYPE_SCP);
    Executer_consumeTree(this, leaf);
    Executer_popContainer(this, UG_CTYPE_SCP);
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

void Executer_consumeWorker(Executer *this, Leaf *leaf)
{
    // worker name
    Cursor *cursor1 = Stack_reset(leaf->tokens);
    Token *worker = Stack_next(leaf->tokens, cursor1);
    Cursor_free(cursor1);
    // worker body
    Cursor *cursor2 = Queue_reset(leaf->leafs);
    Leaf *code = Queue_next(leaf->leafs, cursor2);
    Cursor_free(cursor2);
    // worker self
    Value *self = NULL;
    if (Token_isKey(worker)) {
        Container *container;
        Executer_findValueByToken(this, worker, &container, &INVALID_VAL);
        Executer_assert(this, container != NULL, worker, LANG_ERR_EXECUTER_CONTAINER_NOT_FOUND);
        if (Container_isBox(container)) {
            self = Value_newContainer(container, NULL);
        } else {
            log_error("TODO: register to self object of creator");
        }
    }
    // save worker
    Value *wkr = Value_newWorker(code, self);
    Executer_setValueByToken(this, worker, wkr, true);
}

void Executer_consumeCreator(Executer *this, Leaf *leaf)
{
    // creator name
    Cursor *cursor1 = Stack_reset(leaf->tokens);
    Token *creator = Stack_next(leaf->tokens, cursor1);
    Cursor_free(cursor1);
    // creator body
    Cursor *cursor2 = Queue_reset(leaf->leafs);
    Leaf *code = Queue_next(leaf->leafs, cursor2);
    Cursor_free(cursor2);
    //
    Executer_assert(this, Token_isName(creator), creator, LANG_ERR_EXECUTER_INVALID_NAME);
    // save constructor
    Container *self = Container_newBox();
    Token *funT = Token_name(FUNCTION_KEY);
    Value *funV = Value_newWorker(code, NULL);
    Executer_setValueToContainer(this, self, funT, funV);
    // save paent
    Container *base = NULL;
    // save creator
    Value *ctr = Value_newCreator(self, base);
    Executer_setValueByToken(this, creator, ctr, true);
}

void Executer_consumeCode(Executer *this, Leaf *leaf)
{
    Stack_reverse(this->callStack);
    Cursor *cursor1 = Stack_reset(this->callStack);
    Cursor *cursor2 = Stack_reset(leaf->tokens);
    Token *funcName = Stack_next(leaf->tokens, cursor2);
    Token *arg = Stack_next(leaf->tokens, cursor2);
    while(arg != NULL)
    {
        Value *value = Stack_next(this->callStack, cursor1);
        Executer_setValueToContainer(this, this->currContainer, arg, value);
        arg = Stack_next(leaf->tokens, cursor2);
    }
    Cursor_free(cursor1);
    Cursor_free(cursor2);
    //
    Stack_clear(this->callStack);
    Executer_consumeTree(this, leaf);
}

Value *Executer_applyWorker(Executer *this, Value *workerValue)
{
    Leaf *codeNode = workerValue->object;
    Value *selfValue = workerValue->extra;
    // 
    Executer_pushContainer(this, UG_CTYPE_SCP);
    Container_setLocation(this->currContainer, SCOPE_ALIAS_BOX, selfValue);
    Executer_consumeLeaf(this, codeNode);
    Executer_popContainer(this, UG_CTYPE_SCP);
    // 
    this->isReturn = false;
    Value *r = Stack_pop(this->callStack);
    if (r == NULL) {
        r = Value_newEmpty(NULL);
    } else {
        //
    }
    return r;
}

Value *Executer_applyCreator(Executer *this, Value *creatorValue, Token *creatorToken)
{
    Container *classContainer = creatorValue->object;
    Container *baseContainer = creatorValue->extra;
    //
    Token *funT = Token_name(FUNCTION_KEY);
    Value *funV = Executer_getValueFromContainer(this, classContainer, funT);
    Object_release(funT);
    //
    Leaf *codeNode = funV->object;
    // TODO: copy base to object
    // 
    Container *objectContainer = Container_newBox();
    Value *objectValue = Value_newObject(objectContainer, NULL);
    Container_copyTo(classContainer, objectContainer);
    // // 
    Executer_pushContainer(this, UG_CTYPE_SCP);
    Container_setLocation(this->currContainer, SCOPE_ALIAS_BOX, objectValue);
    Executer_consumeLeaf(this, codeNode);
    Executer_popContainer(this, UG_CTYPE_SCP);
    // 
    this->isReturn = false;
    Value *r = Stack_pop(this->callStack);
    if (r == NULL) {
        r = objectValue;
    } else {
        Object_release(objectValue);
    }
    return r;
}

Value *Executer_applyNative(Executer *this, Value *nativeValue)
{
    Bridge *bridge = this->uyghur->bridge;
    Bridge_startArgument(bridge);
    Cursor *cursor = Stack_reset(this->callStack);
    Value *value = Stack_next(this->callStack, cursor);
    while (value != NULL)
    {
        Object_retain(value);
        Bridge_pushValue(bridge, value);
        value = Stack_next(this->callStack, cursor);
    }
    Cursor_free(cursor);
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
    Cursor *cursor = Stack_reset(leaf->tokens);
    // get runnable name and result name
    Token *runnableName = Stack_next(leaf->tokens, cursor);
    Token *resultName = Stack_next(leaf->tokens, cursor);
    // push args
    Token *arg = Stack_next(leaf->tokens, cursor);
    while(arg != NULL)
    {
        Value *value = Executer_getValueByToken(this, arg, true);
        Executer_pushStack(this, value);
        Object_release(value);
        arg = Stack_next(leaf->tokens, cursor);
    }
    Cursor_free(cursor);
    // get runable
    Value *runnableValue = Executer_getValueByToken(this, runnableName, false);
    Executer_assert(this, runnableValue != NULL, runnableName, LANG_ERR_EXECUTER_RUNNABLE_NOT_FOUND);
    // run runnable
    Value *r = NULL;
    Debug_pushTrace(this->uyghur->debug, runnableName);
    if (runnableValue->type == UG_TYPE_WKR) {
        r = Executer_applyWorker(this, runnableValue);
    } else if (runnableValue->type == UG_TYPE_CTR) {
        r = Executer_applyCreator(this, runnableValue, runnableName);
    } else if (runnableValue->type == UG_TYPE_NTV) {
        r = Executer_applyNative(this, runnableValue);
    } else {
        Executer_error(this, runnableName, LANG_ERR_EXECUTER_RUNNABLE_NOT_VALID);
    }
    Debug_popTrace(this->uyghur->debug, NULL);
    // return result
    if (!Token_isEmpty(resultName)) {
        Executer_setValueByToken(this, resultName, r, true);
    } else {
        Object_release(r);
    }
    // release objects
    Object_release(runnableValue);
    Stack_clear(this->callStack);
}

void Executer_consumeResult(Executer *this, Leaf *leaf)
{
    Cursor *cursor = Stack_reset(leaf->tokens);
    Token *result = Stack_next(leaf->tokens, cursor);
    Cursor_free(cursor);
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
        Object_release(leftR);
        Object_release(rightR);
    } else if (foliage->left != NULL) {
        result = Executer_calculateBTree(this, foliage->left);
    } else if (foliage->right != NULL) {
        result = Executer_calculateBTree(this, foliage->right);
    } else {
        Executer_assert(this, is_eq_strings(token->type, TTYPES_GROUP_VALUES), token, LANG_ERR_EXECUTER_CALCULATION_INVALID_ARGS);
        result = Executer_getValueByToken(this, token, true);
    }
    return result;
}

void Executer_consumeCalculator(Executer *this, Leaf *leaf)
{
    Cursor *cursor = Stack_reset(leaf->tokens);
    Token *body = Stack_next(leaf->tokens, cursor);
    Token *target = Stack_next(leaf->tokens, cursor);
    Cursor_free(cursor);
    Foliage *root = (Foliage *)body->value;
    //
    // TODO:free r object
    Value *r = Executer_calculateBTree(this, root);
    //
    Executer_assert(this, r != NULL, target, LANG_ERR_EXECUTER_CALCULATION_INVALID_ARGS);
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
    // end
    if(tp == UG_ATYPE_END)
    {
        return;
    }
    //
    log_error("executer.error: %s", tp);
    helper_print_leaf(leaf, " ");
    tools_error("%s:[%c]", LANG_ERR_EXECUTER_NOT_IMPLEMENTED, tp);
}

bool Executer_consumeTree(Executer *this, Leaf *tree)
{
    Cursor *cursor = Queue_reset(tree->leafs);
    Leaf *leaf = Queue_next(tree->leafs, cursor);
    while (leaf != NULL)
    {
        Executer_consumeLeaf(this, leaf);
        if (this->isReturn) break;
        leaf = Queue_next(tree->leafs, cursor);
    }
    Cursor_free(cursor);
    return true;
}

Value *Executer_executeTree(Executer *this, char *path, Leaf *tree)
{
    Executer_pushContainer(this, UG_CTYPE_MDL);
    Executer_consumeTree(this, tree);
    if (this->containerStack->head == this->containerStack->tail) return NULL;
    Container *container = Executer_popContainer(this, UG_CTYPE_MDL);
    Value *module = Value_newContainer(container, NULL);
    Container_setLocation(this->globalScope, path, module);
    return module;
}

void Executer_free(Executer *this)
{
    Executer_reset(this);
    free(this);
}
