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
        // TODO: loop scope stack
        Value *value = Hashmap_get(this->currentScope, token->value);
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
        printf("\n\n!! %s %s %s !!\n\n", firstType, act, secondType);
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
    //
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
    Hashmap *scope = Hashmap_new(NULL);
    Stack_push(this->scopeStack, scope);
    this->currentScope = scope;
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
