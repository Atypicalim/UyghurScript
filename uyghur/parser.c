// parser

#ifndef HEAD_HEADER
#include "header.h"
#endif

#ifndef HEAD_TOKEN
#include "token.c"
#endif

#ifndef HEAD_LEAF
#include "leaf.c"
#endif

typedef struct
{
    int position;
    Token *head;
    // TODO: tree
    // tree -> leaf(ASTTYPE_PROGRAM)
    // current -> root
} Parser;

void Parser_reset(Parser *this)
{
    this->position = 1;
    this->head = NULL;
}

Parser *Parser_new()
{
    Parser *parser = malloc(sizeof(Parser));
    //
    return parser;
}

void *Parser_parseTokens(Parser *this, Token *head)
{
    this->head = head;
    Token *current = this->head;
    while (current != NULL)
    {
        // TODO:
        Token_print(current);
        current = current->next;
    }
    void *astNode = NULL;
    return astNode;
}

void Parser_free(Parser *this)
{
    Parser_reset(this);
    free(this);
}
