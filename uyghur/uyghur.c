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

#include "internals/header.h"
#include "externals/header.h"

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
    uyghur->machine->sweeping = IS_GC_SWEEPING;
    uyghur->machine->freezing = false;
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
    bool exist = file_exist(path);
    tools_assert(exist, "%s:[%s]", LANG_ERR_NO_INPUT_FILE, path);
    char *code = file_read(path);
    Value *value = NULL;
    if (code != NULL) {
        value = Uyghur_runCode(this, code, path);
        free(code);
    }
    if (value == NULL) {
        value = Value_newEmpty(NULL);
    }
    return value;
}

void Uyghur_runRepl(Uyghur *this)
{
	printf("%s %s\n", UG_PROJECT_NAME, UG_VERSION_NAME);
    //
    log_info("> available languages:");
    int size = UG_LANGUAGE_COUNT;
    for (size_t i = 0; i < size; i++) {
        char *tp = UG_LANGUAGE_ARRAY[i];
        log_info("> * %s", UG_LANGUAGE_ARRAY[i]);
    }
    //
    log_info("> please select:");
    CString lang = NULL;
    while (!lang) {
        printf("> ");
        char *text = system_scanf();
        for (size_t i = 0; i < size; i++) {
            char *tp = UG_LANGUAGE_ARRAY[i];
            if (is_eq_string(text, tp)) lang = tp;
        }
    }
    //
    log_info("> please enter (%s):", lang);
    while (true) {
        printf("> ");
        char *text = system_scanf();
        Token *headToken = Tokenizer_parseCode(this->tokenizer, ".?", text);
        Leaf *headLeaf = Parser_parseTokens(this->parser, headToken);
        Executer_consumeLeaf(this->executer, headLeaf);
    }
}

void Uyghur_free(Uyghur *this)
{
    //
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
