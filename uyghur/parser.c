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

void Parser_error(Parser *this)
{
    Token *token = this->token;
    printf("parser exception at line:%d, column:%d in file %s", token->line, token->column, token->file);
}

void Parser_assert(Parser *this, bool value)
{
    if (value == true) return;
    Parser_error(this);
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

void Parser_pushLeaf(Parser *this, char *tp, int num, Token *token, ...)
{
    Leaf *leaf = Leaf_new(tp);
    va_list valist;
    int i;
    va_start(valist, token);
    for (i = 0; i < num; i++)
    {
        Stack_push(leaf->tokens, token);
       token = va_arg(valist, Token *);
    }
    va_end(valist);
    Queue_push(this->leaf->leafs, leaf);
}

Token *Parser_checkType(Parser *this, int indent, int num, char *s, ...)
{
    Parser_moveToken(this, indent);
    tools_check(this->token != NULL, "keynidin sanliq melumat tipi [type] umut qilindi emma tepilmidi");
    bool isMatch = false;
    va_list valist;
    int i;
    va_start(valist, s);
    for (i = 0; i < num; i++)
    {
        if (is_equal(this->token->type, s))
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

Token *Parser_checkValue(Parser *this, int indent, int num, char *s, ...)
{
    Parser_moveToken(this, indent);
    tools_check(this->token != NULL, "keynidin sanliq melumat qimmiti [value] umut qilindi emma tepilmidi");
    bool isMatch = false;
    va_list valist;
    int i;
    va_start(valist, s);
    for (i = 0; i < num; i++)
    {
        if (is_equal(this->token->value, s))
        {
            isMatch = true;
            break;
        }
       s = va_arg(valist, char *);
    }
    va_end(valist);
    tools_check(isMatch, "keynidin sanliq melumat qimmiti [value] umut qilindi emma [value] tepildi");
    return this->token;
}

bool Parser_isType(Parser *this, int indent, char *tp)
{
    Token *token = this->token;
    int count = indent > 0 ? indent : -indent;
    for (size_t i = 0; i < count; i++)
    {
        token = indent > 0 ? token->next : token->last;
    }
    return is_equal(token->type, tp);
}

bool Parser_isValue(Parser *this, int indent, char *value)
{
    Token *token = this->token;
    int count = indent > 0 ? indent : -indent;
    for (size_t i = 0; i < count; i++)
    {
        token = indent > 0 ? token->next : token->last;
    }
    return is_equal(token->value, value);
}


bool Parser_isWord(Parser *this, int indent, char *value)
{
    Token *token = this->token;
    int count = indent > 0 ? indent : -indent;
    for (size_t i = 0; i < count; i++)
    {
        token = indent > 0 ? token->next : token->last;
    }
    return is_equal(token->type, TTYPE_WORD) && is_equal(token->value, value);
}

Token *Parser_checkWord(Parser *this, int indent, int num, char *s, ...)
{
    Parser_moveToken(this, indent);
    tools_check(this->token != NULL, "keynidin xalqiliq soz [value] umut qilindi emma tepilmidi");
    tools_check(is_equal(this->token->type, TTYPE_WORD), "keynidin xalqiliq soz [value] umut qilindi emma [value] tepildi");
    bool isMatch = false;
    va_list valist;
    int i;
    va_start(valist, s);
    for (i = 0; i < num; i++)
    {
        if (is_equal(this->token->value, s))
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
    Token *name = Parser_checkType(this, 1, 1, TTYPE_NAME);
    Parser_checkWord(this, 1, 1, TVALUE_VALUE);
    Token *value = Parser_checkType(this, 1, TTYPES_GROUP_DEFINE);
    Parser_checkWord(this, 1, 1, TVALUE_MADE);
    Parser_pushLeaf(this, ASTTYPE_VARIABLE, 2, name, value);
}

void Parser_consumeAstOperate(Parser *this)
{
    Token *target = Parser_checkWord(this, 0, TTYPES_GROUP_TARGETS);
    Token *name = NULL;
    Token *action = NULL;
    if (is_equal(this->token->value, TVALUE_TARGET_FROM))
    {
        name = Parser_checkType(this, 1, 1, TTYPE_NAME);
        action = Parser_checkWord(this, 1, 1, TVALUE_INPUT);
    }
    else if (is_equal(this->token->value, TVALUE_TARGET_TO))
    {
        name = Parser_checkType(this, 1, TTYPES_GROUP_DEFINE);
        action = Parser_checkWord(this, 1, 1, TVALUE_OUTPUT);
    }
    Parser_pushLeaf(this, ASTTYPE_OPERATE, 3, target, name, action);
}

void Parser_consumeAstExpression(Parser *this)
{
    Token *target = Parser_checkType(this, 0, 1, TTYPE_NAME);
    Parser_checkWord(this, 1, 1, TVALUE_VALUE);
    // single
    if (Parser_isWord(this, 2, TVALUE_MADE))
    {
        Token *source = NULL;
        if (Parser_isWord(this, 1, TVALUE_NOT))
        {
            source = Parser_checkWord(this, 1, 1, TVALUE_NOT);
        }
        else
        {
            source = Parser_checkType(this, 1, TTYPES_GROUP_DEFINE);
        }
        
        Parser_checkWord(this, 1, 1, TVALUE_MADE);
        Parser_pushLeaf(this, ASTTYPE_EXPRESSION_SINGLE, 2, target, source);
        return;
    }
    // string
    if (Parser_isType(this, 1, TTYPE_STRING))
    {
        Token *first = Parser_checkType(this, 1, 1, TTYPE_STRING);
        Token *action = Parser_checkValue(this,1, TVALUE_GROUP_EXP_STRING);
        Token *second = Parser_checkType(this, 1, 2, TTYPE_STRING, TTYPE_NAME);
        Parser_checkWord(this, 1, 1, TVALUE_MADE);
        Parser_pushLeaf(this, ASTTYPE_EXPRESSION_DOUBLE, 4, target, first, action, second);
        return;
    }
    // number
    if (Parser_isType(this, 1, TTYPE_NUMBER))
    {
        Token *first = Parser_checkType(this, 1, 1, TTYPE_NUMBER);
        Token *action = Parser_checkValue(this,1, TVALUE_GROUP_EXP_NUMBER);
        Token *second = Parser_checkType(this, 1, 2, TTYPE_NUMBER, TTYPE_NAME);
        Parser_checkWord(this, 1, 1, TVALUE_MADE);
        Parser_pushLeaf(this, ASTTYPE_EXPRESSION_DOUBLE, 4, target, first, action, second);
        return;
    }
    // bool
    if (Parser_isType(this, 1, TTYPE_BOOL) || Parser_isType(this, 1, TTYPE_EMPTY))
    {
        Token *first = Parser_checkType(this, 1, 2, TTYPE_BOOL, TTYPE_EMPTY);
        Token *action = Parser_checkValue(this,1, TVALUE_GROUP_EXP_BOOL);
        Token *second = Parser_checkType(this, 1, 3, TTYPE_BOOL, TTYPE_EMPTY, TTYPE_NAME);
        Parser_checkWord(this, 1, 1, TVALUE_MADE);
        Parser_pushLeaf(this, ASTTYPE_EXPRESSION_DOUBLE, 4, target, first, action, second);
        return;
    }
    // name
    if (Parser_isType(this, 1, TTYPE_NAME))
    {
        Token *first = Parser_checkType(this, 1, 1, TTYPE_NAME);
        Token *action = NULL;
        Token *second = NULL;
        if (Parser_isType(this, 2, TTYPE_STRING))
        {
            action = Parser_checkValue(this,1, TVALUE_GROUP_EXP_STRING);
            second = Parser_checkType(this, 1, 1, TTYPE_STRING);
        }
        else if (Parser_isType(this, 2, TTYPE_NUMBER))
        {
            action = Parser_checkValue(this,1, TVALUE_GROUP_EXP_NUMBER);
            second = Parser_checkType(this, 1, 1, TTYPE_NUMBER);
        }
        else if (Parser_isType(this, 2, TTYPE_BOOL) || Parser_isType(this, 2, TTYPE_EMPTY))
        {
            action = Parser_checkValue(this,1, TVALUE_GROUP_EXP_BOOL);
            second = Parser_checkType(this, 1, 2, TTYPE_BOOL, TTYPE_EMPTY);
        }
        else if (Parser_isType(this, 2, TTYPE_NAME))
        {
            action = Parser_checkValue(this,1, TVALUE_GROUP_EXP_ALL);
            second = Parser_checkType(this, 1, 1, TTYPE_NAME);
        }
        else
        {
            Parser_error(this);
        }
        Parser_checkWord(this, 1, 1, TVALUE_MADE);
        Parser_pushLeaf(this, ASTTYPE_EXPRESSION_DOUBLE, 4, target, first, action, second);
        return;
    }
    Parser_error(this);
}

void Parser_consumeToken(Parser *this, Token *token)
{
    //
    char *t = token->type;
    char *v = token->value;
    // VARIABLE
    if (t == TTYPE_WORD && is_equal(v, TVALUE_VARIABLE))
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
    // EXPRESSION, EXPRESSION_NUMBER, EXPRESSION_LOGIC
    if (t == TTYPE_NAME && Parser_isWord(this, 1, TVALUE_VALUE))
    {
        Parser_consumeAstExpression(this);
        return;
    }
    // OPERATE
    if (t == TTYPE_WORD && (is_equal(v, TVALUE_TARGET_FROM) || is_equal(v, TVALUE_TARGET_TO)))
    {
        Parser_consumeAstOperate(this);
        return;
    }
    //
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
