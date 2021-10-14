// tools

#ifndef H_TOOLS
#define H_TOOLS

void tools_assert(bool value, const char *msg)
{
    //
}

void tools_error(const char *msg)
{
    printf("[%s] => %s ...\n", LANG_ERR, msg);
}

void tools_check(bool value, const char *msg)
{
    if (value) return;
    tools_error(msg);
    exit(1);
}

const char *tools_format(const char *temp, const char *msg)
{
    char txt[1024];
    sprintf(txt, temp, msg);
    const char *t = txt;
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

char **S2A(int num, char *s, ...)
{
    va_list valist;
    char **arr = malloc((num + 1) * sizeof(char *));
    int i;
    va_start(valist, s);
    char *n = malloc(sizeof(char));
    n[0] = (char)num;;
    arr[0] = n;
    for (i = 0; i < num; i++)
    {
        arr[i + 1] = (char *)malloc((strlen(s) + 1) * sizeof(char));
        strcpy(arr[i + 1], s);
       s = va_arg(valist, char *);
    }
    va_end(valist);
    return arr;
}

#endif
