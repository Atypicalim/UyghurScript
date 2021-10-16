// executer

#include "others/header.h"

struct Executer {
    Uyghur *uyghur;
    Leaf *tree;
    Leaf *leaf;
};

void Executer_reset(Executer *this)
{
    this->uyghur = NULL;
    this->tree = NULL;
    this->leaf = NULL;
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

void Executer_consumeLeaf(Executer *this, Leaf *leaf)
{
    //
    //
    Executer_error(this, NULL);
    helper_print_leaf(leaf, " ");
}

bool Executer_executeTree(Executer *this, Leaf *headLeaf)
{
    // this->tree = headLeaf;
    // this->leaf = this->tree;
    // //
    // while (this->leaf != NULL)
    // {
    //     Executer_consumeLeaf(this, this->leaf);
    //     this->leaf = Queue_pop(this->leaf->leafs);
    // }

    printf("--->\n");
    helper_print_leaf(headLeaf, " ");
    printf("--->\n");
    return true;
}

void Executer_free(Executer *this)
{
    Executer_reset(this);
    free(this);
}
