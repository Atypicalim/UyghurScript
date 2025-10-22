// token

#ifndef H_UG_HEADER
#include "../others/header.h"
#endif

#ifndef H_UG_TOKEN
#define H_UG_TOKEN

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

Token *Token_empty()
{
    if (Token_EMPTY == NULL) {
        Token_EMPTY = Token_new(UG_TTYPE_NIL, LETTER_NIL);
    }
    return Token_EMPTY;
}

Token *Token_getTemporary() {
    if (Token_TEMPORARY == NULL) {
        Token_TEMPORARY = Token_new(UG_TTYPE_NIL, LETTER_NIL);
    }
    return Token_TEMPORARY;
}

Token *Token_variable()
{
    if (Token_VARIABLE == NULL) {
        Token_VARIABLE = Token_new(UG_TTYPE_NAM, TEXT_VARIABLE);
    }
    return Token_VARIABLE;
}

Token *Token_function()
{
    if (Token_FUNCTION == NULL) {
        Token_FUNCTION = Token_new(UG_TTYPE_NAM, TEXT_FUNCTION);
    }
    return Token_FUNCTION;
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
    return is_eq_string(this->type, UG_TTYPE_NIL);
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
        return tools_number_to_string(tools_string_to_number(val));
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
