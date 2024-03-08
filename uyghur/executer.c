// executer

#include "others/header.h"

struct Executer {
    Uyghur *uyghur;
    Leaf *tree;
    Leaf *leaf;
    Stack *callStack;
    Stack *containerStack;
    Container *currentContainer;
    Container *rootContainer;
    Container *globalContainer;
    bool isReturn;
};

void Executer_consumeLeaf(Executer *, Leaf *);
bool Executer_consumeTree(Executer *, Leaf *);

void Executer_reset(Executer *this)
{
    this->uyghur = NULL;
    this->tree = NULL;
    this->leaf = NULL;
    this->callStack = Stack_new();
    this->containerStack = Stack_new();
    this->currentContainer = NULL;
    this->rootContainer = NULL;
    this->globalContainer = Container_newScope();
    this->isReturn = false;
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
    char *s = token == NULL ? LANG_UNKNOWN : format_token_place(token);
    char *err = tools_format("Executer: %s, %s", m, s);
    printf("[%s] => %s\n", LANG_ERR, err);
    Debug_writeTrace(this->uyghur->debug);
    exit(1);
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
    this->currentContainer = (Container *)this->containerStack->tail->data;
    this->rootContainer = (Container *)this->containerStack->head->data;
}

Container *Executer_popContainer(Executer *this, char type)
{
    Container *container = Stack_pop(this->containerStack);
    this->currentContainer = (Container *)this->containerStack->tail->data;
    tools_assert(container != NULL && container->type == type, LANG_ERR_NO_VALID_STATE);
    if (type == UG_CTYPE_SCP) {
        Object_release(container);
        return NULL;
    } else {
        return container;
    }
}

Container *Executer_getCurrentGlobal(Executer *this, Token *token)
{
    Container *container = this->globalContainer;
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
        if (Container_isBox(container) || Container_isModule(container)) break;
        // TODO: self
        // give a _ variable in fun call as argument
        // then search _ in scopes to find self box
    }
    Cursor_free(cursor);
    Executer_assert(this, Container_isBox(container), token, LANG_ERR_EXECUTER_CONTAINER_NOT_FOUND);
    return container;
}

Container *Executer_getCurrentScope(Executer *this, Token *token)
{
    Container *container = this->currentContainer;
    Executer_assert(this, container != NULL, token, LANG_ERR_EXECUTER_CONTAINER_NOT_FOUND);
    return container;
}

Container *Executer_getContainerByKey(Executer *this, char *key)
{
    Value *value = NULL;
    Block *block = this->containerStack->tail;
    while (value == NULL && block != NULL)
    {
        Container *container = block->data;
        Value *v = Container_get(container, key);
        if (v != NULL) value = v;
        if (v != NULL) break;
        if (Container_isModule(container)) break;
        block = block->last;
    }
    if (value != NULL) return block->data;
    value = Container_get(this->globalContainer, key);
    if (value != NULL) return this->globalContainer;
    return NULL;
}

Container *Executer_getContainerByToken(Executer *this, Token *token)
{
    // name
    if (Token_isName(token)) return Executer_getContainerByKey(this, token->value);
    Executer_assert(this, Token_isKey(token), token, LANG_ERR_EXECUTER_KEY_INVALID_TOKEN);
    Token *extra = (Token *)token->extra;
    if (is_equal(extra->value, SCOPE_ALIAS_PRG)) return Executer_getCurrentGlobal(this, token);
    if (is_equal(extra->value, SCOPE_ALIAS_MDL)) return Executer_getCurrentModule(this, token);
    if (is_equal(extra->value, SCOPE_ALIAS_BOX)) return Executer_getCurrentBox(this, token);
    Container *container = Executer_getContainerByKey(this, extra->value);
    if (container == NULL) return NULL;
    Value *value = Container_get(container, extra->value);
    if (value == NULL) return NULL;
    Executer_assert(this, value->type == UG_RTYPE_CNT, token, LANG_ERR_EXECUTER_INVALID_BOX_NAME);
    return value->object;
}

// TODO: release keys
char *Executer_getKeyByToken(Executer *this, Token *token)
{
    char *key = token->value;
    if (!Token_isKey(token)) {
        return tools_format("%s", key);
    }
    Token *extra = (Token *)token->extra;
    if (Token_isNumber(extra)) {
        String *s = String_format("%f", atof(token->value));
        key = String_dump(s);
        String_free(s);
    } else if (Token_isString(extra)) {
        String *s = String_format("%s", token->value);
        key = String_dump(s);
        String_free(s);
    } else if (Token_isName(extra)) {
        Container *container = Executer_getContainerByKey(this, token->value);
        Executer_assert(this, container!= NULL, token, LANG_ERR_EXECUTER_INVALID_KEY_NAME);
        Value *value = Container_get(container, token->value);
        Executer_assert(this, value!= NULL, token, LANG_ERR_EXECUTER_INVALID_KEY_NAME);
        key = Value_toString(value);
    }
    return key;
}

Value *Executer_getValueByToken(Executer *this, Token *token, bool withEmpty)
{
    if (Token_isEmpty(token)) return Value_newEmpty(token);
    if (Token_isBool(token)) return Value_newBoolean(is_equal(token->value, TVALUE_TRUE), token);
    if (Token_isNumber(token)) return Value_newNumber(atof(token->value), token);
    if (Token_isString(token)) return Value_newString(String_format("%s", token->value), token);
    if (Token_isBox(token)) return Value_newContainer(Container_newBox(), token);
    //
    char *key = Executer_getKeyByToken(this, token);
    Container *container = Executer_getContainerByToken(this, token);
    Executer_assert(this, container != NULL, token, LANG_ERR_EXECUTER_INVALID_VARIABLE);
    Value *result = Container_get(container, key);
    if (result != NULL) {
        Object_retain(result);
    } else if (withEmpty) {
        result = Value_newEmpty(token);
    }
    free(key);
    return result;
}

void *Executer_setValueByToken(Executer *this, Token *token, Value *value, bool withDeclare)
{
    Container *container = Executer_getContainerByToken(this, token);
    if (withDeclare && container == NULL) container = this->currentContainer;
    Executer_assert(this, container != NULL, token, LANG_ERR_EXECUTER_INVALID_VARIABLE);
    char *key = Executer_getKeyByToken(this, token);
    Value *replacedValue = Container_set(container, key, value);
    pct_free(key);
    Object_release(value);
    // if (replacedValue != NULL) Object_release(replacedValue);
}

void Executer_consumeVariable(Executer *this, Leaf *leaf)
{
    Cursor *cursor = Stack_reset(leaf->tokens);
    Token *token = Stack_next(leaf->tokens, cursor);
    Token *name = Stack_next(leaf->tokens, cursor);
    Cursor_free(cursor);
    Container *container = Executer_getCurrentScope(this, name);
    Value *old = Container_get(container, name->value);
    Value *new = Executer_getValueByToken(this, token, true);
    Executer_assert(this, old == NULL, name, LANG_ERR_EXECUTER_VARIABLE_ALREADY_DEFINED);
    char *key = Executer_getKeyByToken(this, name);
    Container_set(container, key, new);
    Object_release(new);
    pct_free(key);
}

void Executer_consumeOperate(Executer *this, Leaf *leaf)
{
    Cursor *cursor = Stack_reset(leaf->tokens);
    Token *action = Stack_next(leaf->tokens, cursor);
    Token *name = Stack_next(leaf->tokens, cursor);
    Token *target = Stack_next(leaf->tokens, cursor);
    Cursor_free(cursor);
    if (is_equal(target->value, TVALUE_TARGET_TO) && is_equal(action->value, TVALUE_OUTPUT))
    {
        Value *value = Executer_getValueByToken(this, name, true);
        Executer_assert(this, value != NULL, name, LANG_ERR_NO_VALID_STATE);
        char *content = Value_toString(value);
        printf("%s", content);
        Object_release(value);
        pct_free(content);
    }
    else if (is_equal(target->value, TVALUE_TARGET_FROM) && is_equal(action->value, TVALUE_INPUT))
    {
        char line[1024];
        scanf(" %[^\n]", line);
        Executer_setValueByToken(this, name, Value_newString(String_format("%s", line), NULL), false);
    }
}

void Executer_consumeExpSingle(Executer *this, Leaf *leaf)
{
    Cursor *cursor = Stack_reset(leaf->tokens);
    Token *action = Stack_next(leaf->tokens, cursor);
    Token *target = Stack_next(leaf->tokens, cursor);
    Cursor_free(cursor);
    Value *value = Executer_getValueByToken(this, target, true);
    Value *r = NULL;
    char *act = action->value;
    //
    if (is_equal(action->type, UG_TTYPE_WRD))
    {
        if (is_equal(act, TVALUE_EMPTY))
        {
            // TODO free object
            r = Value_newEmpty(NULL);
        }
        else if (is_equal(act, TVALUE_NOT))
        {
            if (value->type == UG_RTYPE_NUM)
            {
                r = Value_newBoolean(value->number <= 0, NULL);
            }
            else if (value->type == UG_RTYPE_STR)
            {
                r = Value_newBoolean(!is_equal(String_get(value->string), TVALUE_TRUE), NULL);
            }
            else if (value->type == UG_RTYPE_NIL)
            {
                r = Value_newBoolean(true, NULL);
            }
            else if (value->type == UG_RTYPE_BOL)
            {
                r = Value_newBoolean(!value->boolean, NULL);
            }
            else if (value->type == UG_RTYPE_FUN)
            {
                r = Value_newBoolean(false, NULL);
            }
        }
        else if (is_equal(act, TVALUE_NUM))
        {
            r = Value_toNumber(value);
        }
        else if (is_equal(act, TVALUE_STR))
        {
            char *content = Value_toString(value);
            r = Value_newString(String_format("%s", content), NULL);
            pct_free(content);
        }
        else if (is_equal(act, TVALUE_BOOLEAN))
        {
            r = Value_toBoolean(value);
        }
        else if (is_equal(act, TVALUE_FUNCTION))
        {
            char *funcName = String_get(value->string);
            Token *funcToken = Token_name(funcName);
            // TODO: memory leak
            Value *funcValue = Executer_getValueByToken(this, funcToken, true);
            if(funcValue->type == UG_RTYPE_FUN || funcValue->type == UG_RTYPE_NTV)
            {
                r = funcValue;
            }
            else
            {
                r = Value_newEmpty(NULL);
            }
        }
    }
    else
    {
        r = Executer_getValueByToken(this, action, true);
    }
    tools_assert(r != NULL, LANG_ERR_EXECUTER_CALCULATION_INVALID, act);
    Executer_setValueByToken(this, target, r, false);
    Object_release(value);
}

void Executer_consumeExpDouble(Executer *this, Leaf *leaf)
{
    Cursor *cursor = Stack_reset(leaf->tokens);
    Token *second = Stack_next(leaf->tokens, cursor);
    Token *action = Stack_next(leaf->tokens, cursor);
    Token *first = Stack_next(leaf->tokens, cursor);
    Token *target = Stack_next(leaf->tokens, cursor);
    Cursor_free(cursor);
    Value *secondV = Executer_getValueByToken(this, second, true);
    Value *firstV = Executer_getValueByToken(this, first, true);
    char firstType = firstV->type;
    char secondType = secondV->type;
    Value *r = NULL;
    char *act = action->value;
    //
    // TODO add type change to standard libray and forbid expression between different types
    if (is_equal(act, TVALUE_EQUAL) && firstType != secondType)
    {
        r = Value_newBoolean(false, NULL);
    }
    else if (firstType == secondType)
    {
        Executer_error(this, action, LANG_ERR_EXECUTER_EXP_INVALID_TYPE);
    }
    else if (is_values(act, TVALUE_GROUP_EXP_STRING) && firstType == UG_RTYPE_STR)
    {
        char *firstS = Value_toString(firstV);
        char *secondS = Value_toString(secondV);
        if (is_equal(act, TVALUE_EQUAL))
        {
            bool boolean = is_equal(firstS, secondS);
            r = Value_newBoolean(boolean, NULL);
        }
        else if (is_equal(act, TVALUE_CONCAT))
        {
            r = Value_newString(String_format("%s%s", firstS, secondS), NULL);
        }
        pct_free(firstS);
        pct_free(secondS);
    }
    else if (is_values(act, TVALUE_GROUP_EXP_NUMBER) && firstType == UG_RTYPE_NUM)
    {
        Value *vFirst = Value_toNumber(firstV);
        Value *vSecond = Value_toNumber(secondV);
        double firstN = vFirst->number;
        double secondN = vSecond->number;
        Object_release(vFirst);
        Object_release(vSecond);
        if (is_equal(act, TVALUE_EQUAL))
        {
            r = Value_newBoolean(firstN == secondN, NULL);
        }
        else if (is_equal(act, TVALUE_LESS))
        {
            r = Value_newBoolean(firstN < secondN, NULL);
        }
        else if (is_equal(act, TVALUE_MORE))
        {
            r = Value_newBoolean(firstN > secondN, NULL);
        }
        else if (is_equal(act, TVALUE_ADD))
        {
            r = Value_newNumber(firstN + secondN, NULL);
        }
        else if (is_equal(act, TVALUE_SUB))
        {
            r = Value_newNumber(firstN - secondN, NULL);
        }
        else if (is_equal(act, TVALUE_MUL))
        {
            r = Value_newNumber(firstN * secondN, NULL);
        }
        else if (is_equal(act, TVALUE_DIV))
        {
            Executer_assert(this, secondN != 0, second, LANG_ERR_EXECUTER_INVALID_DEVICE);
            r = Value_newNumber(firstN / secondN, NULL);
        }
    }
    else if (is_values(act, TVALUE_GROUP_EXP_BOOL) && firstType == UG_RTYPE_BOL)
    {
        Value *vFirst = Value_toBoolean(firstV);
        Value *vSecond = Value_toBoolean(secondV);
        bool firstB = vFirst->boolean;
        bool secondB = vSecond->boolean;
        Object_release(vFirst);
        Object_release(vSecond);
        if (is_equal(act, TVALUE_EQUAL))
        {
            r = Value_newBoolean(firstB == secondB, NULL);
        }
        else if (is_equal(act, TVALUE_AND))
        {
            r = Value_newBoolean(firstB == true && secondB == true, NULL);
        }
        else if (is_equal(act, TVALUE_OR))
        {
            r = Value_newBoolean(firstB == true || secondB == true, NULL);
        }
    }
    else if (is_values(act, TVALUE_GROUP_EXP_CONTAINER) && firstType == UG_RTYPE_CNT)
    {
        char *firstS = Value_toString(firstV);
        char *secondS = Value_toString(secondV);
        if (is_equal(act, TVALUE_EQUAL))
        {
            bool boolean = is_equal(firstS, secondS);
            r = Value_newBoolean(boolean, NULL);
        }
        pct_free(firstS);
        pct_free(secondS);
    }
    Object_release(firstV);
    Object_release(secondV);
    tools_assert(r != NULL, LANG_ERR_EXECUTER_CALCULATION_INVALID, act);
    Executer_setValueByToken(this, target, r, false);
}

bool Executer_checkJudge(Executer *this, Token *left, Token *right, Token *judge)
{
    bool value = false;
    Value *leftV = Executer_getValueByToken(this, left, true);
    Value *rightV = Executer_getValueByToken(this, right, true);
    char *judgeValue = judge->value;
    char leftType = leftV->type;
    char rightType = rightV->type;
    bool shouldYes = is_equal(judgeValue, TVALUE_IF_OK);
    // different type
    if (leftType != rightType) {
        value = !shouldYes;
    } else if (leftType == UG_RTYPE_STR && rightType == UG_RTYPE_STR) {
        char *leftS = Value_toString(leftV);
        char *rightS = Value_toString(rightV);
        value = shouldYes == is_equal(leftS, rightS);
        pct_free(leftS);
        pct_free(rightS);
    } else if (leftType == UG_RTYPE_NUM && rightType == UG_RTYPE_NUM) {
        Value *vFirst = Value_toNumber(leftV);
        Value *vSecond = Value_toNumber(rightV);
        double leftN = vFirst->number;
        double rightN = vSecond->number;
        Object_release(vFirst);
        Object_release(vSecond);
        value = shouldYes == (leftN == rightN);
    } else if (leftType == UG_RTYPE_BOL && rightType == UG_RTYPE_BOL) {
        Value *vFirst = Value_toBoolean(leftV);
        Value *vSecond = Value_toBoolean(rightV);
        bool leftB = vFirst->boolean;
        bool rightB = vSecond->boolean;
        Object_release(vFirst);
        Object_release(vSecond);
        value = shouldYes == (leftB == rightB);
    } else if (leftType == UG_RTYPE_NIL && rightType == UG_RTYPE_NIL) {
        value = true;
    } else if (leftType == UG_RTYPE_CNT && rightType == UG_RTYPE_CNT) {
        char *leftS = Value_toString(leftV);
        char *rightS = Value_toString(rightV);
        value = shouldYes == is_equal(leftS, rightS); // check pointer id
        pct_free(leftS);
        pct_free(rightS);
    } else {
        value = false;
    }
    Object_release(leftV);
    Object_release(rightV);
    return value;
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
    Cursor *cursor2 = Stack_reset(ifNode->tokens);
    Token *judge = Stack_next(ifNode->tokens, cursor2);
    Token *right = Stack_next(ifNode->tokens, cursor2);
    Token *left = Stack_next(ifNode->tokens, cursor2);
    Cursor_free(cursor2);
    if (!isFinish && Executer_checkJudge(this, left, right, judge))
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
        Cursor *cursor2 = Stack_reset(ifNode->tokens);
        Token *judge = Stack_next(ifNode->tokens, cursor2);
        Token *right = Stack_next(ifNode->tokens, cursor2);
        Token *left = Stack_next(ifNode->tokens, cursor2);
        Cursor_free(cursor2);
        if (!isFinish && Executer_checkJudge(this, left, right, judge))
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
        Token *judge = Stack_next(ifNode->tokens, cursor2);
        Cursor_free(cursor2);
        tools_assert(is_equal(judge->value, TVALUE_IF_NO), LANG_ERR_EXECUTER_INVALID_IF);
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
    Cursor *cursor = Stack_reset(leaf->tokens);
    Token *judge = Stack_next(leaf->tokens, cursor);
    Token *right = Stack_next(leaf->tokens, cursor);
    Token *left = Stack_next(leaf->tokens, cursor);
    Cursor_free(cursor);
    while (Executer_checkJudge(this, left, right, judge))
    {
        Executer_pushContainer(this, UG_CTYPE_SCP);
        Executer_consumeTree(this, leaf);
        Executer_popContainer(this, UG_CTYPE_SCP);
    }
}

void Executer_consumeFunction(Executer *this, Leaf *leaf)
{
    Cursor *cursor1 = Stack_reset(leaf->tokens);
    Token *function = Stack_next(leaf->tokens, cursor1);
    Cursor_free(cursor1);
    Cursor *cursor2 = Queue_reset(leaf->leafs);
    Leaf *code = Queue_next(leaf->leafs, cursor2);
    Cursor_free(cursor2);
    Executer_setValueByToken(this, function, Value_newFunction(code, NULL), true);
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
        char *key = Executer_getKeyByToken(this, arg);
        Container_set(this->currentContainer, key, value);
        arg = Stack_next(leaf->tokens, cursor2);
        pct_free(key);
    }
    Cursor_free(cursor1);
    Cursor_free(cursor2);
    //
    Stack_clear(this->callStack);
    Executer_consumeTree(this, leaf);
}

Value *Executer_runFunc(Executer *this, Value *funcValue)
{
    Leaf *codeNode = funcValue->object;
    Executer_pushContainer(this, UG_CTYPE_SCP);
    Executer_consumeLeaf(this, codeNode);
    Executer_popContainer(this, UG_CTYPE_SCP);
    this->isReturn = false;
    Value *r = Stack_pop(this->callStack);
    if (r == NULL) r = Value_newEmpty(NULL);
    return r;
}

Value *Executer_runNative(Executer *this, Value *funcValue)
{
    Bridge *bridge = this->uyghur->bridge;
    Bridge_startArgument(bridge);
    Stack_reverse(this->callStack);
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
    Bridge_run(bridge, funcValue);
    //
    tools_assert(bridge->type == BRIDGE_STACK_TP_RES, LANG_ERR_EXECUTER_BRIDGE_INVALID_RESULT);
    Value *r = Bridge_popValue(bridge);
    if (r == NULL) r = Value_newEmpty(NULL);
    return r;
}

void Executer_consumeCall(Executer *this, Leaf *leaf)
{
    Stack_clear(this->callStack);
    Cursor *cursor = Stack_reset(leaf->tokens);
    // get func name and result name
    Token *funcName = Stack_next(leaf->tokens, cursor);
    Token *resultName = Stack_next(leaf->tokens, cursor);
    // push args to use
    Token *arg = Stack_next(leaf->tokens, cursor);
    while(arg != NULL)
    {
        Value *value = Executer_getValueByToken(this, arg, true);
        Stack_push(this->callStack, value);
        Object_release(value);
        arg = Stack_next(leaf->tokens, cursor);
    }
    Cursor_free(cursor);
    // get func
    Value *r = NULL;
    Value *funcValue = Executer_getValueByToken(this, funcName, true);
    Debug_pushTrace(this->uyghur->debug, funcName);
    if (funcValue->type == UG_RTYPE_FUN) {
        r = Executer_runFunc(this, funcValue);
    } else if (funcValue->type == UG_RTYPE_NTV) {
        r = Executer_runNative(this, funcValue);
    } else {
        tools_error(LANG_ERR_EXECUTER_FUNCTION_NOT_FOUND, funcName->value);
    }
    Debug_popTrace(this->uyghur->debug, NULL);
    if (!is_equal(resultName->type, UG_TTYPE_EMP) && !is_equal(resultName->value, TVALUE_EMPTY)) {
        Executer_setValueByToken(this, resultName, r, true);
    } else {
        Object_release(r);
    }
    //
    Object_release(funcValue);
    Stack_clear(this->callStack);
}

void Executer_consumeResult(Executer *this, Leaf *leaf)
{
    Cursor *cursor = Stack_reset(leaf->tokens);
    Token *result = Stack_next(leaf->tokens, cursor);
    Cursor_free(cursor);
    Value *value = Executer_getValueByToken(this, result, true);
    Stack_clear(this->callStack);
    Stack_push(this->callStack, value);
    this->isReturn = true;
}

double Executer_calculateNumbers(Executer *this, double left, char *sign, double right, Token *token)
{
    if (is_equal(sign, TVALUE_SIGN_ADD)) return left + right;
    if (is_equal(sign, TVALUE_SIGN_SUB)) return left - right;
    if (is_equal(sign, TVALUE_SIGN_POW)) return pow(left, right);
    if (is_equal(sign, TVALUE_SIGN_PER)) return fmod(left, right);
    if (is_equal(sign, TVALUE_SIGN_MUL)) return left * right;
    if (is_equal(sign, TVALUE_SIGN_DIV)) {
        Executer_assert(this, right != 0, token, LANG_ERR_EXECUTER_INVALID_DEVICE);
        return left / right;
    }
    int lInt = (int)left;
    int rInt = (int)right;
    bool isInt = lInt == left && rInt == right;
    Executer_assert(this, isInt, token, LANG_ERR_CANNOT_BE_FLOAT);
    if (is_equal(sign, TVALUE_SIGN_NOT)) return lInt ^ rInt;
    if (is_equal(sign, TVALUE_SIGN_AND)) return lInt & rInt;
    if (is_equal(sign, TVALUE_SIGN_ORR)) return lInt | rInt;
    Executer_error(this, token, LANG_ERR_EXECUTER_CALCULATION_INVALID_SIGN);
    return 0;
}

bool Executer_calculateBooleans(Executer *this, bool left, char *sign, bool right, Token *token)
{
    if (is_equal(sign, TVALUE_SIGN_NOT)) return left != right;
    if (is_equal(sign, TVALUE_SIGN_AND)) return left && right;
    if (is_equal(sign, TVALUE_SIGN_ORR)) return left || right;
    Executer_error(this, token, LANG_ERR_EXECUTER_CALCULATION_INVALID_SIGN);
    return NULL;
}

String* Executer_calculateStrings(Executer *this, String *left, char *sign, String *right, Token *token)
{
    if (is_equal(sign, TVALUE_SIGN_LNK)) {
        return String_format("%s%s", left->data, right->data);
    }
    Executer_error(this, token, LANG_ERR_EXECUTER_CALCULATION_INVALID_SIGN);
    return NULL;
}

Value *Executer_calculateFoliage(Executer *this, Value *left, char *sign, Value *right, Token *token)
{
    Value *result = NULL;
    char lType = left->type;
    char rType = right->type;
    int compCode = Value_compareTo(left, right);
    int sameType = compCode != CODE_FAIL;
    if (is_values(sign, TVAUE_GROUP_CALCULATION_ALL)) {
        if (is_equal(sign, TVALUE_SIGN_EQUAL)) {
            bool r = sameType && compCode == CODE_NONE;
            result = Value_newBoolean(r, token);
        } else if (sameType && is_equal(sign, TVALUE_SIGN_MORE)) {
            bool r = compCode == CODE_TRUE;
            result = Value_newBoolean(r, token);
        } else if (sameType && is_equal(sign, TVALUE_SIGN_LESS)) {
            bool r = compCode == CODE_FALSE;
            result = Value_newBoolean(r, token);
        }
    } else if (sameType) {
        if (is_values(sign, TVAUE_GROUP_CALCULATION_NUM) && lType == UG_RTYPE_NUM) {
            double r = Executer_calculateNumbers(this, left->number, sign, right->number, token);
            result = Value_newNumber(r, token);
        } else if (is_values(sign, TVAUE_GROUP_CALCULATION_BOL) && lType == UG_RTYPE_NUM) {
            double r = Executer_calculateNumbers(this, left->number, sign, right->number, token);
            result = Value_newNumber(r, token);
        } else if (is_values(sign, TVAUE_GROUP_CALCULATION_BOL) && lType == UG_RTYPE_BOL) {
            bool r = Executer_calculateBooleans(this, left->boolean, sign, right->boolean, token);
            result = Value_newBoolean(r, token);
        } else if (is_values(sign, TVAUE_GROUP_CALCULATION_STR) && lType == UG_RTYPE_STR) {
            String *r = Executer_calculateStrings(this, left->string, sign, right->string, token);
            result = Value_newString(r, token);
        }
    } else {
        bool bLeftStr = lType == UG_RTYPE_STR;
        bool bRightStr = rType == UG_RTYPE_STR;
        bool bLeftNum = lType == UG_RTYPE_NUM;
        bool bRightNum = rType == UG_RTYPE_NUM;
        bool hasStr = bLeftStr || bRightStr;
        bool hasNum = bLeftNum || bRightNum;
        if (hasStr && hasNum && is_equal(sign, TVALUE_SIGN_RPT)) {
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
    // TODO: momory leak
    // Value_free(left);
    // Value_free(right);
    Executer_assert(this, result != NULL, token, LANG_ERR_EXECUTER_CALCULATION_INVALID_SIGN);
    return result;
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
        result = Executer_calculateFoliage(this, leftR, token->value, rightR, token);
        Object_release(leftR);
        Object_release(rightR);
    } else if (foliage->left != NULL) {
        result = Executer_calculateBTree(this, foliage->left);
    } else if (foliage->right != NULL) {
        result = Executer_calculateBTree(this, foliage->right);
    } else {
        Executer_assert(this, is_values(token->type, TTYPES_GROUP_VALUES), token, LANG_ERR_EXECUTER_CALCULATION_INVALID_ARGS);
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
    // helper_print_btree(root, " ");
    Value *r = Executer_calculateBTree(this, root);
    //
    Executer_assert(this, r != NULL, target, LANG_ERR_EXECUTER_CALCULATION_INVALID_ARGS);
    Executer_setValueByToken(this, target, r, false);
}

void Executer_consumeLeaf(Executer *this, Leaf *leaf)
{
    char tp = leaf->type;
    // variable
    if (tp == UG_ATYPE_VAR)
    {
        Executer_consumeVariable(this, leaf);
        return;
    }
    // operate
    if (tp == UG_ATYPE_OPRT)
    {
        Executer_consumeOperate(this, leaf);
        return;
    }
    // expression single
    if (tp == UG_ATYPE_EXP_S)
    {
        Executer_consumeExpSingle(this, leaf);
        return;
    }
    // expression double
    if (tp == UG_ATYPE_EXP_D)
    {
        Executer_consumeExpDouble(this, leaf);
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
    // function
    if(tp == UG_ATYPE_FUN)
    {
        Executer_consumeFunction(this, leaf);
        return;
    }
    // call
    if(tp == UG_ATYPE_CALL)
    {
        Executer_consumeCall(this, leaf);
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
    printf("--->\n");
    helper_print_leaf(leaf, " ");
    printf("--->\n");
    //
    tools_error(LANG_ERR_EXECUTER_NOT_IMPLEMENTED, tp);
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
    Container_set(this->globalContainer, path, module);
    return module;
}

void Executer_free(Executer *this)
{
    Executer_reset(this);
    free(this);
}
