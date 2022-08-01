// parser

#include "others/header.h"

struct Parser
{
    Uyghur *uyghur;
    int position;
    Token *tokens;
    Token *token;
    Leaf *tree;
    Leaf *leaf;
};

void Parser_reset(Parser *this)
{
    this->position = 1;
    this->tokens = NULL;
    this->token = NULL;
    this->tree = NULL;
    this->leaf = NULL;
}

Parser *Parser_new(Uyghur *uyghur)
{
    Parser *parser = malloc(sizeof(Parser));
    parser->uyghur = uyghur;
    return parser;
}

void Parser_error(Parser *this, char *msg)
{
    Token *token = this->token;
    char *m = msg != NULL ? msg : LANG_ERR_NO_VALID_TOKEN;
    char *s = tools_format(LANG_ERR_TOKEN_PLACE, token->value, token->line, token->column, token->file);
    tools_error("%s%s", m, s);
}

void Parser_assert(Parser *this, bool value, char *msg)
{
    if (value == true)
        return;
    Parser_error(this, msg);
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
    else if (indent == 0)
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
    Leaf_pushLeaf(this->leaf, leaf);
}

void Parser_openBranch(Parser *this)
{
    Leaf *tail = (Leaf *)this->leaf->leafs->tail->data;
    this->leaf = tail;
}

void Parser_closeBranch(Parser *this)
{
    this->leaf = this->leaf->parent;
}

Token *Parser_checkType(Parser *this, int indent, int num, char *s, ...)
{
    Parser_moveToken(this, indent);
    Parser_assert(this, this->token != NULL, LANG_ERR_NO_VALID_TYPE);
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
    Parser_assert(this, isMatch, LANG_ERR_NO_VALID_TYPE);
    return this->token;
}

Token *Parser_checkValue(Parser *this, int indent, int num, char *s, ...)
{
    Parser_moveToken(this, indent);
    tools_assert(this->token != NULL, "keynidin sanliq melumat qimmiti [value] umut qilindi emma tepilmidi");
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
    Parser_assert(this, isMatch, NULL);
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
    return token != NULL && is_equal(token->type, tp);
}

bool Parser_isValue(Parser *this, int indent, char *value)
{
    Token *token = this->token;
    int count = indent > 0 ? indent : -indent;
    for (size_t i = 0; i < count; i++)
    {
        token = indent > 0 ? token->next : token->last;
    }
    return token != NULL && is_equal(token->value, value);
}

bool Parser_isWord(Parser *this, int indent, char *value)
{
    Token *token = this->token;
    int count = indent > 0 ? indent : -indent;
    for (size_t i = 0; i < count; i++)
    {
        token = indent > 0 ? token->next : token->last;
    }
    return token != NULL && is_equal(token->type, TTYPE_WORD) && is_equal(token->value, value);
}

Token *Parser_checkWord(Parser *this, int indent, int num, char *s, ...)
{
    Parser_moveToken(this, indent);
    Parser_assert(this, this->token != NULL, LANG_ERR_NO_VALID_WORD);
    Parser_assert(this, is_equal(this->token->type, TTYPE_WORD), LANG_ERR_NO_VALID_WORD);
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
    Parser_assert(this, isMatch, LANG_ERR_NO_VALID_WORD);
    return this->token;
}

void Parser_consumeAstVariable(Parser *this)
{
    Parser_checkWord(this, 0, 1, TVALUE_VARIABLE);
    Token *name = Parser_checkType(this, 1, 1, TTYPE_NAME);
    Parser_checkWord(this, 1, 1, TVALUE_VALUE);
    Token *action = Parser_checkType(this, 1, TTYPES_GROUP_DEFINE);
    Parser_checkWord(this, 1, 1, TVALUE_MADE);
    Parser_pushLeaf(this, ASTTYPE_VARIABLE, 2, name, action);
}

void Parser_consumeAstOperate(Parser *this)
{
    Token *target = Parser_checkWord(this, 0, TTYPES_GROUP_TARGETS);
    Token *name = NULL;
    Token *action = NULL;
    if (is_equal(this->token->value, TVALUE_TARGET_FROM))
    {
        name = Parser_checkType(this, 1, 2, TTYPE_NAME, TTYPE_KEY);
        action = Parser_checkWord(this, 1, 1, TVALUE_INPUT);
    }
    else if (is_equal(this->token->value, TVALUE_TARGET_TO))
    {
        name = Parser_checkType(this, 1, TTYPES_GROUP_DEFINE);
        action = Parser_checkWord(this, 1, 2, TVALUE_OUTPUT, TTYPE_KEY);
    }
    Parser_pushLeaf(this, ASTTYPE_OPERATE, 3, target, name, action);
}

void Parser_consumeAstExpression(Parser *this)
{
    Token *target = Parser_checkType(this, 0, 2, TTYPE_NAME, TTYPE_KEY);
    Parser_checkWord(this, 1, 1, TVALUE_VALUE);
    // single
    if (Parser_isWord(this, 2, TVALUE_MADE))
    {
        Token *source = NULL;
        if (Parser_isType(this, 1, TTYPE_WORD))
        {
            source = Parser_checkWord(this, 1, TVAUE_GROUP_EXP_SINGLE);
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
        Token *action = Parser_checkValue(this, 1, TVALUE_GROUP_EXP_STRING);
        Token *second = Parser_checkType(this, 1, 3, TTYPE_STRING, TTYPE_NAME, TTYPE_KEY);
        Parser_checkWord(this, 1, 1, TVALUE_MADE);
        Parser_pushLeaf(this, ASTTYPE_EXPRESSION_DOUBLE, 4, target, first, action, second);
        return;
    }
    // number
    if (Parser_isType(this, 1, TTYPE_NUMBER))
    {
        Token *first = Parser_checkType(this, 1, 1, TTYPE_NUMBER);
        Token *action = Parser_checkValue(this, 1, TVALUE_GROUP_EXP_NUMBER);
        Token *second = Parser_checkType(this, 1, 3, TTYPE_NUMBER, TTYPE_NAME, TTYPE_KEY);
        Parser_checkWord(this, 1, 1, TVALUE_MADE);
        Parser_pushLeaf(this, ASTTYPE_EXPRESSION_DOUBLE, 4, target, first, action, second);
        return;
    }
    // bool
    if (Parser_isType(this, 1, TTYPE_BOOL) || Parser_isType(this, 1, TTYPE_EMPTY))
    {
        Token *first = Parser_checkType(this, 1, 2, TTYPE_BOOL, TTYPE_EMPTY);
        Token *action = Parser_checkValue(this, 1, TVALUE_GROUP_EXP_BOOL);
        Token *second = Parser_checkType(this, 1, 4, TTYPE_BOOL, TTYPE_EMPTY, TTYPE_NAME, TTYPE_KEY);
        Parser_checkWord(this, 1, 1, TVALUE_MADE);
        Parser_pushLeaf(this, ASTTYPE_EXPRESSION_DOUBLE, 4, target, first, action, second);
        return;
    }
    // name
    if (Parser_isType(this, 1, TTYPE_NAME) || Parser_isType(this, 1, TTYPE_KEY))
    {
        Token *first = Parser_checkType(this, 1, 2, TTYPE_NAME, TTYPE_KEY);
        Token *action = NULL;
        Token *second = NULL;
        if (Parser_isType(this, 2, TTYPE_STRING))
        {
            action = Parser_checkValue(this, 1, TVALUE_GROUP_EXP_STRING);
            second = Parser_checkType(this, 1, 1, TTYPE_STRING);
        }
        else if (Parser_isType(this, 2, TTYPE_NUMBER))
        {
            action = Parser_checkValue(this, 1, TVALUE_GROUP_EXP_NUMBER);
            second = Parser_checkType(this, 1, 1, TTYPE_NUMBER);
        }
        else if (Parser_isType(this, 2, TTYPE_BOOL) || Parser_isType(this, 2, TTYPE_EMPTY))
        {
            action = Parser_checkValue(this, 1, TVALUE_GROUP_EXP_BOOL);
            second = Parser_checkType(this, 1, 2, TTYPE_BOOL, TTYPE_EMPTY);
        }
        else if (Parser_isType(this, 2, TTYPE_NAME) || Parser_isType(this, 2, TTYPE_KEY))
        {
            action = Parser_checkValue(this, 1, TVALUE_GROUP_EXP_ALL);
            second = Parser_checkType(this, 1, 2, TTYPE_NAME, TTYPE_KEY);
        }
        else if (Parser_isType(this, 2, TTYPE_BOX))
        {
            action = Parser_checkValue(this, 1, TVALUE_GROUP_EXP_CONTAINER);
            second = Parser_checkType(this, 1, 1, TTYPE_BOX);
        }
        else
        {
            Parser_error(this, NULL);
        }
        Parser_checkWord(this, 1, 1, TVALUE_MADE);
        Parser_pushLeaf(this, ASTTYPE_EXPRESSION_DOUBLE, 4, target, first, action, second);
        return;
    }
    // box
    if (Parser_isType(this, 1, TTYPE_BOX))
    {
        Token *first = Parser_checkType(this, 1, 1, TTYPE_BOX);
        Token *action = Parser_checkValue(this, 1, TVALUE_GROUP_EXP_CONTAINER);
        Token *second = Parser_checkType(this, 1, 3, TTYPE_BOX, TTYPE_NAME, TTYPE_KEY);
        Parser_checkWord(this, 1, 1, TVALUE_MADE);
        Parser_pushLeaf(this, ASTTYPE_EXPRESSION_DOUBLE, 4, target, first, action, second);
        return;
    }
    Parser_error(this, NULL);
}

void Parser_consumeAstIfFirst(Parser *this)
{
    // open ASTTYPE_IF
    Parser_pushLeaf(this, ASTTYPE_IF, 0, NULL);
    Parser_openBranch(this);
    // open ASTTYPE_IF_FIRST
    Parser_checkWord(this, 0, 1, TVALUE_IF);
    Token *name = Parser_checkType(this, 1, TTYPES_GROUP_DEFINE);
    Parser_checkWord(this, 1, 1, TVALUE_VALUE);
    Token *value = Parser_checkType(this, 1, TTYPES_GROUP_DEFINE);
    Token *action = Parser_checkWord(this, 1, 2, TVALUE_IF_OK, TVALUE_IF_NO);
    Parser_pushLeaf(this, ASTTYPE_IF_FIRST, 3, name, value, action);
    Parser_openBranch(this);
}

void Parser_consumeAstIfMiddle(Parser *this)
{
    // close ASTTYPE_IF_FIRST or ASTTYPE_IF_MIDDLE
    Token *token = this->token;
    char *curType = this->leaf->type;
    Parser_assert(this, is_equal(curType, ASTTYPE_IF_FIRST) || is_equal(curType, ASTTYPE_IF_MIDDLE), LANG_ERR_NO_IF_FIRST);
    Parser_closeBranch(this);
    //  open ASTTYPE_IF_MIDDLE
    Parser_checkWord(this, 0, 1, TVALUE_IF_ELSE);
    Token *name = Parser_checkType(this, 1, TTYPES_GROUP_DEFINE);
    Parser_checkWord(this, 1, 1, TVALUE_VALUE);
    Token *value = Parser_checkType(this, 1, TTYPES_GROUP_DEFINE);
    Token *action = Parser_checkWord(this, 1, 2, TVALUE_IF_OK, TVALUE_IF_NO);
    Parser_pushLeaf(this, ASTTYPE_IF_MIDDLE, 3, name, value, action);
    Parser_openBranch(this);
}

void Parser_consumeAstIfLast(Parser *this)
{
    // close ASTTYPE_IF_FIRST or ASTTYPE_IF_MIDDLE
    char *curType = this->leaf->type;
    Parser_assert(this, is_equal(curType, ASTTYPE_IF_FIRST) || is_equal(curType, ASTTYPE_IF_MIDDLE), LANG_ERR_NO_IF_FIRST);
    Parser_closeBranch(this);
    // open ASTTYPE_IF_LAST
    Token *token = Parser_checkWord(this, 0, 1, TVALUE_IF_NO);
    Parser_pushLeaf(this, ASTTYPE_IF_LAST, 1, token);
    Parser_openBranch(this);
}

void Parser_consumeAstEnd(Parser *this)
{
    // close ASTTYPE_IF_FIRST or ASTTYPE_IF_MIDDLE or ASTTYPE_IF_LAST
    char *curType = this->leaf->type;
    if (
        is_equal(curType, ASTTYPE_IF_FIRST)
        || is_equal(curType, ASTTYPE_IF_MIDDLE)
        || is_equal(curType, ASTTYPE_IF_LAST)
        || is_equal(curType, ASTTYPE_CODE))
    {
        Parser_closeBranch(this);
    }
    // close ast code
    Parser_checkWord(this, 0, 1, TVALUE_CODE_END);
    Parser_pushLeaf(this, ASTTYPE_END, 0, NULL);
    Parser_closeBranch(this);
}

void Parser_consumeAstWhile(Parser *this)
{
    Parser_checkWord(this, 0, 1, TVALUE_WHILE);
    Token *name = Parser_checkType(this, 1, TTYPES_GROUP_DEFINE);
    Parser_checkWord(this, 1, 1, TVALUE_VALUE);
    Token *value = Parser_checkType(this, 1, TTYPES_GROUP_DEFINE);
    Token *action = Parser_checkWord(this, 1, 2, TVALUE_IF_OK, TVALUE_IF_NO);
    Parser_pushLeaf(this, ASTTYPE_WHILE, 3, name, value, action);
    Parser_openBranch(this);
}

void Parser_consumeAstResult(Parser *this)
{
    Parser_checkWord(this, 0, 1, TVALUE_RESULT);
    Token *name = Parser_checkType(this, 1, TTYPES_GROUP_DEFINE);
    Parser_checkWord(this, 1, 1, TVALUE_RETURN);
    Parser_pushLeaf(this, ASTTYPE_RESULT, 1, name);
}

void Parser_consumeAstFunc(Parser *this)
{
    Parser_checkWord(this, 0, 1, TVALUE_FUNCTION);
    Token *name = Parser_checkType(this, 1, 2, TTYPE_NAME, TTYPE_KEY);
    Leaf *leaf = Leaf_new(ASTTYPE_FUNC);
    Leaf *code = Leaf_new(ASTTYPE_CODE);
        // args
    if (Parser_isValue(this, 1, TVALUE_VARIABLE))
    {
        Parser_checkValue(this, 1, 1, TVALUE_VARIABLE);
        Token *variable = Parser_checkType(this, 1, 2, TTYPE_NAME, TTYPE_KEY);
        while (variable != NULL)
        {
            Stack_push(code->tokens, variable);
            variable = Parser_isValue(this, 1, TVALUE_CONTENT) ? NULL : Parser_checkType(this, 1, 2, TTYPE_NAME, TTYPE_KEY);
        }
    }
    // finish
    Parser_checkValue(this, 1, 1, TVALUE_CONTENT);
    Stack_push(leaf->tokens, name);
    Stack_push(code->tokens, name);
    Leaf_pushLeaf(this->leaf, leaf);
    Parser_openBranch(this);
    Leaf_pushLeaf(this->leaf, code);
    Parser_openBranch(this);
}

void Parser_consumeAstCall(Parser *this)
{
    Parser_checkWord(this, 0, 1, TVALUE_FUNCTION);
    Token *name = Parser_checkType(this, 1, 2, TTYPE_NAME, TTYPE_KEY);
    Leaf *leaf = Leaf_new(ASTTYPE_CALL);
    // args
    if (Parser_isValue(this, 1, TVALUE_WITH))
    {
        Parser_checkValue(this, 1, 1, TVALUE_WITH);
        Token *variable = Parser_checkType(this, 1, TTYPES_GROUP_DEFINE);
        while (variable != NULL)
        {
            Stack_push(leaf->tokens, variable);
            variable = Parser_isValue(this, 1, TVALUE_CALL) ? NULL : Parser_checkType(this, 1, TTYPES_GROUP_DEFINE);
        }
    }
    //
    Parser_checkWord(this, 1, 1, TVALUE_CALL);
    // result
    Token *result = Token_empty();
    if (Parser_isValue(this, 1, TVALUE_FURTHER))
    {
        Parser_checkValue(this, 1, 1, TVALUE_FURTHER);
        Parser_checkWord(this, 1, 1, TVALUE_RESULT);
        result = Parser_checkType(this, 1, 2, TTYPE_NAME, TTYPE_KEY);
        Parser_checkWord(this, 1, 1, TVALUE_MADE);
    }
    Stack_push(leaf->tokens, result);
    //
    Stack_push(leaf->tokens, name);
    Leaf_pushLeaf(this->leaf, leaf);
}

void Parser_consumeAstCalculator(Parser *this)
{
    Token *target = Parser_checkType(this, 0, 2, TTYPE_NAME, TTYPE_KEY);
    Parser_checkWord(this, 1, 1, TVALUE_CALCULATOR);
    Token *tempT = NULL;
    Foliage *tempF = NULL;
    Foliage *current = NULL;
    Foliage *root = Foliage_new(NULL);
    Stack *currents = Stack_new();
    Stack_push(currents, root);
    char *lastType = NULL;
    while (true) {
        current = (Foliage*)currents->tail->data;
        if (Parser_isType(this, 1, TTYPE_NAME) || Parser_isType(this, 1, TTYPE_KEY) || Parser_isType(this, 1, TTYPE_NUMBER)) {
            tempT = Parser_checkType(this, 1, TTYPES_GROUP_NUMBER);
            tempF = Foliage_new(tempT);
            if (lastType == NULL) {
                current->left = tempF;
            } else if (is_values(lastType, 1, TTYPE_CALCULATION)) {
                current->right = tempF;
            } else {
                Parser_error(this, "invalid argument for calculator in parser:");
            }
            lastType = tempT->type;
            continue;
        } else if (Parser_isType(this, 1, TTYPE_CALCULATION)) {
            Parser_assert(this, lastType == NULL || is_values(lastType, TTYPES_GROUP_NUMBER), "invalid calculator order in parser: ");
            tempT = Parser_checkType(this, 1, 1, TTYPE_CALCULATION);
            Parser_assert(this, current->data == NULL, "brackets expected for multiple calculation:");
            current->data = tempT;
            lastType = tempT->type;
            continue;
        } else if (Parser_isWord(this, 1, TVALUE_OPEN)) {
            Parser_checkWord(this, 1, 1, TVALUE_OPEN);
            tempF = Foliage_new(NULL);
            if (lastType == NULL) {
                current->left = tempF;
            } else if (is_values(lastType, 1, TTYPE_CALCULATION)) {
                current->right = tempF;
            } else {
                Parser_error(this, "invalid open for calculator in parser:");
            }
            Stack_push(currents, tempF);
            lastType = NULL;
            continue;
        } else if (Parser_isWord(this, 1, TVALUE_CLOSE)) {
            Parser_assert(this, lastType == NULL || is_values(lastType, TTYPES_GROUP_NUMBER), "invalid calculator order in parser: ");
            Parser_checkWord(this, 1, 1, TVALUE_CLOSE);
            Stack_pop(currents);
            lastType = NULL;
            continue;
        }
        break;
    }
    current = (Foliage*)currents->tail->data;
    Parser_assert(this, root == current, "invalid calculator tree status:");
    Token *body = Token_new(TTYPE_CALCULATION, root);
    Parser_pushLeaf(this, ASTTYPE_CALCULATOR, 2, target, body);
}

void Parser_consumeToken(Parser *this, Token *token)
{
    //
    char *t = token->type;
    char *v = token->value;
    // VARIABLE
    if (is_equal(t, TTYPE_WORD) && is_equal(v, TVALUE_VARIABLE))
    {
        Parser_consumeAstVariable(this);
        return;
    }
    // END
    if (is_equal(v, TVALUE_CODE_END))
    {
        Parser_consumeAstEnd(this);
        return;
    }
    // RESULT
    if (is_equal(v, TVALUE_RESULT))
    {
        Parser_consumeAstResult(this);
        return;
    }
    // FUNC
    if (is_equal(v, TVALUE_FUNCTION) && (Parser_isValue(this, 2, TVALUE_VARIABLE) || Parser_isValue(this, 2, TVALUE_CONTENT)))
    {
        Parser_consumeAstFunc(this);
        return;
    }
    // CALL
    if (is_equal(v, TVALUE_FUNCTION) && (Parser_isValue(this, 2, TVALUE_WITH) || Parser_isValue(this, 2, TVALUE_CALL)))
    {
        Parser_consumeAstCall(this);
        return;
    }
    // IF_FIRST
    if (is_equal(v, TVALUE_IF))
    {
        Parser_consumeAstIfFirst(this);
        return;
    }
    // IF_MIDDLE
    if (is_equal(v, TVALUE_IF_ELSE))
    {
        Parser_consumeAstIfMiddle(this);
        return;
    }
    // IF_LAST
    if (is_equal(v, TVALUE_IF_NO))
    {
        Parser_consumeAstIfLast(this);
        return;
    }
    // WHILE
    if (is_equal(v, TVALUE_WHILE))
    {
        Parser_consumeAstWhile(this);
        return;
    }
    // EXPRESSION
    if ((is_equal(t, TTYPE_NAME) || is_equal(t, TTYPE_KEY)) && Parser_isWord(this, 1, TVALUE_VALUE))
    {
        Parser_consumeAstExpression(this);
        return;
    }
    // calculate
    if ((is_equal(t, TTYPE_NAME) || is_equal(t, TTYPE_KEY)) && Parser_isWord(this, 1, TVALUE_CALCULATOR))
    {
        Parser_consumeAstCalculator(this);
        return;
    }
    // OPERATE
    if (is_equal(t, TTYPE_WORD) && (is_equal(v, TVALUE_TARGET_FROM) || is_equal(v, TVALUE_TARGET_TO)))
    {
        Parser_consumeAstOperate(this);
        return;
    }
    //
    Parser_error(this, NULL);
    Token_print(token);
}

Leaf *Parser_parseTokens(Parser *this, Token *tokens)
{
    Parser_reset(this);
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
    this->uyghur = NULL;
    Parser_reset(this);
    free(this);
}
