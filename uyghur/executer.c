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
    this->globalContainer = Container_newScope();;
}

void Executer_pushContainer(Executer *this, bool isBox)
{
    Container *container = isBox ? Container_newBox() : Container_newScope();
    Stack_push(this->containerStack, container);
    this->currentContainer = (Container *)this->containerStack->tail->data;
    this->rootContainer = (Container *)this->containerStack->head->data;
}

Container *Executer_popContainer(Executer *this, bool isBox)
{
    Container *container = Stack_pop(this->containerStack);
    this->currentContainer = (Container *)this->containerStack->tail->data;
    tools_assert(container != NULL && container->isBox == isBox, LANG_ERR_NO_VALID_STATE);
    return container;
}

Executer *Executer_new(Uyghur *uyghur)
{
    Executer *executer = malloc(sizeof(Executer));
    Executer_reset(executer);
    Executer_pushContainer(executer, false);
    executer->uyghur = uyghur;
    return executer;
}

void Executer_error(Executer *this, Token *token, char *msg)
{
    char *m = msg != NULL ? msg : LANG_ERR_EXECUTER_EXCEPTION;
    char *s = tools_format(LANG_ERR_TOKEN_PLACE, token->value, token->line, token->column, token->file);
    tools_error("%s%s", m, s);
}

void Executer_assert(Executer *this, bool value, Token *token, char *msg)
{
    if (value == true) return;
    Executer_error(this, token, msg);
}

// get token runtime value
Value *Executer_getTRValue(Executer *this, Token *token)
{
    // TODO: put const values like empty or number to globalContainer
    if (is_equal(token->type, TTYPE_EMPTY))
    {
        return Value_newEmpty(token);
    }
    else if (is_equal(token->type, TTYPE_BOX))
    {
        return Value_newBox(token);
    }
    else if (is_equal(token->type, TTYPE_BOOL))
    {
        bool boolean = is_equal(token->value, TVALUE_TRUE) ? true : false;
        return Value_newBoolean(boolean, token);
    }
    else if (is_equal(token->type, TTYPE_NUMBER))
    {
        double val = atof(token->value);
        return Value_newNumber(val, token);
    }
    else if (is_equal(token->type, TTYPE_STRING))
    {
        return Value_newString(token->value, token);
    }
    else if (is_equal(token->type, TTYPE_NAME))
    {
        Block *block = this->containerStack->tail;
        Value *value = NULL;
        while (value == NULL && block != NULL)
        {
            Value *v = Container_get(block->data, token->value);
            if (v != NULL)
            {
                value = v;
                break;
            }
            else
            {
                block = block->last;
            }
        }
        if (value == NULL)
        {
            value = Value_newEmpty(NULL);
        }
        return value;
    }
    else
    {
        return Value_newObject(token->value, token);
    }
    // TODO
}

void Executer_setTRValue(Executer *this, Token *key, Value *value)
{
    Block *block = this->containerStack->tail;
    while (block != NULL)
    {
        if (Container_get(block->data, key->value) != NULL)
        {
            break;
        }
        else
        {
            block = block->last;
        }
    }
    if (block == NULL)
    {
        block = this->containerStack->tail;
    }
    Container_set(block->data, key->value, value);
}

void Executer_consumeVariable(Executer *this, Leaf *leaf)
{
    Stack_reset(leaf->tokens);
    Token *value = Stack_next(leaf->tokens);
    Token *key = Stack_next(leaf->tokens);
    Value *v = Executer_getTRValue(this, value);
    Container_set(this->currentContainer, key->value, v);
}

void Executer_consumeOperate(Executer *this, Leaf *leaf)
{
    Stack_reset(leaf->tokens);
    Token *action = Stack_next(leaf->tokens);
    Token *name = Stack_next(leaf->tokens);
    Token *target = Stack_next(leaf->tokens);
    if (is_equal(target->value, TVALUE_TARGET_TO) && is_equal(action->value, TVALUE_OUTPUT))
    {
        Value *value = Executer_getTRValue(this, name);
        printf("%s", Value_toString(value));
    }
    else if (is_equal(target->value, TVALUE_TARGET_FROM) && is_equal(action->value, TVALUE_INPUT))
    {
        char line[1024];
        scanf(" %[^\n]", line);
        char *l = tools_format("%s", line);
        Executer_setTRValue(this, name, Value_newString(l, NULL));
    }
}

void Executer_consumeExpSingle(Executer *this, Leaf *leaf)
{
    Stack_reset(leaf->tokens);
    Token *action = Stack_next(leaf->tokens);
    Token *target = Stack_next(leaf->tokens);
    Value *value = Executer_getTRValue(this, target);
    Value *r = NULL;
    char *act = action->value;
    //
    if (is_values(action->type, 1, TTYPE_WORD))
    {
        if (is_equal(act, TVALUE_EMPTY))
        {
            // TODO free object
            r = Value_newEmpty(NULL);
        }
        else if (is_equal(act, TVALUE_NOT))
        {
            if (is_equal(value->type, RTYPE_NUMBER))
            {
                r = Value_newBoolean(value->number <= 0, NULL);
            }
            else if (is_equal(value->type, RTYPE_STRING))
            {
                r = Value_newBoolean(!is_equal(value->string, TVALUE_TRUE), NULL);
            }
            else if (is_equal(value->type, RTYPE_EMPTY))
            {
                r = Value_newBoolean(true, NULL);
            }
            else if (is_equal(value->type, RTYPE_BOOLEAN))
            {
                r = Value_newBoolean(!value->boolean, NULL);
            }
            else if (is_equal(value->type, RTYPE_FUNCTION))
            {
                r = Value_newNumber(true, NULL);
            }
        }
        else if (is_equal(act, TVALUE_NUM))
        {
            r = Value_toNumber(value);
        }
        else if (is_equal(act, TVALUE_STR))
        {
            char *s = Value_toString(value);
            r = Value_newString(s, NULL);
        }
        else if (is_equal(act, TVALUE_BOOLEAN))
        {
            r = Value_toBoolean(value);
        }
        else if (is_equal(act, TVALUE_FUNCTION))
        {
            char *funcName = value->string;
            Token *funcToken = Token_name(funcName);
            Value *funcValue = Executer_getTRValue(this, funcToken);
            if(is_equal(funcValue->type, RTYPE_FUNCTION) || is_equal(funcValue->type, RTYPE_CFUNCTION))
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
        r = Executer_getTRValue(this, action);
    }
    tools_assert(r != NULL, "not supported action for expression:%s", act);
    Executer_setTRValue(this, target, r);
}

void Executer_consumeExpDouble(Executer *this, Leaf *leaf)
{
    Stack_reset(leaf->tokens);
    Token *second = Stack_next(leaf->tokens);
    Token *action = Stack_next(leaf->tokens);
    Token *first = Stack_next(leaf->tokens);
    Token *target = Stack_next(leaf->tokens);
    Value *secondV = Executer_getTRValue(this, second);
    Value *firstV = Executer_getTRValue(this, first);
    char *firstType = firstV->type;
    char *secondType = secondV->type;
    Value *r = NULL;
    char *act = action->value;
    //
    // TODO add type change to standard libray and forbid expression between different types
    if (is_equal(act, TVALUE_EQUAL) && !is_equal(firstType, secondType))
    {
        r = Value_newBoolean(false, NULL);
    }
    else if (!is_equal(firstType, secondType))
    {
        Executer_error(this, action, LANG_ERR_EXECUTER_EXP_INVALID_TYPE);
    }
    else if (is_values(act, TVALUE_GROUP_EXP_STRING) && is_equal(firstType, RTYPE_STRING))
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
            char *ch = str_concat(firstS, secondS);
            r = Value_newString(ch, NULL);
        }
    }
    else if (is_values(act, TVALUE_GROUP_EXP_NUMBER) && is_equal(firstType, RTYPE_NUMBER))
    {
        double firstN = Value_toNumber(firstV)->number;
        double secondN = Value_toNumber(secondV)->number;
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
        else if (is_equal(act, TVALUE_MUL))
        {
            r = Value_newNumber(firstN / secondN, NULL);
        }
    }
    else if (is_values(act, TVALUE_GROUP_EXP_BOOL) && is_equal(firstType, RTYPE_BOOLEAN))
    {
        bool firstB = Value_toBoolean(firstV)->boolean;
        bool secondB = Value_toBoolean(secondV)->boolean;
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
    else if (is_values(act, TVALUE_GROUP_EXP_BOX) && is_equal(firstType, RTYPE_BOX))
    {
        char *firstS = Value_toString(firstV);
        char *secondS = Value_toString(secondV);
        if (is_equal(act, TVALUE_EQUAL))
        {
            bool boolean = is_equal(firstS, secondS);
            r = Value_newBoolean(boolean, NULL);
        }
    }
    tools_assert(r != NULL, "not supported action for expression:%s", act);
    Executer_setTRValue(this, target, r);
}

bool Executer_checkJudge(Executer *this, Token *left, Token *right, Token *judge)
{
    Value *leftV = Executer_getTRValue(this, left);
    Value *rightV = Executer_getTRValue(this, right);
    char *judgeValue = judge->value;
    char *leftType = leftV->type;
    char *rightType = rightV->type;
    bool shouldYes = is_equal(judgeValue, TVALUE_IF_OK);
    // different type
    if (!is_equal(leftType, rightType))
    {
        return !shouldYes;
    }
    else if (is_equal(leftType, RTYPE_STRING) && is_equal(rightType, RTYPE_STRING))
    {
        char *leftS = Value_toString(leftV);
        char *rightS = Value_toString(rightV);
        return shouldYes == is_equal(leftS, rightS);
    }
    else if (is_equal(leftType, RTYPE_NUMBER) && is_equal(rightType, RTYPE_NUMBER))
    {
        double leftN = Value_toNumber(leftV)->number;
        double rightN = Value_toNumber(rightV)->number;
        return shouldYes == (leftN == rightN);
    }
    else if (is_equal(leftType, RTYPE_BOOLEAN) && is_equal(rightType, RTYPE_BOOLEAN))
    {
        bool leftB = Value_toBoolean(leftV)->boolean;
        bool rightB = Value_toBoolean(rightV)->boolean;
        return shouldYes == (leftB == rightB);
    }
    else if (is_equal(leftType, RTYPE_EMPTY) && is_equal(rightType, RTYPE_EMPTY))
    {
        return true;
    }
    else if (is_equal(leftType, RTYPE_BOX) && is_equal(rightType, RTYPE_BOX))
    {
        char *leftS = Value_toString(leftV);
        char *rightS = Value_toString(rightV);
        return shouldYes == is_equal(leftS, rightS); // check pointer id
    }
    else
    {
        return false;
    }
}

void Executer_consumeIf(Executer *this, Leaf *leaf)
{
    //
    bool isFinish = false;
    Queue_reset(leaf->leafs);
    // if
    Leaf *ifNode = Queue_next(leaf->leafs);
    tools_assert(ifNode != NULL, "invalid if");
    tools_assert(is_equal(ifNode->type, ASTTYPE_IF_FIRST), "invalid if");
    Stack_reset(ifNode->tokens);
    Token *judge = Stack_next(ifNode->tokens);
    Token *right = Stack_next(ifNode->tokens);
    Token *left = Stack_next(ifNode->tokens);
    if (!isFinish && Executer_checkJudge(this, left, right, judge))
    {
        Executer_pushContainer(this, false);
        Executer_consumeTree(this, ifNode);
        Executer_popContainer(this, false);
        isFinish = true;
    }
    // elseif
    ifNode = Queue_next(leaf->leafs);
    while (is_equal(ifNode->type, ASTTYPE_IF_MIDDLE))
    {
        Stack_reset(ifNode->tokens);
        Token *judge = Stack_next(ifNode->tokens);
        Token *right = Stack_next(ifNode->tokens);
        Token *left = Stack_next(ifNode->tokens);
        if (!isFinish && Executer_checkJudge(this, left, right, judge))
        {
            Executer_pushContainer(this, false);
            Executer_consumeTree(this, ifNode);
            Executer_popContainer(this, false);
            isFinish = true;
        }
        ifNode = Queue_next(leaf->leafs);
        tools_assert(ifNode != NULL, "invalid if");
    }
    // else
    if (is_equal(ifNode->type, ASTTYPE_IF_LAST))
    {
        Stack_reset(ifNode->tokens);
        Token *judge = Stack_next(ifNode->tokens);
        tools_assert(is_equal(judge->value, TVALUE_IF_NO), "invalid if");
        if (!isFinish)
        {
            Executer_pushContainer(this, false);
            Executer_consumeTree(this, ifNode);
            Executer_popContainer(this, false);
            isFinish = true;
        }
        ifNode = Queue_next(leaf->leafs);
        tools_assert(ifNode != NULL, "invalid if");
    }
    // end
    tools_assert(is_equal(ifNode->type, ASTTYPE_END), "invalid if");
    Leaf *nullValue = Queue_next(leaf->leafs);
    tools_assert(nullValue == NULL, "invalid if");
}

void Executer_consumeWhile(Executer *this, Leaf *leaf)
{
    Stack_reset(leaf->tokens);
    Token *judge = Stack_next(leaf->tokens);
    Token *right = Stack_next(leaf->tokens);
    Token *left = Stack_next(leaf->tokens);
    while (Executer_checkJudge(this, left, right, judge))
    {
        Executer_pushContainer(this, false);
        Executer_consumeTree(this, leaf);
        Executer_popContainer(this, false);
    }
}

void Executer_consumeFunction(Executer *this, Leaf *leaf)
{
    Stack_reset(leaf->tokens);
    Token *function = Stack_next(leaf->tokens);
    Queue_reset(leaf->leafs);
    Leaf *code = Queue_next(leaf->leafs);
    Executer_setTRValue(this, function, Value_newFunction(code, NULL));
}

void Executer_consumeCode(Executer *this, Leaf *leaf)
{
    Stack *callStack = Stack_reverse(this->callStack);
    Stack_reset(callStack);
    Stack_reset(leaf->tokens);
    // 
    Token *funcName = Stack_next(leaf->tokens);
    Token *arg = Stack_next(leaf->tokens);
    while(arg != NULL)
    {
        Value *value = Stack_next(callStack);
        Container_set(this->currentContainer, arg->value, value);
        arg = Stack_next(leaf->tokens);
    }
    //
    Stack_clear(this->callStack);
    Executer_consumeTree(this, leaf);
}

Value *Executer_runFunc(Executer *this, Token *funcName)
{
    Value *funcValue = Executer_getTRValue(this, funcName);
    tools_assert(is_equal(funcValue->type, RTYPE_FUNCTION), "function not valid for name: %s", funcName->value);
    // execute func
    Leaf *codeNode = funcValue->object;
    Executer_pushContainer(this, false);
    Executer_consumeLeaf(this, codeNode);
    Executer_popContainer(this, false);
    // return result
    Value *r = Stack_pop(this->callStack);
    if (r == NULL)
    {
        r = Value_newEmpty(NULL);
    }
    return r;
}

Value *Executer_runCFunc(Executer *this, Token *funcName)
{
    Value *funcValue = Executer_getTRValue(this, funcName);
    tools_assert(is_equal(funcValue->type, RTYPE_CFUNCTION), "cfunction not valid");
    void (*funcBody)(Bridge *) = funcValue->object;
    //
    Bridge *bridge = this->uyghur->bridge;
    Bridge_startArgument(bridge);
    Stack *callStack = Stack_reverse(this->callStack);
    Stack_reset(callStack);
    Value *value = Stack_next(callStack);
    while (value != NULL)
    {
        Bridge_pushValue(bridge, value);
        value = Stack_next(callStack);
    }
    Bridge_send(bridge);
    //
    funcBody(bridge);
    //
    tools_assert(bridge->type == BRIDGE_STACK_TP_RES, "invalid bridge status, func should return result at the end");
    Value *r = Bridge_popValue(bridge);
    if (r == NULL)
    {
        r = Value_newEmpty(NULL);
    }
    return r;
}

void Executer_consumeCall(Executer *this, Leaf *leaf)
{
    Stack_clear(this->callStack);
    Stack_reset(leaf->tokens);
    // get func name and result name
    Token *funcName = Stack_next(leaf->tokens);
    Token *resultName = Stack_next(leaf->tokens);
    // push args to use
    Token *arg = Stack_next(leaf->tokens);
    while(arg != NULL)
    {
        Value *value = Executer_getTRValue(this, arg);
        Stack_push(this->callStack, value);
        arg = Stack_next(leaf->tokens);
    }
    // get func
    Value *r = NULL;
    Value *funcValue = Executer_getTRValue(this, funcName);
    if (is_equal(funcValue->type, RTYPE_FUNCTION))
    {
        r = Executer_runFunc(this, funcName);
    }
    else if (is_equal(funcValue->type, RTYPE_CFUNCTION))
    {
        r = Executer_runCFunc(this, funcName);
    }
    else
    {
        tools_error("function not found for func name: %s", funcName->value);
    }
    //
    if (!is_equal(resultName->type, RTYPE_EMPTY))
    {
        Executer_setTRValue(this, resultName, r);
    }
    Stack_clear(this->callStack);
}

void Executer_consumeResult(Executer *this, Leaf *leaf)
{
    Stack_reset(leaf->tokens);
    Token *result = Stack_next(leaf->tokens);
    Value *value = Executer_getTRValue(this, result);
    Stack_clear(this->callStack);
    Stack_push(this->callStack, value);
}

void Executer_consumeLeaf(Executer *this, Leaf *leaf)
{
    char *tp = leaf->type;
    // variable
    if (is_equal(tp, ASTTYPE_VARIABLE))
    {
        Executer_consumeVariable(this, leaf);
        return;
    }
    // operate
    if (is_equal(tp, ASTTYPE_OPERATE))
    {
        Executer_consumeOperate(this, leaf);
        return;
    }
    // expression single
    if (is_equal(tp, ASTTYPE_EXPRESSION_SINGLE))
    {
        Executer_consumeExpSingle(this, leaf);
        return;
    }
    // expression double
    if (is_equal(tp, ASTTYPE_EXPRESSION_DOUBLE))
    {
        Executer_consumeExpDouble(this, leaf);
        return;
    }
    // if
    if (is_equal(tp, ASTTYPE_IF))
    {
        Executer_consumeIf(this, leaf);
        return;
    }
    // while
    if(is_equal(tp, ASTTYPE_WHILE))
    {
        Executer_consumeWhile(this, leaf);
        return;
    }
    // function
    if(is_equal(tp, ASTTYPE_FUNC))
    {
        Executer_consumeFunction(this, leaf);
        return;
    }
    // call
    if(is_equal(tp, ASTTYPE_CALL))
    {
        Executer_consumeCall(this, leaf);
        return;
    }
    // code
    if(is_equal(tp, ASTTYPE_CODE))
    {
        Executer_consumeCode(this, leaf);
        return;
    }
    // result
    if(is_equal(tp, ASTTYPE_RESULT))
    {
        Executer_consumeResult(this, leaf);
        return;
    }
    // end
    if(is_equal(tp, ASTTYPE_END))
    {
        return;
    }
    //
    printf("--->\n");
    helper_print_leaf(leaf, " ");
    printf("--->\n");
    //
    tools_error("EXECUTER NOT IMPLEMENTED FOR %s", tp);
    // helper_print_leaf(leaf, " ");
}

bool Executer_consumeTree(Executer *this, Leaf *tree)
{
    Queue_reset(tree->leafs);
    Leaf *leaf = Queue_next(tree->leafs);
    while (leaf != NULL)
    {
        Executer_consumeLeaf(this, leaf);
        leaf = Queue_next(tree->leafs);
    }
    return true;
}

bool Executer_executeTree(Executer *this, char *path, Leaf *tree)
{
    Executer_pushContainer(this, true);
    Executer_consumeTree(this, tree);
    Container *container = Executer_popContainer(this, true);
    Container_set(this->globalContainer, path, container);
    return true;
}

void Executer_free(Executer *this)
{
    Executer_reset(this);
    free(this);
}
