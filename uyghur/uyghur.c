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

Value *Uyghur_run(Uyghur *this, char *path, char *content)
{
    Token *headToken = Tokenizer_parseCode(this->tokenizer, path, content);
    Leaf *headLeaf = Parser_parseTokens(this->parser, headToken);
    Value *moduleBox = Executer_executeTree(this->executer, path, headLeaf);
    return moduleBox;
}

Value *Uyghur_execute(Uyghur *this, char *path)
{
    char *content = file_read(path);
    if (content == NULL) return NULL;
    return Uyghur_run(this, path, content);
}

void Uyghur_free(Uyghur *this)
{
    Executer_free(this->executer);
    Parser_free(this->parser);
    Tokenizer_free(this->tokenizer);
    free(this);
}

#endif
