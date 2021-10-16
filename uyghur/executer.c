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

void Executer_error(Executer *this, char *msg)
{
    Token *token = NULL;
    char *m = msg != NULL ? msg : LANG_ERR_EXECUTER_EXCEPTION;
    char *s = tools_format(LANG_ERR_TOKEN_PLACE, token->value, token->line, token->column, token->file);
    tools_error("%s%s", m, s);
}

void Executer_assert(Executer *this, bool value, char *msg)
{
    if (value == true) return;
    Executer_error(this, msg);
}

void Execer_consumeVariable(Executer *this, Leaf *leaf)
{
    Token *value = Stack_pop(leaf->tokens);
    Token *key = Stack_pop(leaf->tokens);
    Hashmap_set(this->currentScope, key->value, value->value);
}

void Execer_consumeOperate(Executer *this, Leaf *leaf)
{
    Token *action = Stack_pop(leaf->tokens);
    Token *name = Stack_pop(leaf->tokens);
    Token *target = Stack_pop(leaf->tokens);
    if (is_equal(target->value, TVALUE_TARGET_TO) && is_equal(action->value, TVALUE_OUTPUT))
    {
        char *value = Hashmap_get(this->currentScope, name->value);
        printf("%s", value);
    }
    else if (is_equal(target->value, TVALUE_TARGET_FROM) && is_equal(action->value, TVALUE_INPUT))
    {
        char line[1024];
        scanf("%[^\n]", line);
        Hashmap_set(this->currentScope, name->value, line);
    }
}

void Executer_consumeLeaf(Executer *this, Leaf *leaf)
{
    //
    char *tp = leaf->type;
    // variable
    if (is_equal(tp, ASTTYPE_VARIABLE))
    {
        Execer_consumeVariable(this, leaf);
        return;
    }
    // operate
    if (is_equal(tp, ASTTYPE_OPERATE))
    {
        Execer_consumeOperate(this, leaf);
        return;
    }
    //
    //
    printf("--->\n");
    helper_print_leaf(leaf, " ");
    printf("--->\n");
    //
    Executer_error(this, NULL);
    helper_print_leaf(leaf, " ");
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
