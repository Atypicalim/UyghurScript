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

Leaf *Uyghur_processCode(Uyghur *this, char *path, char *code)
{
    log_warn("uyghur.tokenize");
    Token *headToken = Tokenizer_parseCode(this->tokenizer, path, code);
    log_warn("uyghur.parse");
    Leaf *headLeaf = Parser_parseTokens(this->parser, headToken);
    return headLeaf;
}

Value *_Uyghur_runCode(Uyghur *this, char *path, char *code)
{
    log_warn("uyghur.execute");
    Leaf *tree = Uyghur_processCode(this, path, code);
    Value *result = Executer_executeCode(this->executer, tree);
    log_warn("uyghur.runned!");
    return result;
}

Value *_Uyghur_runScript(Uyghur *this, char *path, char *code) {
    tools_assert(code != NULL, "%s:[%s]", LANG_ERR_NO_INPUT_FILE, path);
    if (path == NULL) path = UG_SCRIPT_NAME;
    //
    log_warn("uyghur.run: %s", path);
    USTRING lang = helper_parse_language(path);
    tools_assert(lang != NULL, "invalid lang for path:[%s]", path);
    log_warn("uyghur.lang: %s", lang);
    helper_add_languages(this, lang);
    if (!IS_DEVELOP) {
        helper_set_languages(this, lang);
    }
    //
    Leaf *tree = Uyghur_processCode(this, path, code);
    Value *script = Executer_executeScript(this->executer, path, tree);
    return script;
}

Value *Uyghur_runScript(Uyghur *this, char *code) {
    Value *script = _Uyghur_runScript(this, NULL, code);
    Executer_endExecute(this->executer);
    return script;
}

Value *_Uyghur_runPath(Uyghur *this, char *path) {
    bool exist = file_exist(path);
    tools_assert(exist, "%s:[%s]", LANG_ERR_NO_INPUT_FILE, path);
    char *code = file_read(path);
    Value *holder = _Uyghur_runScript(this, path, code);
    pct_free(code);
    return holder;
}

Value *Uyghur_runModule(Uyghur *this, char *path)
{
    Value *module = _Uyghur_runPath(this, path);
    Executer_returnModule(this->executer);
    return module;
}

Value *Uyghur_runProgram(Uyghur *this, char *path)
{
    Value *program = _Uyghur_runPath(this, path);
    Executer_endExecute(this->executer);
    return program;
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
    log_info("> please select a language to run program:");
    CString lang = NULL;
    while (!lang) {
        printf("> ");
        char *text = system_scanf();
        for (size_t i = 0; i < size; i++) {
            char *tp = UG_LANGUAGE_ARRAY[i];
            if (is_eq_string(text, tp)) lang = tp;
        }
    }
    CString *path = tools_format("*.%s", lang);
    CString *code = "";
    Value *script = _Uyghur_runScript(this, path, code);
    //
    log_info("> please end with [;] in single line:", lang);
    String *text = String_new();
    int line = 0;
    while (true) {
        line = line + 1;
        if (line == 1) {
            log_info("> please input code lines in [%s] language:", lang);
        }
        printf("%d > ", line);
        char *input = system_scanf();
        if (!is_eq_string(input, ";")) {
            String_appendStr(text, "\n");
            String_appendStr(text, input);
        } else if (String_length(text) <= 0) {
            break;
        } else {
            _Uyghur_runCode(this, path, String_get(text));
            String_clear(text);
            line = 0;
        }
    }
    //
    log_info("> quitted!");
    Executer_endExecute(this->executer);
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
