// executer

#include "others/header.h"

struct Executer {
    Uyghur *uyghur;
    Leaf *tree;
    Leaf *leaf;
    Hashmap *globalMap;
    Stack *callStack;
    Stack *scopeStack;
    Hashmap *currentScope;
};

bool Executer_executeTree(Executer *, Leaf *);

void Executer_reset(Executer *this)
{
    this->uyghur = NULL;
    this->tree = NULL;
    this->leaf = NULL;
    this->globalMap = Hashmap_new(NULL);
    this->callStack = Stack_new();
    this->scopeStack = Stack_new();
    this->currentScope = NULL;
}

void Executer_pushScope(Executer *this)
{
    Hashmap *scope = Hashmap_new(NULL);
    Stack_push(this->scopeStack, scope);
    this->currentScope = (Hashmap *)this->scopeStack->tail->data; 
}

void Executer_popScope(Executer *this)
{
    tools_assert(this->scopeStack->head->data != this->currentScope, "executer trying to exit root scope");
    Stack_pop(this->scopeStack);
    this->currentScope = (Hashmap *)this->scopeStack->tail->data; 
}

Executer *Executer_new(Uyghur *uyghur)
{
    Executer *executer = malloc(sizeof(Executer));
    Executer_reset(executer);
    Executer_pushScope(executer);
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
    // TODO: put const values like empty or number to globalScope
    if (is_equal(token->type, TTYPE_EMPTY))
    {
        return Value_newEmpty(token);
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
        Block *block = this->scopeStack->tail;
        Value *value = NULL;
        while (value == NULL && block != NULL)
        {
            Value *v = Hashmap_get(block->data, token->value);
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
}

void Executer_setTRValue(Executer *this, Token *key, Value *value)
{
    // TODO
    // set to target scope of the original value
    // set to global scope for a non-assigned value
    Hashmap_set(this->currentScope, key->value, value);
}

void Executer_consumeVariable(Executer *this, Leaf *leaf)
{
    Token *value = Stack_pop(leaf->tokens);
    Token *key = Stack_pop(leaf->tokens);
    Value *v = Executer_getTRValue(this, value);
    Hashmap_set(this->currentScope, key->value, v);
}

void Executer_consumeOperate(Executer *this, Leaf *leaf)
{
    Token *action = Stack_pop(leaf->tokens);
    Token *name = Stack_pop(leaf->tokens);
    Token *target = Stack_pop(leaf->tokens);
    if (is_equal(target->value, TVALUE_TARGET_TO) && is_equal(action->value, TVALUE_OUTPUT))
    {
        Value *value = Executer_getTRValue(this, name);
        // printf("\n\n--->\n");
        // Token_print(action);
        // Token_print(name);
        // Value_print(value);
        // printf("--->\n\n");
        printf("%s", Value_toString(value));
    }
    else if (is_equal(target->value, TVALUE_TARGET_FROM) && is_equal(action->value, TVALUE_INPUT))
    {
        char line[1024];
        scanf("%[^\n]", line);
        char *l = tools_format("%s", line);
        Executer_setTRValue(this, name, Value_newString(l, NULL));
    }
}

void Executer_consumeExpSingle(Executer *this, Leaf *leaf)
{
    Token *action = Stack_pop(leaf->tokens);
    Token *target = Stack_pop(leaf->tokens);
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
            // TODO return function
            r = Value_newObject(NULL, NULL);
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
    Token *second = Stack_pop(leaf->tokens);
    Token *action = Stack_pop(leaf->tokens);
    Token *first = Stack_pop(leaf->tokens);
    Token *target = Stack_pop(leaf->tokens);
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
        if (is_equal(act, RTYPE_STRING))
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
        float firstN = Value_toNumber(firstV)->number;
        float secondN = Value_toNumber(secondV)->number;
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
        float leftN = Value_toNumber(leftV)->number;
        float rightN = Value_toNumber(rightV)->number;
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
    else
    {
        return false;
    }
}

void Executer_consumeIf(Executer *this, Leaf *leaf)
{
    //
    bool isFinish = false;
    // if
    Leaf *ifNode = Queue_pop(leaf->leafs);
    tools_assert(ifNode != NULL, "invalid if");
    tools_assert(is_equal(ifNode->type, ASTTYPE_IF_FIRST), "invalid if");
    Token *judge = Leaf_popToken(ifNode);
    Token *right = Leaf_popToken(ifNode);
    Token *left = Leaf_popToken(ifNode);
    if (!isFinish && Executer_checkJudge(this, left, right, judge))
    {
        Executer_pushScope(this);
        Executer_executeTree(this, ifNode);
        Executer_popScope(this);
        isFinish = true;
    }
    // elseif
    ifNode = Queue_pop(leaf->leafs);
    while (is_equal(ifNode->type, ASTTYPE_IF_MIDDLE))
    {
        Token *judge = Leaf_popToken(ifNode);
        Token *right = Leaf_popToken(ifNode);
        Token *left = Leaf_popToken(ifNode);
        if (!isFinish && Executer_checkJudge(this, left, right, judge))
        {
            Executer_pushScope(this);
            Executer_executeTree(this, ifNode);
            Executer_popScope(this);
            isFinish = true;
        }
        ifNode = Queue_pop(leaf->leafs);
        tools_assert(ifNode != NULL, "invalid if");
    }
    // else
    if (is_equal(ifNode->type, ASTTYPE_IF_LAST))
    {
        Token *judge = Leaf_popToken(ifNode);
        tools_assert(is_equal(judge->value, TVALUE_IF_NO), "invalid if");
        if (!isFinish)
        {
            printf("\n\n\n\n");
            Executer_pushScope(this);
            Executer_executeTree(this, ifNode);
            Executer_popScope(this);
            printf("\n\n\n\n");
            isFinish = true;
        }
        ifNode = Queue_pop(leaf->leafs);
        tools_assert(ifNode != NULL, "invalid if");
    }
    // end
    tools_assert(is_equal(ifNode->type, ASTTYPE_END), "invalid if");
    Leaf *nullValue = Queue_pop(leaf->leafs);
    tools_assert(nullValue == NULL, "invalid if");
    // pop scope
    // Token *key = Stack_pop(leaf->tokens);
    // Value *v = Executer_getTRValue(this, value);
    // Hashmap_set(this->currentScope, key->value, v);
}


void Executer_consumeLeaf(Executer *this, Leaf *leaf)
{
    //
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
    //
    printf("--->\n");
    helper_print_leaf(leaf, " ");
    printf("--->\n");
    //
    tools_error("EXECUTER NOT IMPLEMENTED FOR %s", tp);
    // helper_print_leaf(leaf, " ");
}

bool Executer_executeTree(Executer *this, Leaf *tree)
{
    Leaf *leaf = Queue_pop(tree->leafs);
    while (leaf != NULL)
    {
        Executer_consumeLeaf(this, leaf);
        leaf = Queue_pop(tree->leafs);
    }
    return true;
}

void Executer_free(Executer *this)
{
    Executer_reset(this);
    free(this);
}
