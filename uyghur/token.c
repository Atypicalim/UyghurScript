// token

#ifndef HEAD_HEADER
#include "header.h"
#endif

#define HEAD_TOKEN

typedef struct TokenNode {
    const char *file;
    int line;
    int column;
    char *type;
    void *value;
    struct TokenNode *next;
    struct TokenNode *last;
} Token;

Token *Token_new(const char *file, int line, int column, char *type, void *value)
{
    Token *token = malloc(sizeof(Token));
    token->file = file;
    token->line = line;
    token->column = column;
    token->type = type;
    token->value = value;
    token->next = NULL;
    token->last = NULL;
    return token;
}

void Token_print(Token *this)
{
    printf("[(TOKEN) => type:%s, value:(%s) in (%d, %d %s)]\n", this->type, this->value,  this->line,  this->column,  this->file);
}

void Token_append(Token *this, Token *other)
{
    this->next = other;
    other->last = this;
}

void Token_prepend(Token *this, Token *other)
{
    this->last = other;
    other->next = this;
}

Token *Token_next(Token *this)
{
    return this->next;
}

Token *Token_last(Token *this)
{
    return this->last;
}

void Token_free(Token *this)
{
    if (this->next != NULL)
    {
        this->next->last = NULL;
    }
    if (this->last != NULL)
    {
        this->last->next = NULL;
    }
    this->next = NULL;
    this->last = NULL;
    free(this->value);
    free(this);
}
