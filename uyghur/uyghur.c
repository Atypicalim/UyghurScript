// main program

#ifndef H_UYGHUR
#define H_UYGHUR

#include "others/header.h"

#include "tokenizer.c"
#include "parser.c"
#include "executer.c"
#include "bridge.c"

#include "libraries/global.c"
#include "libraries/number.c"
#include "libraries/string.c"
#include "libraries/time.c"
#include "libraries/system.c"
#include "libraries/file.c"
#include "libraries/box.c"

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
    lib_number_register(uyghur->bridge);
    lib_string_register(uyghur->bridge);
    lib_time_register(uyghur->bridge);
    lib_system_register(uyghur->bridge);
    lib_file_register(uyghur->bridge);
    lib_box_register(uyghur->bridge);
    return uyghur;
}

Value *Uyghur_execute(Uyghur *this, char *path, bool isEntry)
{
    char *content = tools_read_file(path);
    if (content == NULL) return false;
    Token *headToken = Tokenizer_parseCode(this->tokenizer, path, content);
    Leaf *headLeaf = Parser_parseTokens(this->parser, headToken);
    Value *moduleBox = Executer_executeTree(this->executer, path, headLeaf);
    return moduleBox;
}

bool Uyghur_run(Uyghur *this, const char *path)
{
    Value *moduleBox = Uyghur_execute(this, (char *)path, true);
    tools_assert(moduleBox != NULL, tools_format(LANG_ERR_INVALID_INPUT_FILE, path));
    return moduleBox != NULL;
}

void Uyghur_free(Uyghur *this)
{
    Executer_free(this->executer);
    Parser_free(this->parser);
    Tokenizer_free(this->tokenizer);
    free(this);
}

#endif
