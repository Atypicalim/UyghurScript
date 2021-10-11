// parser

#include "header.h"
#include "token.c"
#include "leaf.c"

typedef struct
{
    int position;
    Token *tokens;
    Token *token;
    Leaf *tree;
    Leaf *leaf;
} Parser;

void Parser_reset(Parser *this)
{
    this->position = 1;
    this->tokens = NULL;
    this->token = NULL;
    this->tree = NULL;
    this->leaf = NULL;
}

Parser *Parser_new()
{
    Parser *parser = malloc(sizeof(Parser));
    Parser_reset(parser);
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

Token *Parser_checkToken(Parser *this, char *tp)
{
    tools_check(this->token != NULL, "keynidin sanliq melumat tipi [type] umut qilindi emma tepilmidi");
    tools_check(strcmp(this->token->type, tp) == 0, "keynidin sanliq melumat tipi [type] umut qilindi emma [type] tepildi");
    return this->token;
}

Token *Parser_nextToken(Parser *this, char *tp)
{
    this->token = this->token->next;
    return Parser_checkToken(this, tp);
}

Token *Parser_lastToken(Parser *this, char *tp)
{
    this->token = this->token->last;
    return Parser_checkToken(this, tp);
}

Token *Parser_checkWord(Parser *this, char *value)
{
    tools_check(this->token != NULL, "keynidin xalqiliq soz [value] umut qilindi emma tepilmidi");
    
    tools_check(strcmp(this->token->type, TTYPE_WORD) == 0, "keynidin xalqiliq soz [value] umut qilindi emma [value] tepildi");
    tools_check(strcmp(this->token->value, value) == 0, "keynidin xalqiliq soz [value] umut qilindi emma [value] tepildi");
    return this->token;
}

Token *Parser_nextWord(Parser *this, char *value)
{
    this->token = this->token->next;
    return Parser_checkWord(this, value);
}

Token *Parser_lastWord(Parser *this, char *value)
{
    this->token = this->token->last;
    return Parser_checkWord(this, value);
}

void Parser_consumeAstVariable(Parser *this)
{
    Parser_checkWord(this, TVALUE_VARIABLE);
    Token *name = Parser_nextToken(this, TTYPE_NAME);
    Parser_nextWord(this, TVALUE_VALUE);
    Token *value = Parser_nextToken(this, TTYPE_NUMBER); // TODO: suppor multi data type check
    Parser_nextWord(this, TVALUE_MADE);
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
