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
    char *value;
    char *scope;
} Token;

Token *Token_new(const char *file, int line, int column, char *type, void *value, void *scope)
{
    Token *token = (Token *)malloc(sizeof(Token));
    Block_init(token, NULL);
    token->file = file;
    token->line = line;
    token->column = column;
    token->type = type;
    token->value = value;
    token->scope = scope;
    return token;
}

Token *Token_empty()
{
    return Token_new("", 0, 0, TTYPE_WORD, TVALUE_EMPTY, NULL);
}

Token *Token_name(char *name)
{
    return Token_new("", 0, 0, TTYPE_NAME, name, NULL);
}

void Token_print(Token *this)
{
    char *scope = this->scope != NULL ? this->scope : "NULL";
    printf("[(TOKEN) => type:%s, value:(%s), scope:(%s) in (%d, %d %s)]\n", this->type, this->value, scope, this->line, this->column, this->file);
}

void Token_free(Token *this)
{
    Block_free(this);
}

#endif
