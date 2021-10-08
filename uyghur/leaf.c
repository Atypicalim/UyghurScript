// leaf

#ifndef H_LEAF
#define H_LEAF

#include "header.h"
#include "token.c"

typedef struct LeafNode {
    struct _Block;
    char *type; // ast type
    Stack *tokens; // tokens of this ast leaf (params of expression, statement or func)
    Queue *leafs; // programs of this ast leaf (sub code block of statement or func)
} Leaf;

Leaf *Leaf_new(char *type)
{
    Leaf *leaf = malloc(sizeof(Leaf));
    leaf->type = type;
    leaf->tokens = Stack_new();
    leaf->leafs = Queue_new();
    return leaf;
}

void Leaf_print(Leaf *this)
{
    printf("[(LEAF_START) => address:%d type:%s]\n", this, this->type);
    Stack_print(this->tokens); // TODO: while stack->next
    Queue_print(this->leafs); // while queue->next
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
    free(this);
}

#endif
