// token

#ifndef H_UG_HEADER
#include "../others/header.h"
#endif

#ifndef H_UG_TOKEN
#define H_UG_TOKEN

typedef struct _Token {
    struct _Object;
    const char *file;
    int line;
    int column;
    char *type;
    char *value;
    void *extra;
    void *next;
    void *last;
} Token;

Token *Token_new(char *type, void *value)
{
    Token *token = (Token *)malloc(sizeof(Token));
    Object_init(token, PCT_OBJ_TOKEN);
    token->file = "";
    token->line = 0;
    token->column = 0;
    token->type = type;
    token->value = value;
    token->extra = NULL;
    token->next = NULL;
    token->last = NULL;
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
    return Token_new(TTYPE_EMPTY, TVALUE_EMPTY);
}

Token *Token_name(char *name)
{
    return Token_new(TTYPE_NAME, name);
}

Token *Token_key(char *scope, char *keyType, char *keyValue)
{
    Token *token = Token_new(TTYPE_KEY, scope);
    token->extra = Token_new(keyValue, keyValue);
    return token;
}

bool Token_isName(Token *this)
{
    return is_equal(this->type, TTYPE_NAME);
}

bool Token_isKey(Token *this)
{
    return is_equal(this->type, TTYPE_KEY);
}

bool Token_isWord(Token *this)
{
    return is_equal(this->type, TTYPE_WORD);
}

bool Token_isString(Token *this)
{
    return is_equal(this->type, TTYPE_STRING);
}

bool Token_isNumber(Token *this)
{
    return is_equal(this->type, TTYPE_NUMBER);
}

bool Token_isBool(Token *this)
{
    return is_equal(this->type, TTYPE_BOOL);
}

bool Token_isEmpty(Token *this)
{
    return is_equal(this->type, TTYPE_EMPTY);
}

bool Token_isBox(Token *this)
{
    return is_equal(this->type, TTYPE_BOX);
}

bool Token_isCalculation(Token *this)
{
    return is_equal(this->type, TTYPE_CALCULATION);
}

bool Token_isStatic(Token *this)
{
    return Token_isString(this) || Token_isNumber(this) || Token_isBool(this) || Token_isEmpty(this);
}

void Token_print(Token *this)
{
    printf("[(TOKEN) => type:%s, value:(%s) in (%d, %d %s)]\n", this->type, this->value, this->line, this->column, this->file);
}

void Token_free(Token *this)
{
    Object_free(this);
    // TODO: ug free pointers
}

#endif
