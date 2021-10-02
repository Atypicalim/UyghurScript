// main program

#ifndef HEAD_HEADER
#include "header.h"
#endif

#include "token.c"
#include "tokenizer.c"

typedef struct {
    bool running;
} Uyghur;

Uyghur *Uyghur_new()
{
    Uyghur *uyghur = malloc(sizeof(Uyghur));
    return uyghur;
}

void Uyghur_execute(Uyghur *uyghur, const char *path, const char *code)
{
    Tokenizer *tokenizer = Tokenizer_new();
    //
    Token *haedToken = Tokenizer_parseCode(tokenizer, path, code);
    //
    Tokenizer_free(tokenizer);
}

void Uyghur_compile(Uyghur *uyghur, const char *path)
{
    //
}

void Uyghur_run(Uyghur *this, const char *path)
{
    char *content = tools_read_file(path);
    tools_check(content != NULL, tools_format(LANG_ERR_NO_INPUT_FILE, path));
    Uyghur_execute(this, path, content);
}

void Uyghur_inport()
{
    //
}

void Uyghur_export()
{
    //
}

void Uyghur_free(Uyghur *uyghur)
{
    //
}
