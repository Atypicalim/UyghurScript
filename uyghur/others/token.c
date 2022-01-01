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
    bool isKey;
    char *scope;
    char *kind;
} Token;

Token *Token_new(char *type, void *value)
{
    Token *token = (Token *)malloc(sizeof(Token));
    Block_init(token, NULL);
    token->file = "";
    token->line = 0;
    token->column = 0;
    token->type = type;
    token->value = value;
    token->isKey = false;
    token->scope = NULL;
    token->kind = NULL;
    return token;
}

void Token_bindInfo(Token *this, const char *file, int line, int column)
{
    this->file = file;
    this->line = line;
    this->column = column;
}

Token *Token_empty()
{
    return Token_new(TTYPE_WORD, TVALUE_EMPTY);
}

Token *Token_name(char *name)
{
    return Token_new(TTYPE_NAME, name);
}

Token *Token_key(char *key, char *scope, char *kind)
{
    Token *token = Token_new(TTYPE_KEY, key);
    token->isKey = true;
    token->scope = scope;
    token->kind = kind;
    return token;
}

bool Token_isStatic(Token *this)
{
    return is_equal(this->type, TTYPE_STRING)
    || is_equal(this->type, TTYPE_NUMBER)
    || is_equal(this->type, TTYPE_BOOL)
    || is_equal(this->type, TTYPE_EMPTY);
}

void Token_print(Token *this)
{
    char *scope = this->scope != NULL ? this->scope : "NULL";
    printf("[(TOKEN) => type:%s, value:(%s), scope:(%s) in (%d, %d %s)]\n", this->type, this->value, scope, this->line, this->column, this->file);
}

void Token_free(Token *this)
{
    Block_free(this);
    // TODO: ug free pointers
}

#endif
