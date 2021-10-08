// token

#ifndef H_TOKEN
#define H_TOKEN

#include "header.h"

typedef struct _Token {
    struct _Block;
    const char *file;
    int line;
    int column;
    char *type;
    void *value;
} Token;

Token *Token_new(const char *file, int line, int column, char *type, void *value)
{
    Token *token = (Token *)malloc(sizeof(Token));
    Block_init(token, NULL);
    token->file = file;
    token->line = line;
    token->column = column;
    token->type = type;
    token->value = value;
    return token;
}

void Token_print(Token *this)
{
    printf("[(TOKEN) => type:%s, value:(%s) in (%d, %d %s)]\n", this->type, this->value,  this->line,  this->column,  this->file);
}

void Token_free(Token *this)
{
    Block_free(this);
}

#endif
