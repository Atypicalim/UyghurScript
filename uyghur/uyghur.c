// main program

#ifndef H_UYGHUR
#define H_UYGHUR

#include "others/header.h"

#include "tokenizer.c"
#include "parser.c"
#include "machine.c"
#include "executer.c"
#include "debug.c"
#include "bridge.c"

void Uyghur_init(Uyghur *this)
{
    this->lettersMap = Hashmap_new(false);
    this->wordsMap = Hashmap_new(false);
    this->language = NULL;
}

Uyghur *Uyghur_instance()
{
    if (__uyghur != NULL) return __uyghur;
    Uyghur *uyghur = malloc(sizeof(Uyghur));
    Uyghur_init(uyghur);
    __uyghur = uyghur;
    uyghur->tokenizer = Tokenizer_new(uyghur);
    uyghur->parser = Parser_new(uyghur);
    uyghur->machine = Machine_new(uyghur);
    uyghur->debug = Debug_new(uyghur);
    uyghur->bridge = Bridge_new(uyghur);
    uyghur->executer = Executer_new(uyghur);
    //
    register_internal_libraries(uyghur->bridge);
    register_external_libraries(uyghur->bridge);
    //
    uyghur->machine->sweeping = true;
    uyghur->machine->freezing = false;
    uyghur->machine->collects = true;
    Machine_runGC(uyghur->machine);
    //
    return __uyghur;
}

Value *Uyghur_runCode(Uyghur *this, char *code, char *path)
{
    if (path == NULL) path = UG_SCRIPT_NAME;
    log_warn("uyghur.run: %s", path);
    USTRING lang = helper_parse_language(path);
    //
    log_warn("uyghur.lang: %s", lang);
    helper_add_languages(this, lang);
    if (!IS_DEVELOP) {
        helper_set_languages(this, lang);
    }
    //
    log_warn("uyghur.tokenize");
    Token *headToken = Tokenizer_parseCode(this->tokenizer, path, code);
    log_warn("uyghur.parse");
    Leaf *headLeaf = Parser_parseTokens(this->parser, headToken);
    log_warn("uyghur.execute");
    Value *moduleBox = Executer_executeTree(this->executer, path, headLeaf);
    log_warn("uyghur.runned!");
    return moduleBox;
}

Value *Uyghur_runPath(Uyghur *this, char *path)
{
    char *code = file_read(path);
    if (code == NULL) return NULL;
    Value *value = Uyghur_runCode(this, code, path);
    free(code);
    return value;
}

Value *Uyghur_runArgs(Uyghur *this, int argc, char const *argv[])
{
    if (argc <= 1) return NULL;
    char *path = (char *)argv[1];
    bool exist = file_exist(path);
    tools_assert(exist, "%s:[%s]", LANG_ERR_NO_INPUT_FILE, path);
    Uyghur_runPath(this, path);

}

void Uyghur_free(Uyghur *this)
{
    
    Machine_free(this->machine);
    Executer_free(this->executer);
    Parser_free(this->parser);
    Tokenizer_free(this->tokenizer);
    // 
    if (this->lettersMap != NULL)
    {
        Object_release(this->lettersMap);
        this->lettersMap = NULL;
    }
    if (this->wordsMap != NULL)
    {
        Object_release(this->wordsMap);
        this->wordsMap = NULL;
    }
    //
    log_warn("uyghur.freed!");
    free(this);
}

#endif
