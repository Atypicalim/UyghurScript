// tools

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
    if (is_alpha(c)) return true;
    if ((is_uchar_eq_char(c, '_') || is_uchar_eq_char(c, '$')) && is_alpha(n)) return true;
    return false;
}

bool is_letter_body(UCHAR c)
{
    return is_alnum(c) || is_uchar_eq_char(c, '_');
}

bool is_letter_valid(const char* str)
{
    UCHAR c;
    UCHAR n;
    for (int i = 0; str[i] != '\0'; i++) {
        c = ""; // str[i];
        n = ""; // str[i + 1];
        if (i == 0 && !is_letter_begin(c, n)) return false;
        if (i != 0 && !is_letter_body(c)) return false;
    }
    return true;
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
    char r = ' ';
    switch (c[0])
    {
    case '{':
        r = '}';
        break;
    case '}':
        r = '{';
        break;
    case '[':
        r = ']';
        break;
    case ']':
        r = '[';
        break;
    case '(':
        r = ')';
        break;
    case ')':
        r = '(';
        break;
    default:
        break;
    }
    char *s = " ";
    s[0] = r;
    return s;
}

bool is_border_open(UCHAR c) {
    // return c == '{' || c == '[' || c == '(';
    return false;
}

bool is_border_close(UCHAR c) {
    // return c == '}' || c == ']' || c == ')';
    return false;
}

bool is_calculator(UCHAR c)
{
    // return c == '=';
    return false;
}

bool is_scope(UCHAR c)
{
    // return c == '*' || c == '+' || c == '-';
    return false;
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

UCHAR decode_escape(UCHAR c)
{
    char r = ' ';
    switch (c[0])
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
    bool valid = r != ' ';
    int size = valid ? 1 : 2;
    char *s = (char*) malloc(sizeof(char) * size);
    if (valid) {
        s[0] = r;
        s[1] = '\n';
    } else {
        s[0] = '\n';
    }
    return s;
}

#endif
