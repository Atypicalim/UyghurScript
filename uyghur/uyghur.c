// main program

#ifndef H_UYGHUR
#define H_UYGHUR

#include "others/header.h"

#include "tokenizer.c"
#include "parser.c"
#include "executer.c"
#include "bridge.c"
#include "libraries/global.c"

void Uyghur_init(Uyghur *this)
{
    //
}

Uyghur *Uyghur_new()
{
    Uyghur *uyghur = malloc(sizeof(Uyghur));
    Uyghur_init(uyghur);
    uyghur->tokenizer = Tokenizer_new(uyghur);
    uyghur->parser = Parser_new(uyghur);
    uyghur->executer = Executer_new(uyghur);
    uyghur->bridge = Bridge_new(uyghur);
    lib_global_register(uyghur->bridge);
    return uyghur;
}

bool Uyghur_execute(Uyghur *this, char *path)
{
    char *content = tools_read_file(path);
    if (content == NULL) return false;
    Token *headToken = Tokenizer_parseCode(this->tokenizer, path, content);
    Leaf *headLeaf = Parser_parseTokens(this->parser, headToken);
    bool isSuccess = Executer_executeTree(this->executer, path, headLeaf);
    return isSuccess;
}

bool Uyghur_run(Uyghur *this, const char *path)
{
    bool isSuccess = Uyghur_execute(this, (char *)path);
    tools_assert(isSuccess, tools_format(LANG_ERR_INVALID_INPUT_FILE, path));
}

void Uyghur_free(Uyghur *this)
{
    Executer_free(this->executer);
    Parser_free(this->parser);
    Tokenizer_free(this->tokenizer);
    free(this);
}

#endif
