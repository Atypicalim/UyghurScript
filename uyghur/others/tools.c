// tools

#include "header.h"

#ifndef H_TOOLS
#define H_TOOLS


double _toolsIntPart;
bool tools_number_is_integer(double num)
{
    return modf(num, &_toolsIntPart) == 0.0;
}

char *tools_number_to_string(double num)
{
    return tools_number_is_integer(num) ? tools_format("%i", (int)num) : tools_format("%.15g", (double)num);
}

char *tools_boolean_to_string(bool bol)
{
    return tools_format("%s", bol ? TVALUE_TRUE : TVALUE_FALSE);
}

bool is_eq_string(char *this, char* other)
{
    return strcmp(this, other) == 0;
}

bool is_eq_to_char(char *str, char c)
{
    return strlen(str) == 1 && str[0] == c;
}

bool is_eq_to_uchar(char *str, UCHAR ch)
{
    return utf8cmp(str, ch) == 0;
}

////////////////////////////////////////////////////////////////////////

bool is_uchar_eq_char(UCHAR ch, char c) {
    if (ch[1] != '\0') return false;
    return ch[0] == c;
}

bool is_char_eq_char(char c1, char c2) {
    return c1 == c2;
}

bool is_uchar_eq_uchar(UCHAR ch1, UCHAR ch2) {
    return utf8cmp(ch1, ch2) == 0;
}

bool is_line(UCHAR uChar) {
    if (uChar[1] != '\0') return false;
    return uChar[0] == '\n';
}

bool is_empty(UCHAR uChar) {
    return strlen(uChar) == 0 && uChar[0] == '\0';
}

bool is_space(UCHAR uChar) {
    if (uChar[1] != '\0') return false;
    return !!isspace(uChar[0]);
}

bool is_cntrl(UCHAR uChar) {
    if (uChar[1] != '\0') return false;
    return !!iscntrl(uChar[0]);
}

bool is_alnum(UCHAR uChar) {
    if (uChar[1] != '\0') return false;
    return !!isalnum(uChar[0]);
}

bool is_alpha(UCHAR uChar) {
    if (uChar[1] != '\0') return false;
    return !!isalpha(uChar[0]);
}

bool is_digit(UCHAR uChar) {
    if (uChar[1] != '\0') return false;
    return !!isdigit(uChar[0]);
}

int is_sign(UCHAR uChar) {
    if (uChar[1] != '\0') return false;
    char c = uChar[0];
    return isprint(c) && !isalpha(c) && !isdigit(c);
}

bool is_special(UCHAR uChar) {
    return is_space(uChar) || is_cntrl(uChar) || is_sign(uChar);
}

////////////////////////////////////////////////////////////////////////

UCHAR clone_uchar(UCHAR ch) {
    char *_ch = malloc(sizeof(char) * 10);
    strcpy(_ch, ch);
    return _ch;
}

bool free_uchar(UCHAR _ch) {
    free(_ch);
}

////////////////////////////////////////////////////////////////////////

bool is_eq_strings(char *target, int num, char *s, ...)
{
    tools_assert(target != NULL, "invalid target in is values");
    va_list valist;
    int i;
    va_start(valist, s);
    for (i = 0; i < num; i++)
    {
        if (is_eq_string(target, s)) return true;
        s = va_arg(valist, char *);
    }
    va_end(valist);
    return false;
}

bool is_eq_characters(char target, int num, char *s, ...)
{
    tools_assert(target != ' ', "invalid target in is characters");
    va_list valist;
    int i;
    va_start(valist, s);
    for (i = 0; i < num; i++)
    {
        if (is_eq_to_char(s, target)) return true;
        s = va_arg(valist, char *);
    }
    va_end(valist);
    return false;
}

bool is_number_begin(UCHAR c, UCHAR n)
{
    if (is_digit(c)) return true;
    if ((is_uchar_eq_uchar(c, SIGN_ADD) || is_uchar_eq_uchar(c, SIGN_SUB)) && is_digit(n)) return true;
    return false;

}

bool is_number_body(UCHAR c)
{
    return is_digit(c) || is_uchar_eq_char(c, '.');
}

bool is_letter_begin(UCHAR c, UCHAR n)
{
    if (!is_special(c)) return true;
    if ((is_uchar_eq_char(c, '_') || is_uchar_eq_char(c, '$')) && !is_special(n)) return true;
    return false;
}

bool is_letter_body(UCHAR c)
{
    return !is_special(c) || is_uchar_eq_char(c, '_');
}

bool is_string_open(UCHAR c) {
    return is_uchar_eq_char(c, '[');
}

bool is_string_body(UCHAR c) {
    return !is_uchar_eq_char(c, '[') && !is_uchar_eq_char(c, ']');
}

bool is_string_close(UCHAR c) {
    return is_uchar_eq_char(c, ']');
}

UCHAR convert_border_pair(UCHAR c) {
    UCHAR r = " ";
    switch (c[0])
    {
    case '{':
        r = SIGN_CLOSE_MIDDLE;
        break;
    case '}':
        r = SIGN_OPEN_MIDDLE;
        break;
    case '[':
        r = SIGN_CLOSE_BIG;
        break;
    case ']':
        r = SIGN_OPEN_BIG;
        break;
    case '(':
        r = SIGN_CLOSE_SMALL;
        break;
    case ')':
        r = SIGN_OPEN_SMALL;
        break;
    default:
        break;
    }
    return r;
}

bool is_border_open(UCHAR c) {
    return is_uchar_eq_char(c, '{') || is_uchar_eq_char(c, '[') || is_uchar_eq_char(c, '(');
}

bool is_border_close(UCHAR c) {
    return is_uchar_eq_char(c, '}') || is_uchar_eq_char(c, ']') || is_uchar_eq_char(c, ')');
}

bool can_generate_open(char *last) {
    return last == NULL || is_eq_string(last, TVALUE_COLON) || is_eq_string(last, TVALUE_COMMA);
}

bool can_generate_close(char *last) {
    return last == NULL || !is_eq_string(last, TVALUE_COLON);
}

bool is_equaling(UCHAR c)
{
    return is_uchar_eq_char(c, '=');
}

bool is_scope(UCHAR c)
{
    return is_uchar_eq_char(c, '*') || is_uchar_eq_char(c, '+') || is_uchar_eq_char(c, '-');
}

//

bool is_type_listable(char tp) {
    return tp == UG_TYPE_LST;
}

bool is_type_dictable(char tp) {
    return tp == UG_TYPE_DCT;
}

bool is_type_holdable(char tp) {
    return tp == UG_TYPE_BOX
    || tp == UG_TYPE_SCP
    || tp == UG_TYPE_MDL;
}

bool is_type_objective(char tp) {
    return tp == UG_TYPE_CTR
    || tp == UG_TYPE_ATR
    || tp == UG_TYPE_OBJ;
}

bool is_type_runnable(char tp) {
    return tp == UG_TYPE_NTV || tp == UG_TYPE_WKR;
}

char* get_value_name(char tp, char* def) {
    switch (tp) {
        case UG_TYPE_BOX: return "box";
        case UG_TYPE_LST: return "lst";
        case UG_TYPE_DCT: return "dct";
        case UG_TYPE_SCP: return "scp";
        case UG_TYPE_MDL: return "mdl";
        //
        case UG_TYPE_CTR: return "ctr";
        case UG_TYPE_ATR: return "atr";
        case UG_TYPE_OBJ: return "obj";
        //
        case UG_TYPE_NTV: return "ntv";
        case UG_TYPE_WKR: return "wkr";
        //
        default: return def;
    }
}

// 

char *b2s(bool value)
{
    if (value == true) return "true";
    if (value == false) return "false";
    return "empty";
}

char *o2s(void *value)
{
    if (value == NULL) return "<NULL>";
    return tools_format("<Object => p:%p>", value);
}

int p2i(void* value){
    return *((int*) value);
}

char decode_escape(char c)
{
    char r = CHAR_QUESSION;
    switch (c)
    {
    case '[':
        r = '[';
        break;
    case ']':
        r = ']';
        break;
    case '\\':
        r = '\\';
        break;
    case 'n':
        r = '\n';
        break;
    case 'a':
        r = '\a';
        break;
    case 'b':
        r = '\b';
        break;
    case 'f':
        r = '\f';
        break;
    case 'v':
        r = '\v';
        break;
    case 'r':
        r = '\r';
        break;
    case 't':
        r = '\t';
        break;
    default:
        break;
    }
    return r;
}

char *escape_cstring(const char *src) {
    int i, j;
    char *pw;

    for (i = j = 0; src[i] != '\0'; i++) {
        if (src[i] == '\n' || src[i] == '\t' ||
            src[i] == '\\' || src[i] == '\"') {
            j++;
        }
    }
    pw = malloc(i + j + 1);

    for (i = j = 0; src[i] != '\0'; i++) {
        switch (src[i]) {
        case '\n': pw[i+j] = '\\'; pw[i+j+1] = 'n'; j++; break;
        case '\t': pw[i+j] = '\\'; pw[i+j+1] = 't'; j++; break;
        case '\\': pw[i+j] = '\\'; pw[i+j+1] = '\\'; j++; break;
        case '\"': pw[i+j] = '\\'; pw[i+j+1] = '\"'; j++; break;
        default:   pw[i+j] = src[i]; break;
        }
    }
    pw[i+j] = '\0';
    return pw;
}

// TODO:move

typedef void (*ARRAY_FOREACH_FUNC)(int, void *, void *);

void Array_foreachItem(Array *this, ARRAY_FOREACH_FUNC func, void *arg) {
    for (int i = 0; i < this->length; i++) {
        void *ptr = Array_get(this, i);
        func(i, ptr, arg);
    }
}

#endif
