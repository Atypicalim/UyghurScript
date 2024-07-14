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

bool is_eq_character(char *str, char ch)
{
    return strlen(str) == 1 && str[0] == ch;
}

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
        if (is_eq_character(s, target)) return true;
        s = va_arg(valist, char *);
    }
    va_end(valist);
    return false;
}

bool is_number_begin(char c, char n)
{
    if (isdigit(c)) return true;
    if ((c == '+' || c == '-') && isdigit(n)) return true;
    return false;

}

bool is_number_body(char c)
{
    return isdigit(c) || c == '.';
}

bool is_letter_begin(char c, char n)
{
    if (isalpha(c)) return true;
    if ((c == '_' || c == '$') && isalpha(n)) return true;
    return false;
}

bool is_letter_body(char c)
{
    return isalnum(c) || c == '_';
}

bool is_letter_valid(const char* str)
{
    char c;
    char n;
    for (int i = 0; str[i] != '\0'; i++) {
        c = str[i];
        n = str[i + 1];
        if (i == 0 && !is_letter_begin(c, n)) return false;
        if (i != 0 && !is_letter_body(c)) return false;
    }
    return true;
}

bool is_string_open(char c) {
    return c == '[';
}

bool is_string_body(char c) {
    return c != '[' && c != ']';
}

bool is_string_close(char c) {
    return c == ']';
}

char convert_border_pair(char c) {
    if (c == '{') return '}';
    if (c == '}') return '{';
    if (c == '[') return ']';
    if (c == ']') return '[';
    if (c == '(') return ')';
    if (c == ')') return '(';
    return ' ';
}

bool is_border_open(char c) {
    return c == '{' || c == '[' || c == '(';
}

bool is_border_close(char c) {
    return c == '}' || c == ']' || c == ')';
}

bool is_calculator(char c)
{
    return c == '=';
}

bool is_scope(char c)
{
    return c == '*' || c == '+' || c == '-';
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
    switch (c)
    {
    case '[':
        return '[';
        break;
    case ']':
        return ']';
        break;
    case '\\':
        return '\\';
        break;
    case 'n':
        return '\n';
        break;
    case 'a':
        return '\a';
        break;
    case 'b':
        return '\b';
        break;
    case 'f':
        return '\f';
        break;
    case 'v':
        return '\v';
        break;
    case 'r':
        return '\r';
        break;
    case 't':
        return '\t';
        break;
    default:
        return '\0';  
        break;
    }       
}

#endif
