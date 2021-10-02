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

const char *tools_format(const char *temp, const char *msg)
{
    char txt[1024];
    sprintf(txt, temp, msg);
    const char *t = txt;
    return t;
}