// leaf

#ifndef H_LEAF
#define H_LEAF

#include "header.h"
#include "token.c"

typedef struct LeafNode {
    struct _Block;
    char *type; // ast type
    void *tokens; // tokens of this ast leaf (params of expression, statement or func)
    void *leafs; // programs of this ast leaf (sub code block of statement or func)
} Leaf;

Leaf *Leaf_new(char *type)
{
    Leaf *leaf = malloc(sizeof(Leaf));
    leaf->type = type;
    return leaf;
}

void Leaf_print(Leaf *this)
{
    printf("[(LEAF) => type:%s, leafs:(%s), leafs:(%s)]\n", this->type, "leafs", "leafs");
}

void Leaf_pushToken(Leaf *this, Token *token)
{

}

Token *Leaf_popToken(Leaf *this)
{
    return NULL;
}

void Leaf_pushLeaf(Leaf *this, Leaf *leaf)
{
    
}

Leaf *Leaf_popLeaf(Leaf *this)
{
    return NULL;
}

void Leaf_free(Leaf *this)
{
    free(this);
}

#endif
