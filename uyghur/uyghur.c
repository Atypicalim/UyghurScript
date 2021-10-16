// main program

#include "header.h"

#include "token.c"
#include "tokenizer.c"
#include "leaf.c"
#include "parser.c"

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
    tools_assert(content != NULL, tools_format(LANG_ERR_NO_INPUT_FILE, path));
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
