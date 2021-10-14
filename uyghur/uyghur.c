// main program

#include "header.h"

#include "token.c"
#include "tokenizer.c"
#include "leaf.c"
#include "parser.c"

void Uyghur_init(Uyghur *this)
{
    this->TTYPES_GROUP_DEFINE = S2Q(TTYPE_NAME, TTYPE_STRING, TTYPE_NUMBER, TTYPE_BOOL, TTYPE_EMPTY);
    this->TTYPES_GROUP_STRING = S2Q(TTYPE_NAME, TTYPE_STRING);
    this->TTYPES_GROUP_NUMBER = S2Q(TTYPE_NAME, TTYPE_NUMBER);
    this->TTYPES_GROUP_LOGICS = S2Q(TTYPE_NAME, TTYPE_BOOL, TTYPE_EMPTY);
}

Uyghur *Uyghur_new()
{
    Uyghur *uyghur = malloc(sizeof(Uyghur));
    Uyghur_init(uyghur);
    uyghur->tokenizer = Tokenizer_new(uyghur);
    uyghur->parser = Parser_new(uyghur);
    return uyghur;
}

void Uyghur_execute(Uyghur *this, const char *path, const char *code)
{
    Token *headToken = Tokenizer_parseCode(this->tokenizer, path, code);
    Leaf *headLeaf = Parser_parseTokens(this->parser, headToken);
    printf("--->\n");
    helper_print_leaf(headLeaf, " ");
    printf("--->\n");
}

void Uyghur_compile(Uyghur *this, const char *path)
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

void Uyghur_free(Uyghur *this)
{
    Parser_free(this->parser);
    Tokenizer_free(this->tokenizer);
    free(this);
}
