// main program

#ifndef H_UYGHUR
#define H_UYGHUR

#include "others/header.h"

#include "tokenizer.c"
#include "parser.c"
#include "compiler.c"
#include "machine.c"
#include "executer.c"
#include "debug.c"
#include "bridge.c"

#include "internals/header.h"
#include "externals/header.h"

void Uyghur_init(Uyghur *this)
{
    this->lettersMap = Hashmap_new(false);
    this->aliasesMap = Hashmap_new(false);
    this->langsMap = Hashmap_new(false);
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
    uyghur->compiler = Compiler_new(uyghur);
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

Leaf *_Uyghur_processLang(Uyghur *this, char *path)
{
    CString lang = helper_parse_language(path);
    tools_assert(lang != NULL, "invalid lang for path:[%s]", path);
    log_debug("uyghur.lang: %s", lang);
    helper_add_languages(this, lang);
    if (!UG_IS_DEVELOP) {
        helper_set_languages(this, lang);
    }
}

Leaf *_Uyghur_processCode(Uyghur *this, char *path, char *code)
{
    log_debug("uyghur.tokenize");
    Token *headToken = Tokenizer_parseCode(this->tokenizer, path, code);
    log_debug("uyghur.parse");
    Leaf *headLeaf = Parser_parseTokens(this->parser, headToken);
    return headLeaf;
}

Value *_Uyghur_runCode(Uyghur *this, char *path, char *code) {
    tools_assert(code != NULL, "%s:[%s]", LANG_ERR_NO_INPUT_FILE, path);
    if (path == NULL) path = UG_SCRIPT_NAME;
    log_debug("uyghur.run: %s", path);
    _Uyghur_processLang(this, path);
    Leaf *tree = _Uyghur_processCode(this, path, code);
    Value *script = Executer_executeScript(this->executer, path, tree);
    return script;
}

Value *Uyghur_runScript(Uyghur *this, char *code) {
    Value *script = _Uyghur_runCode(this, NULL, code);
    Executer_endExecute(this->executer);
    return script;
}

Value *Uyghur_runModule(Uyghur *this, char *path)
{
    char *code = helper_read_code_file(path);
    Value *module = _Uyghur_runCode(this, path, code);
    Executer_returnModule(this->executer);
    pct_free(code);
    return module;
}

Value *Uyghur_runProgram(Uyghur *this, char *path, cArgs *args)
{
    char *code = helper_read_code_file(path);
    Value *program = _Uyghur_runCode(this, path, code);
    Executer_endExecute(this->executer);
    pct_free(code);
    return program;
}

void Uyghur_runCompile(Uyghur *this, char *path, CString lang)
{
    //
    if (!is_string_valid(lang)) {
        printf("> available languages are:\n");
        helper_print_languages(NULL);
        printf("> please select a language:\n");
        lang = helper_select_language(NULL);
    }
    //
    log_debug("path:%s", path);
    _Uyghur_processLang(this, path);
    char *code = helper_read_code_file(path);
    Leaf *tree = _Uyghur_processCode(this, path, code);
    Compiler_compileCode(this->compiler, tree, lang);
    pct_free(code);
}

void Uyghur_runRepl(Uyghur *this, CString lang)
{
	printf("%s %s\n", UG_PROJECT_NAME, UG_VERSION_NAME);
    //
    if (!is_string_valid(lang)) {
        printf("> available languages are:\n");
        helper_print_languages(NULL);
        printf("> please select a language:\n");
        lang = helper_select_language(NULL);
    }
    //
    CString *code = "";
    CString *path = tools_format("*.%s", lang);
    _Uyghur_processLang(this, path);
    //
    printf("> please end with [;] in single line:\n");
    String *text = String_new();
    int line = 0;
    while (true) {
        line = line + 1;
        if (line == 1) {
            printf("> please input code lines in [%s] language:\n", lang);
        }
        printf("%d > ", line);
        char *input = system_scanf();
        if (!is_eq_string(input, ";")) {
            String_appendStr(text, "\n");
            String_appendStr(text, input);
        } else if (String_length(text) <= 0) {
            break;
        } else {
            log_debug("uyghur.execute");
            char *code = String_get(text);
            Leaf *tree = _Uyghur_processCode(this, path, code);
            Value *result = Executer_executeCode(this->executer, tree);
            log_debug("uyghur.runned!");
            String_clear(text);
            line = 0;
        }
    }
    //
    printf("> quitted!\n");
    Executer_endExecute(this->executer);
}

void Uyghur_free(Uyghur *this)
{
    //
    Machine_free(this->machine);
    Executer_free(this->executer);
    Compiler_free(this->compiler);
    Parser_free(this->parser);
    Tokenizer_free(this->tokenizer);
    // 
    if (this->lettersMap != NULL) {
        Object_release(this->lettersMap);
        this->lettersMap = NULL;
    }
    if (this->aliasesMap != NULL) {
        Object_release(this->aliasesMap);
        this->aliasesMap = NULL;
    }
    if (this->langsMap != NULL) {
        Object_release(this->langsMap);
        this->langsMap = NULL;
    }
    if (this->wordsMap != NULL) {
        Object_release(this->wordsMap);
        this->wordsMap = NULL;
    }
    //
    log_debug("uyghur.freed!");
    free(this);
}

#endif
