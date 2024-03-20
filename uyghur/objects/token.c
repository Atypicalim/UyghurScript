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
    return Token_new(UG_TTYPE_EMP, TVALUE_EMPTY);
}

Token *Token_name(char *name)
{
    return Token_new(UG_TTYPE_NAM, name);
}

Token *Token_key(char *keyType, char *keyValue, char *scope)
{
    Token *token = Token_new(UG_TTYPE_KEY, keyValue);
    token->extra = Token_new(keyType, scope);
    return token;
}

bool Token_isEmpty(Token *this)
{
    return is_eq_string(this->type, UG_TTYPE_EMP);
}

bool Token_isBool(Token *this)
{
    return is_eq_string(this->type, UG_TTYPE_BOL);
}

bool Token_isNumber(Token *this)
{
    return is_eq_string(this->type, UG_TTYPE_NUM);
}

bool Token_isString(Token *this)
{
    return is_eq_string(this->type, UG_TTYPE_STR);
}

bool Token_isName(Token *this)
{
    return is_eq_string(this->type, UG_TTYPE_NAM);
}

bool Token_isWord(Token *this)
{
    return is_eq_string(this->type, UG_TTYPE_WRD);
}

bool Token_isKey(Token *this)
{
    return is_eq_string(this->type, UG_TTYPE_KEY);
}

bool Token_isKeyOfName(Token *this)
{
    return Token_isKey(this) && Token_isName(this->extra);
}

bool Token_isKeyOfString(Token *this)
{
    return Token_isKey(this) && Token_isString(this->extra);
}

bool Token_isKeyOfNumber(Token *this)
{
    return Token_isKey(this) && Token_isNumber(this->extra);
}

bool Token_isCalculation(Token *this)
{
    return is_eq_string(this->type, UG_TTYPE_CLC);
}

bool Token_isStatic(Token *this)
{
    return Token_isString(this) || Token_isNumber(this) || Token_isBool(this) || Token_isEmpty(this);
}

char *_Token_toString(char *val, char *tp)
{
    if (is_eq_string(tp, UG_TTYPE_NUM)) {
        return tools_number_to_string(atof(val));
    } else {
        return tools_format("%s", val);
    }
}

char *Token_toString(Token *this)
{
    if (Token_isKey(this)) {
        Token *extra = (Token *)this->extra;
        return _Token_toString(this->value, extra->type);
    } else {
        return _Token_toString(this->value, this->type);
    }
}

void Token_print(Token *this)
{
    if (this == NULL) {
        printf("[NULL]\n");
    } else {
        char *fmt = "[(TOKEN) => type:%s, value:(%s) in (%d, %d %s)]\n";
        printf(fmt, this->type, this->value, this->line, this->column, this->file);
    }
}

void Token_free(Token *this)
{
    if (Token_isKey(this)) {
        Object_release(this->extra);
    }
    Object_free(this);
    // TODO: ug free pointers
}

#endif
