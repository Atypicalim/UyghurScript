// tools

#ifndef H_TOOLS
#define H_TOOLS

void tools_error(const char* msg, ...) {
    va_list lst;
    va_start(lst, msg);
    printf("[%s] => ", LANG_ERR);
    vfprintf(stdout, msg, lst);
    printf("\n");
    va_end(lst);
    exit(1);
}

void tools_assert(bool value, const char *msg, ...)
{
    if (value == true) return;
    va_list lst;
    va_start(lst, msg);
    printf("[%s] => ", LANG_ERR);
    vfprintf(stdout, msg, lst);
    printf("\n");
    va_end(lst);
    exit(1);
}

char *tools_format(char *msg, ...)
{
    va_list lst;
    va_start(lst, msg);
    int bufsz = vsnprintf(NULL, 0, msg, lst);
    char* t = malloc(bufsz + 1);
    vsnprintf(t, bufsz + 1, msg, lst);
    va_end(lst);
    return t;
}

char *tools_read_file(const char *path)
{
    char *text;
    FILE *file = fopen(path, "rb");
    fseek(file, 0, SEEK_END);
    long lSize = ftell(file);
    text = (char *)malloc(lSize + 1);
    rewind(file);
    fread(text, sizeof(char), lSize, file);
    text[lSize] = '\0';
    fclose(file);
    return text;
}

char *tools_str_new(char *str, int extraLen)
{
    size_t len = strlen(str);
    char *dest = malloc(len + 1 + extraLen);
    dest[len + extraLen] = '\0';
    return dest;
}

char *tools_str_apent(char *str, char c, bool notFree)
{
    size_t len = strlen(str);
     char *dest = tools_str_new(str, 1);
    strcpy(dest, str);
    dest[len] = c;
    if (!notFree)
    {
        free(str);
    }
    return dest;
}

char *str_concat(char *one, char *two)
{
    int bufsz = snprintf(NULL, 0, "%s%s", one, two);
    char* buf = malloc(bufsz + 1);
    snprintf(buf, bufsz + 1, "%s%s", one, two);
    return buf;
}

bool is_equal(char *this, char*other)
{
    return strcmp(this, other) == 0;
}

#endif
