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

void Uyghur_execute(Uyghur *uyghur, const char *path)
{
    FILE *file;
    Tokenizer *tokenizer = Tokenizer_new();
    //
    file = fopen(path, "r");
    if (file == NULL) {
        tools_error(tools_format(LANG_ERR_NO_INPUT_FILE, path));
        return;
    }
    //
    Token *haedToken = Tokenizer_parseFile(tokenizer, file);
    //
    fclose(file);
}

void Uyghur_compile(Uyghur *uyghur, const char *path)
{
    //
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
