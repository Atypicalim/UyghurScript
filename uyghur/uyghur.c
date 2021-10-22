// main program

#ifndef H_UYGHUR
#define H_UYGHUR

#include "others/header.h"

#include "tokenizer.c"
#include "parser.c"
#include "executer.c"
#include "bridge.c"

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
    return uyghur;
}

bool Uyghur_execute(Uyghur *this, const char *path, const char *code)
{
    Token *headToken = Tokenizer_parseCode(this->tokenizer, path, code);
    Leaf *headLeaf = Parser_parseTokens(this->parser, headToken);
    Hashmap *scope = Executer_executeProgram(this->executer, headLeaf);
    return scope != NULL;
}

bool Uyghur_run(Uyghur *this, const char *path)
{
    char *content = tools_read_file(path);
    tools_assert(content != NULL, tools_format(LANG_ERR_NO_INPUT_FILE, path));
    return Uyghur_execute(this, path, content);
}

void Uyghur_free(Uyghur *this)
{
    Executer_free(this->executer);
    Parser_free(this->parser);
    Tokenizer_free(this->tokenizer);
    free(this);
}

#endif
