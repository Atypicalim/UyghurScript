// leaf

#ifndef HEAD_HEADER
#include "header.h"
#endif

#ifndef HEAD_TOKEN
#include "token.c"
#endif

#define HEAD_LEAF

typedef struct LeafNode {
    char *type;
    void *tokens[10];
    void *LeafNode[10];
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
