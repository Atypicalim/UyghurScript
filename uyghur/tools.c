// tools

#ifndef HEAD_HEADER
#include "header.h"
#endif

#define HEAD_TOOLS

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
    if (value)
        return;
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
    FILE *file = fopen(path, "r");
    fseek(file, 0, SEEK_END);
    long lSize = ftell(file);
    text = (char *)malloc(lSize + 1);
    rewind(file);
    fread(text, sizeof(char), lSize, file);
    text[lSize] = '\0';
    fclose(file);
    return text;
}
