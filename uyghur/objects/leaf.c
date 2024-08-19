// leaf

#ifndef H_UG_HEADER
#include "../others/header.h"
#endif

#ifndef H_UG_LEAF
#define H_UG_LEAF

#include "token.c"

Leaf *Leaf_new(char type)
{
    Leaf *leaf = malloc(sizeof(Leaf));
    Object_init(leaf, PCT_OBJ_LEAF);
    leaf->type = type;
    leaf->parent = NULL;
    leaf->tokens = Stack_new();
    leaf->leafs = Queue_new();
    return leaf;
}

void Leaf_print(Leaf *this)
{
    printf("[(LEAF_START) => address:%d type:%c]\n", this, this->type);
    Stack_print(this->tokens);
    Queue_print(this->leafs);
    printf("[(LEAF_END) => address:%d]\n", this);
}

void Leaf_pushToken(Leaf *this, Token *token)
{
    Stack_push(this->tokens, token);
}

Token *Leaf_popToken(Leaf *this)
{
    return Stack_pop(this->tokens);
}

void Leaf_pushLeaf(Leaf *this, Leaf *leaf)
{
    leaf->parent = this;
    Queue_push(this->leafs,leaf);
}

Leaf *Leaf_popLeaf(Leaf *this)
{
    return Queue_pop(this->leafs);
}

void Leaf_free(Leaf *this)
{
    Stack_free(this->tokens);
    Queue_free(this->leafs);
    Object_free(this);
}

#endif
