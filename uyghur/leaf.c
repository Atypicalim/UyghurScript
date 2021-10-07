// leaf

#ifndef H_LEAF
#define H_LEAF

#include "header.h"
#include "token.c"

typedef struct LeafNode {
    struct _Block;
    char *type; // ast type
    void *tokens; // stack: tokens of this ast leaf (params of expression, statement or func)
    void *leafs; // queue: programs of this ast leaf (sub code block of statement or func)
} Leaf;

Leaf *Leaf_new(char *type)
{
    Leaf *leaf = malloc(sizeof(Leaf));
    leaf->type = type;
    leaf->tokens = NULL;
    leaf->leafs = NULL;
    return leaf;
}

void Leaf_print(Leaf *this)
{
    printf("[(LEAF_START) => address:%d type:%s]\n", this, this->type);
    Token *token = this->tokens;
    while (token != NULL)
    {
        Token_print(token);
        token = token->next == this->tokens ? NULL : token->next;
    }
    Leaf *leaf = this->leafs;
    while (leaf != NULL)
    {
        Leaf_print(leaf);
        leaf = leaf->next == this->leafs ? NULL : leaf->next;
    }
    printf("[(LEAF_END) => address:%d]\n", this);
}

void Leaf_pushToken(Leaf *this, Token *token)
{
    if (this->tokens == NULL)
    {
        this->tokens = token;
        Block_link(token, token);
    }
    else
    {
        Block_append(Block_last(this->tokens), token);
        Block_link(token, this->tokens);
    }
}

Token *Leaf_popToken(Leaf *this)
{
    if (this->tokens == NULL)
    {
        return NULL;
    }
    else
    {
        void *last = Block_last(this->tokens);
        if (last == this->tokens)
        {
            this->tokens = NULL;
        }
        else
        {
            Block_remove(last);
        }
        return last;
    }
}

void Leaf_pushLeaf(Leaf *this, Leaf *leaf)
{
    if (this->leafs == NULL)
    {
        this->leafs = leaf;
        Block_link(leaf, leaf);
    }
    else
    {
        Block_append(Block_last(this->leafs), leaf);
        Block_link(leaf, this->leafs);
    }
}

Leaf *Leaf_popLeaf(Leaf *this)
{
    if (this->leafs == NULL)
    {
        return NULL;
    }
    else
    {
        void *first = this->leafs;
        if (first == Block_next(this->leafs))
        {
            this->tokens = NULL;
        }
        else
        {
            Block_remove(first);
        }
        return first;
    }
}

void Leaf_free(Leaf *this)
{
    Token *token = this->tokens;
    Token *tempToken;
    while (token != NULL)
    {
        tempToken = token;
        token = token->next == this->tokens ? NULL : token->next;
        Token_free(tempToken);
    }
    Leaf *leaf = this->leafs;
    Leaf *tempLeaf;
    while (leaf != NULL)
    {
        tempLeaf = leaf;
        leaf = leaf->next == this->leafs ? NULL : leaf->next;
        Leaf_free(tempLeaf);
    }
    free(this);
}

#endif
