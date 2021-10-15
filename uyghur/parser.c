// parser

#include "header.h"
#include "token.c"
#include "leaf.c"

struct Parser {
    Uyghur *uyghur;
    int position;
    Token *tokens;
    Token *token;
    Leaf *tree;
    Leaf *leaf;
};

void Parser_reset(Parser *this)
{
    this->uyghur = NULL;
    this->position = 1;
    this->tokens = NULL;
    this->token = NULL;
    this->tree = NULL;
    this->leaf = NULL;
}

Parser *Parser_new(Uyghur *uyghur)
{
    Parser *parser = malloc(sizeof(Parser));
    Parser_reset(parser);
    parser->uyghur = uyghur;
    return parser;
}

void Parser_asert(bool value, Token *token)
{
    if (value == true) return;
    if (token == NULL)
    {
        /* code */
    }
    
    tools_error("parser exception...");
}

void Parser_moveToken(Parser *this, int indent)
{
    if (indent == 1)
    {
        this->token = this->token->next;
    }
    else if (indent == -1)
    {
        this->token = this->token->last;
    }
    else if(indent == 0)
    {
        //
    }
    else
    {
        tools_error("invalid indent for parser");
    }
}

Token *Parser_checkToken(Parser *this, int indent, int num, char *s, ...)
{
    Parser_moveToken(this, indent);
    tools_check(this->token != NULL, "keynidin sanliq melumat tipi [type] umut qilindi emma tepilmidi");
    bool isMatch = false;
    va_list valist;
    int i;
    va_start(valist, s);
    for (i = 0; i < num; i++)
    {
        if (strcmp(this->token->type, s) == 0)
        {
            isMatch = true;
            break;
        }
       s = va_arg(valist, char *);
    }
    va_end(valist);
    tools_check(isMatch, "keynidin sanliq melumat tipi [type] umut qilindi emma [type] tepildi");
    return this->token;
}

Token *Parser_checkWord(Parser *this, int indent, int num, char *s, ...)
{
    Parser_moveToken(this, indent);
    tools_check(this->token != NULL, "keynidin xalqiliq soz [value] umut qilindi emma tepilmidi");
    tools_check(strcmp(this->token->type, TTYPE_WORD) == 0, "keynidin xalqiliq soz [value] umut qilindi emma [value] tepildi");
    bool isMatch = false;
    va_list valist;
    int i;
    va_start(valist, s);
    for (i = 0; i < num; i++)
    {
        if (strcmp(this->token->value, s) == 0)
        {
            isMatch = true;
            break;
        }
       s = va_arg(valist, char *);
    }
    va_end(valist);
    tools_check(isMatch, "keynidin xalqiliq soz [value] umut qilindi emma [value] tepildi");
    return this->token;
}

void Parser_consumeAstVariable(Parser *this)
{
    Parser_checkWord(this, 0, 1, TVALUE_VARIABLE);
    Token *name = Parser_checkToken(this, 1, 1, TTYPE_NAME);
    Parser_checkWord(this, 1, 1, TVALUE_VALUE);
    Token *value = Parser_checkToken(this, 1, TTYPES_GROUP_DEFINE);
    Parser_checkWord(this, 1, 1, TVALUE_MADE);
    Leaf *leaf = Leaf_new(ASTTYPE_VARIABLE);
    Stack_push(leaf->tokens, name);
    Stack_push(leaf->tokens, value);
    Queue_push(this->leaf->leafs, leaf);
}

void Parser_consumeToken(Parser *this, Token *token)
{
    
    // VARIABLE
    if (token->type == TTYPE_WORD && strcmp(token->value, TVALUE_VARIABLE) == 0)
    {
        Parser_consumeAstVariable(this);
        return;
    }
    // END
    // ASSIGN
    // TRANSFORM
    // RESULT
    // FUNC
    // CALL
    // IF
    // IF_FIRST
    // IF_MIDDLE
    // IF_LAST
    // WHILE
    // EXPRESSION
    // EXPRESSION_NUMBER
    // EXPRESSION_LOGIC
    // OPERATE
    Token_print(token);
}

Leaf *Parser_parseTokens(Parser *this, Token *tokens)
{
    this->tokens = tokens;
    this->tree = Leaf_new(ASTTYPE_PROGRAM);
    this->leaf = this->tree;
    //
    this->token = this->tokens;
    while (this->token != NULL)
    {
        Parser_consumeToken(this, this->token);
        this->token = this->token->next;
    }
    return this->tree;
}

void Parser_free(Parser *this)
{
    Parser_reset(this);
    free(this);
}
