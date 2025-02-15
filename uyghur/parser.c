// parser

#include "others/header.h"

void Parser_reset(Parser *this)
{
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
    char *m = msg != NULL ? msg : LANG_ERR_PARSER_EXCEPTION;
    char *s = tools_format(LANG_ERR_TOKEN_PLACE, token->file, token->line, token->column, token->value);
    log_error("Parser: %s, %s", m, s);
    exit(1);
}

void Parser_assert(Parser *this, bool value, char *msg)
{
    if (value == true)
        return;
    Parser_error(this, msg);
}

Token *Parser_moveToken(Parser *this, int indent)
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
    return this->token;
}

void Parser_pushLeaf(Parser *this, char tp, int num, Token *token, ...)
{
    log_debug("parser.ast: %c", tp);
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



Token *Parser_getToken(Parser *this, int indent)
{
    Token *token = this->token;
    int count = indent > 0 ? indent : -indent;
    for (size_t i = 0; i < count; i++)
    {
        if (token != NULL) {
            token = indent > 0 ? token->next : token->last;
        }
    }
    return token;
}

bool Parser_isType(Parser *this, int indent, char *tp)
{
    Token *token = Parser_getToken(this, indent);
    return token != NULL && is_eq_string(token->type, tp);
}

bool Parser_isValue(Parser *this, int indent, char *value)
{
    Token *token = Parser_getToken(this, indent);
    return token != NULL && is_eq_string(token->value, value);
}

bool Parser_isWord(Parser *this, int indent, char *value)
{
    Token *token = Parser_getToken(this, indent);
    return token != NULL && is_eq_string(token->type, UG_TTYPE_WRD) && is_eq_string(token->value, value);
}

bool Parser_isTypes(Parser *this, int indent, int num, char *s, ...)
{
    Token *token = Parser_getToken(this, indent);
    va_list valist;
    va_start(valist, s);
    return  token != NULL && helper_token_is_types_list(token, num, s, valist);
}

bool Parser_isValues(Parser *this, int indent, int num, char *s, ...)
{
    Token *token = Parser_getToken(this, indent);
    va_list valist;
    va_start(valist, s);
    return token != NULL && helper_token_is_values_list(token, num, s, valist);
}

bool Parser_isWords(Parser *this, int indent, int num, char *s, ...)
{
    Token *token = Parser_getToken(this, indent);
    va_list valist;
    va_start(valist, s);
    return token != NULL && is_eq_string(token->type, UG_TTYPE_WRD) && helper_token_is_values_list(token, num, s, valist);
}

Token *Parser_checkType(Parser *this, int indent, int num, char *s, ...)
{
    Token *token = Parser_moveToken(this, indent);
    Parser_assert(this, token != NULL, LANG_ERR_INVALID_TYPE);
    va_list valist;
    va_start(valist, s);
    bool isMatch = helper_token_is_types_list(token, num, s, valist);
    Parser_assert(this, isMatch, LANG_ERR_INVALID_TYPE);
    return token;
}

Token *Parser_checkValue(Parser *this, int indent, int num, char *s, ...)
{
    Token *token = Parser_moveToken(this, indent);
    Parser_assert(this, token != NULL, LANG_ERR_PARSER_INVALID_VARIABLE);
    va_list valist;
    va_start(valist, s);
    bool isMatch = helper_token_is_values_list(token, num, s, valist);
    Parser_assert(this, isMatch, LANG_ERR_PARSER_INVALID_VARIABLE);
    return token;
}

Token *Parser_checkWord(Parser *this, int indent, int num, char *s, ...)
{
    Token *token = Parser_moveToken(this, indent);
    Parser_assert(this, token != NULL, LANG_ERR_INVALID_WORD);
    Parser_assert(this, is_eq_string(token->type, UG_TTYPE_WRD), LANG_ERR_INVALID_WORD);
    va_list valist;
    va_start(valist, s);
    bool isMatch = helper_token_is_values_list(token, num, s, valist);
    Parser_assert(this, isMatch, LANG_ERR_INVALID_WORD);
    return token;
}

// TODO:relace calculation word
void Parser_consumeAstVariable(Parser *this)
{
    Parser_checkWord(this, 0, 1, LETTER_VARIABLE);
    Token *name = Parser_checkType(this, 1, 1, UG_TTYPE_NAM);
    Parser_checkWord(this, 1, 1, LETTER_VALUE);
    Token *token = Parser_moveToken(this, 1); 
    if (!helper_token_is_values(token, TVAUES_GROUP_UTYPES) && !helper_token_is_types(token, TTYPES_GROUP_VALUES)) {
        Parser_error(this, LANG_ERR_INVALID_TYPE);
    }
    Parser_checkWord(this, 1, 1, LETTER_MADE);
    Parser_pushLeaf(this, UG_ATYPE_VAR, 2, name, token);
}

void Parser_consumeAstCommand(Parser *this)
{
    Token *target = Parser_checkWord(this, 0, 1, LETTER_COMMAND);
    Queue *args = Queue_new(false);
    Token *token = Parser_moveToken(this, 1);
    while (token != NULL && !is_command_action(token->value)) {
        Queue_push(args, token);
        token = Parser_moveToken(this, 1);
    }
    Parser_assert(this, token != NULL && is_command_action(token->value), LANG_ERR_PARSER_EXCEPTION);
    Token *action = token;
    //
    if (action->value == LETTER_CMD_INPUT) {
        Parser_assert(this, args->size >= 1, LANG_ERR_PARSER_EXCEPTION);
        Queue_RESTE(args);
        Token *arg = Queue_NEXT(args);
        while (args) {
            if (
                !helper_token_is_values(arg, TVAUES_GROUP_UTYPES)
                && !helper_token_is_types(arg, TTYPES_GROUP_VALUES)
                && !helper_token_is_values(arg, TVAUES_GROUP_NICKNAME)
            ) {
                Parser_error(this, LANG_ERR_INVALID_TYPE);
            }
            args = Queue_NEXT(args);
        }
    }
    //
    if (action->value == LETTER_CMD_INPUT) {
        Parser_assert(this, args->size >= 1, LANG_ERR_PARSER_EXCEPTION);
        Queue_RESTE(args);
        Token *arg = Queue_NEXT(args);
        while (args) {
            if (!helper_token_is_types(arg, TVAUES_GROUP_CHANGEABLE)) {
                Parser_error(this, LANG_ERR_INVALID_TYPE);
            }
            args = Queue_NEXT(args);
        }
    }
    // 
    action->extra = args;
    Parser_pushLeaf(this, UG_ATYPE_CMD, 2, target, action);
}

void Parser_consumeAstConvert(Parser *this)
{
    Token *target = Parser_checkType(this, 0, TVAUES_GROUP_CHANGEABLE);
    Parser_checkWord(this, 1, 1, LETTER_VALUE);
    // single
    if (Parser_isWord(this, 2, LETTER_MADE))
    {
        Token *source = NULL;
        if (Parser_isType(this, 1, UG_TTYPE_WRD))
        {
            source = Parser_checkWord(this, 1, TVAUES_GROUP_CONVERT);
        }
        else
        {
            source = Parser_checkType(this, 1, TTYPES_GROUP_VALUES);
        }
        Parser_checkWord(this, 1, 1, LETTER_MADE);
        Parser_pushLeaf(this, UG_ATYPE_CVT, 2, target, source);
        return;
    }
    Parser_error(this, NULL);
}

bool _parser_isToApply(Parser *this) {
    return Parser_isTypes(this, 1, TVAUES_GROUP_CHANGEABLE) && Parser_isWord(this, 2, SIGN_OPEN);
}

Leaf *_parser_consumeAstApply(Parser *this, Token *startToken, Token *endToken) {
    Token *name = Parser_checkType(this, 1, TVAUES_GROUP_CHANGEABLE);
    Leaf *leaf = Leaf_new(UG_ATYPE_APPLY);
    // args
    if (Parser_isWord(this, 1, startToken)) {
        Parser_checkValue(this, 1, 1, startToken);
        while (Parser_isTypes(this, 1, TTYPES_GROUP_VALUES)) {
            Token *variable = Parser_checkType(this, 1, TTYPES_GROUP_VALUES);
            Stack_push(leaf->tokens, variable);
        }
    }
    Stack_reverse(leaf->tokens);
    Parser_checkWord(this, 1, 1, endToken);
    // result
    Token *result = Token_empty();
    if (Parser_isValue(this, 1, LETTER_FURTHER))
    {
        Parser_checkValue(this, 1, 1, LETTER_FURTHER);
        result = Parser_checkType(this, 1, TVAUES_GROUP_CHANGEABLE);
        Parser_checkWord(this, 1, 1, LETTER_RECIEVED);
    }
    Stack_push(leaf->tokens, result);
    // name
    Stack_push(leaf->tokens, name);
    return leaf;
}

void Parser_consumeAstJudge(Parser *this, char aType)
{
    Object *first = NULL;
    //
    if (_parser_isToApply(this)) {
        first = _parser_consumeAstApply(this, SIGN_OPEN, SIGN_CLOSE);
    } else {
        first = Parser_checkType(this, 1, TTYPES_GROUP_VALUES);
    }
    // 
    if (Parser_isType(this, 1, UG_TTYPE_CLC)) {
        Token *clcltn = Parser_checkType(this, 1, 1, UG_TTYPE_CLC);
        Object *second = NULL;
        if (_parser_isToApply(this)) {
            second = _parser_consumeAstApply(this, SIGN_OPEN, SIGN_CLOSE);
        } else if (Parser_isType(this, 1, UG_TTYPE_WRD)) {
            second = Parser_checkValue(this, 1, TVAUES_GROUP_UTYPES);
        } else {
            second = Parser_checkType(this, 1, TTYPES_GROUP_VALUES);
        }
        Token *action = Parser_checkWord(this, 1, 2, LETTER_THEN, LETTER_ELSE);
        Parser_pushLeaf(this, aType, 4, second, clcltn, first, action);
    } else {
        Token *action = Parser_checkWord(this, 1, 2, LETTER_THEN, LETTER_ELSE);
        Parser_pushLeaf(this, aType, 2, first, action);
    }
}

void Parser_consumeAstIfFirst(Parser *this)
{
    // open UG_ATYPE_IF
    Parser_pushLeaf(this, UG_ATYPE_IF, 0, NULL);
    Parser_openBranch(this);
    // open UG_ATYPE_IF_F
    Parser_checkWord(this, 0, 1, LETTER_IF);
    Parser_consumeAstJudge(this, UG_ATYPE_IF_F);
    Parser_openBranch(this);
}

void Parser_consumeAstIfMiddle(Parser *this)
{
    // close UG_ATYPE_IF_F or UG_ATYPE_IF_M
    Token *token = this->token;
    char curType = this->leaf->type;
    Parser_assert(this, curType == UG_ATYPE_IF_F || curType == UG_ATYPE_IF_M, LANG_ERR_PARSER_INVALID_IF);
    Parser_closeBranch(this);
    //  open UG_ATYPE_IF_M
    Parser_checkWord(this, 0, 1, LETTER_ELIF);
    Parser_consumeAstJudge(this, UG_ATYPE_IF_M);
    Parser_openBranch(this);
}

void Parser_consumeAstIfLast(Parser *this)
{
    // close UG_ATYPE_IF_F or UG_ATYPE_IF_M
    char curType = this->leaf->type;
    Parser_assert(this, curType == UG_ATYPE_IF_F || curType == UG_ATYPE_IF_M, LANG_ERR_PARSER_INVALID_IF);
    Parser_closeBranch(this);
    // open UG_ATYPE_IF_L
    Token *token = Parser_checkWord(this, 0, 1, LETTER_ELSE);
    Parser_pushLeaf(this, UG_ATYPE_IF_L, 1, token);
    Parser_openBranch(this);
}

void Parser_consumeAstEnd(Parser *this)
{
    // close UG_ATYPE_IF_F or UG_ATYPE_IF_M or UG_ATYPE_IF_L
    char curType = this->leaf->type;
    if (
        curType == UG_ATYPE_IF_F
        || curType == UG_ATYPE_IF_M
        || curType == UG_ATYPE_IF_L
        || curType == UG_ATYPE_CODE
    ) {
        Parser_closeBranch(this);
    }
    // close ast code
    Parser_checkWord(this, 0, 1, LETTER_END);
    Parser_pushLeaf(this, UG_ATYPE_END, 0, NULL);
    Parser_closeBranch(this);
}

void Parser_consumeAstWhile(Parser *this)
{
    Parser_checkWord(this, 0, 1, LETTER_WHILE);
    Parser_consumeAstJudge(this, UG_ATYPE_WHL);
    Parser_openBranch(this);
}

void Parser_consumeAstSpread(Parser *this)
{
    Parser_checkWord(this, 0, 1, LETTER_SPREAD);
    Token *target = Parser_checkType(this, 1, TVAUES_GROUP_ITERABLE);
    Parser_checkWord(this, 1, 1, LETTER_VARIABLE);

    Token *iter1 = Parser_checkType(this, 1, 1, UG_TTYPE_NAM);
    Token *iter2 = Parser_isWord(this, 1, LETTER_BECOME) ? Token_variable() : Parser_checkType(this, 1, 1, UG_TTYPE_NAM);
    //
    Parser_checkWord(this, 1, 1, LETTER_BECOME);
    Parser_pushLeaf(this, UG_ATYPE_SPR, 3, iter2, iter1, target);
    Parser_openBranch(this);
}

void Parser_consumeAstException(Parser *this)
{
    Parser_checkWord(this, 0, 1, LETTER_EXCEPTION);
    Token *name = Parser_checkType(this, 1, TVAUES_GROUP_CHANGEABLE);
    Parser_checkWord(this, 1, 1, LETTER_BECOME);
    Parser_pushLeaf(this, UG_ATYPE_EXC, 1, name);
    Parser_openBranch(this);
}

void Parser_consumeAstResult(Parser *this)
{
    Parser_checkWord(this, 0, 1, LETTER_RESULT);
    Token *name = Parser_checkType(this, 1, TTYPES_GROUP_VALUES);
    Parser_checkWord(this, 1, 1, LETTER_RETURNED);
    Parser_pushLeaf(this, UG_ATYPE_RSLT, 1, name);
}

void _parser_continueAstBody(Parser *this) {

}

void _Parser_continueAstAppliable(Parser *this, char appliableType, Token *name) {
    // begin
    Leaf *leaf = Leaf_new(appliableType);
    Leaf *code = Leaf_new(UG_ATYPE_CODE);
    // args
    if (Parser_isValue(this, 1, LETTER_VARIABLE))
    {
        Parser_checkValue(this, 1, 1, LETTER_VARIABLE);
        Token *variable = Parser_checkType(this, 1, 1, UG_TTYPE_NAM);
        while (variable != NULL)
        {
            Stack_push(code->tokens, variable);
            variable = Parser_isValue(this, 1, LETTER_CONTENT) ? NULL : Parser_checkType(this, 1, 1, UG_TTYPE_NAM);
        }
    }
    Stack_reverse(code->tokens);
    // finish
    Parser_checkValue(this, 1, 1, LETTER_CONTENT);
    Stack_push(leaf->tokens, name);
    Stack_push(code->tokens, name);
    Leaf_pushLeaf(this->leaf, leaf);
    Parser_openBranch(this);
    Leaf_pushLeaf(this->leaf, code);
    Parser_openBranch(this);
}

void Parser_consumeAstWorker(Parser *this)
{
    Parser_checkWord(this, 0, 1, LETTER_WORKER);
    Token *name = Parser_checkType(this, 1, TVAUES_GROUP_CHANGEABLE);
    _Parser_continueAstAppliable(this, UG_ATYPE_WRKR, name);
}

void Parser_consumeAstCreator(Parser *this)
{
    Parser_checkWord(this, 0, 1, LETTER_CREATOR);
    Token *name = Parser_checkType(this, 1, 1, UG_TTYPE_NAM);
    _Parser_continueAstAppliable(this, UG_ATYPE_CRTR, name);
}

void Parser_consumeAstAssister(Parser *this)
{
    Parser_checkWord(this, 0, 1, LETTER_ASSISTER);
    Token *name = Parser_checkType(this, 1, 1, UG_TTYPE_NAM);
    _Parser_continueAstAppliable(this, UG_ATYPE_ASTR, name);
}

void Parser_consumeAstApply(Parser *this)
{
    bool isWorker = Parser_isWord(this, 0, LETTER_WORKER);
    bool isCrator = Parser_isWord(this, 0, LETTER_CREATOR);
    bool isAssister = Parser_isWord(this, 0, LETTER_ASSISTER);
    Parser_assert(this, isWorker || isCrator || isAssister, LANG_ERR_PARSER_INVALID_VARIABLE);
    Parser_checkType(this, 0, 1, UG_TTYPE_WRD);
    //
    Leaf *leaf = _parser_consumeAstApply(this, LETTER_WITH, LETTER_APPLY);
    Leaf_pushLeaf(this->leaf, leaf);
}

void Parser_consumeAstCalculator(Parser *this)
{
    Token *target = Parser_checkType(this, 0, TVAUES_GROUP_CHANGEABLE);
    Parser_checkWord(this, 1, 1, SIGN_EQUAL);
    Token *tempT = NULL;
    Foliage *tempF = NULL;
    Foliage *current = NULL;
    Stack *currents = Stack_new(true);
    Stack_push(currents, Foliage_new(NULL));
    char *lastType = NULL;
    while (true) {
        current = (Foliage*)currents->tail->data;
        if (Parser_isTypes(this, 1, TTYPES_GROUP_VALUES) || Parser_isValues(this, 1, TVAUES_GROUP_UTYPES)) {
            if (lastType == NULL || is_eq_string(lastType, SIGN_OPEN)) {
                tempT = Parser_checkType(this, 1, TTYPES_GROUP_VALUES);
                tempF = Foliage_new(tempT);
                current->left = tempF;
            } else if (is_calculation_str(lastType)) {
                if (Parser_isType(this, 1, UG_TTYPE_WRD)) {
                    tempT = Parser_checkValue(this, 1, TVAUES_GROUP_UTYPES);
                } else {
                    tempT = Parser_checkType(this, 1, TTYPES_GROUP_VALUES);
                }
                tempF = Foliage_new(tempT);
                current->right = tempF;
            } else {
                break;
            }
            lastType = tempT->type;
            continue;
        } else if (lastType != NULL && Parser_isType(this, 1, UG_TTYPE_CLC)) {
            Parser_assert(this, is_eq_string(lastType, SIGN_CLOSE) || is_eq_strings(lastType, TTYPES_GROUP_VALUES), LANG_ERR_PARSER_INVALID_CALCULATOR);
            tempT = Parser_checkType(this, 1, 1, UG_TTYPE_CLC);
            if (current->data != NULL) {
                tempF = Foliage_new(tempT);
                if (is_higher_priority_calculation(tempT->value, ((Token *)current->data)->value)) {
                    tempF->left = current->right;
                    current->right = tempF;
                } else {
                    tempF->left = current;
                    Stack_pop(currents);
                }
                current = tempF;
                Stack_push(currents, tempF);
            }
            current->data = tempT;
            lastType = tempT->value;
            continue;
        } else if (Parser_isWord(this, 1, SIGN_OPEN)) {
            if (lastType == NULL || is_eq_string(lastType, SIGN_OPEN)) {
                Parser_checkWord(this, 1, 1, SIGN_OPEN);
                tempF = Foliage_new(NULL);
                current->left = tempF;
            } else if (is_calculation_str(lastType)) {
                Parser_checkWord(this, 1, 1, SIGN_OPEN);
                tempF = Foliage_new(NULL);
                current->right = tempF;
            } else {
                break;
            }
            Stack_push(currents, tempF);
            lastType = SIGN_OPEN;
            continue;
        } else if (Parser_isWord(this, 1, SIGN_CLOSE)) {
            Parser_assert(this, is_eq_string(lastType, SIGN_CLOSE) || is_eq_strings(lastType, TTYPES_GROUP_VALUES), LANG_ERR_PARSER_INVALID_CALCULATOR);
            Parser_checkWord(this, 1, 1, SIGN_CLOSE);
            Stack_pop(currents);
            lastType = SIGN_CLOSE;
            continue;
        }
        break;
    }
    current = (Foliage*)currents->head->data;
    Token *body = Token_new(SIGN_EQUAL, current);
    Parser_pushLeaf(this, UG_ATYPE_CLC, 2, target, body);
}

void Parser_consumeAstGenerator(Parser *this)
{
    Token *target = Parser_checkType(this, 0, TVAUES_GROUP_CHANGEABLE);
    Parser_checkWord(this, 1, 1, SIGN_EQUAL);
    Block *waitingKey = NULL;
    Object *current = NULL;
    Stack *currents = Stack_new(true);
    Object *root = NULL;
    char *lastType = NULL;
    while (true) {
        current = currents->tail != NULL ? currents->tail->data : NULL;
        Token *token = Parser_getToken(this, 1);
        UTFCHAR *value = token->value;
        //
        Object *box = NULL;
        if (Parser_isWord(this, 1, SIGN_OPEN_BIG)) {
            box = Queue_new(true);
            Parser_checkWord(this, 1, 1, SIGN_OPEN_BIG);
        } else if (Parser_isWord(this, 1, SIGN_OPEN_MIDDLE)) {
            box = Stack_new(true);
            Parser_checkWord(this, 1, 1, SIGN_OPEN_MIDDLE);
        }
        if (box) {
            Parser_assert(this, can_generate_open(lastType), LANG_ERR_PARSER_INVALID_GENERATOR);
            Stack_push(currents, box);
            if (current) {
                Block *block = Block_new(box);
                if (current->objType == PCT_OBJ_QUEUE) {
                    Queue_push(current, block);
                    Parser_assert(this, waitingKey == NULL, LANG_ERR_PARSER_INVALID_GENERATOR);
                } else if (current->objType == PCT_OBJ_STACK) {
                    Stack_push(current, block);
                    Parser_assert(this, waitingKey != NULL, LANG_ERR_PARSER_INVALID_GENERATOR);
                    block->next = waitingKey;
                    waitingKey = NULL;
                }
            }
            // 
            lastType = value;
            continue;
        }
        //
        bool close = false;
        if (Parser_isWord(this, 1, SIGN_CLOSE_BIG)) {
            Parser_checkWord(this, 1, 1, SIGN_CLOSE_BIG);
            Parser_assert(this, current->objType == PCT_OBJ_QUEUE, LANG_ERR_PARSER_INVALID_GENERATOR);
            close = true;
        } else if (Parser_isWord(this, 1, SIGN_CLOSE_MIDDLE)) {
            Parser_checkWord(this, 1, 1, SIGN_CLOSE_MIDDLE);
            Parser_assert(this, current->objType == PCT_OBJ_STACK, LANG_ERR_PARSER_INVALID_GENERATOR);
            close = true;
        }
        if (close) {
            Parser_assert(this, currents->size > 0 && can_generate_close(lastType), LANG_ERR_PARSER_INVALID_GENERATOR);
            lastType = token->value;
            //
            Object *popped = Stack_pop(currents);
            if (currents->size == 0) {
                root = popped;
                break;
            }
            continue;
        }
        //
        if (Parser_isWord(this, 2, SIGN_COLON)) {
            Token *key = Parser_checkType(this, 1, 1, UG_TTYPE_NAM);
            waitingKey = key;
            Parser_checkWord(this, 1, 1, SIGN_COLON);
            lastType = SIGN_COLON;
            continue;
        } else if (Parser_isWord(this, 1, SIGN_COMMA)) {
            Parser_checkWord(this, 1, 1, SIGN_COMMA);
            lastType = SIGN_COMMA;
            continue; 
        } else if (Parser_isTypes(this, 1, TTYPES_GROUP_VALUES)) {
            Token *val = Parser_checkType(this, 1, TTYPES_GROUP_VALUES);
            Token *tmp = Parser_getToken(this, 1);
            // 
            Block *block = Block_new(val);
            if (current->objType == PCT_OBJ_QUEUE) {
                Queue_push(current, block);
                Parser_assert(this, waitingKey == NULL, LANG_ERR_PARSER_INVALID_GENERATOR);
            } else if (current->objType == PCT_OBJ_STACK) {
                Stack_push(current, block);
                Parser_assert(this, waitingKey != NULL, LANG_ERR_PARSER_INVALID_GENERATOR);
                block->next = waitingKey;
                waitingKey = NULL;
            }
            //
            lastType = SIGN_EQUAL;
            continue; 
        }
        break;
    }
    Parser_assert(this, is_eq_strings(lastType, 2, SIGN_CLOSE_BIG, SIGN_CLOSE_MIDDLE), LANG_ERR_PARSER_INVALID_GENERATOR);
    Parser_assert(this, currents->size == 0, LANG_ERR_PARSER_INVALID_GENERATOR);
    Parser_assert(this, root != NULL, LANG_ERR_PARSER_INVALID_GENERATOR);
    Token *body = Token_new(SIGN_EQUAL, root);
    Parser_pushLeaf(this, UG_ATYPE_GNR, 2, target, body);  
}

void Parser_consumeToken(Parser *this, Token *token)
{
    //
    char *t = token->type;
    char *v = token->value;
    log_debug("parser.next: %s | %s", t, v);
    // VARIABLE
    if (is_eq_string(t, UG_TTYPE_WRD) && is_eq_string(v, LETTER_VARIABLE))
    {
        Parser_consumeAstVariable(this);
        return;
    }
    // END
    if (is_eq_string(v, LETTER_END))
    {
        Parser_consumeAstEnd(this);
        return;
    }
    // RESULT
    if (is_eq_string(v, LETTER_RESULT))
    {
        Parser_consumeAstResult(this);
        return;
    }
    // WORKER
    if (is_eq_string(v, LETTER_WORKER)) {
        if (Parser_isValue(this, 2, LETTER_VARIABLE) || Parser_isValue(this, 2, LETTER_CONTENT)) {
            Parser_consumeAstWorker(this);
            return;
        }
    }
    // WORKER
    if (is_eq_string(v, LETTER_CREATOR)) {
        if (Parser_isValue(this, 2, LETTER_VARIABLE) || Parser_isValue(this, 2, LETTER_CONTENT)) {
            Parser_consumeAstCreator(this);
            return;
        }
    }
    // ASSISTER
    if (is_eq_string(v, LETTER_ASSISTER)) {
        if (Parser_isValue(this, 2, LETTER_VARIABLE) || Parser_isValue(this, 2, LETTER_CONTENT)) {
            Parser_consumeAstAssister(this);
            return;
        }
    }
    // APPLY
    if ((is_eq_string(v, LETTER_WORKER) || is_eq_string(v, LETTER_CREATOR) || is_eq_string(v, LETTER_ASSISTER))) {
        if (Parser_isValue(this, 2, LETTER_WITH) || Parser_isValue(this, 2, LETTER_APPLY)) {
            Parser_consumeAstApply(this);
            return;
        }
    }
    // IF_FIRST
    if (is_eq_string(v, LETTER_IF))
    {
        Parser_consumeAstIfFirst(this);
        return;
    }
    // IF_MIDDLE
    if (is_eq_string(v, LETTER_ELIF))
    {
        Parser_consumeAstIfMiddle(this);
        return;
    }
    // IF_LAST
    if (is_eq_string(v, LETTER_ELSE))
    {
        Parser_consumeAstIfLast(this);
        return;
    }
    // WHILE
    if (is_eq_string(v, LETTER_WHILE))
    {
        Parser_consumeAstWhile(this);
        return;
    }
    // SPREAD
    if (is_eq_string(v, LETTER_SPREAD))
    {
        Parser_consumeAstSpread(this);
        return;
    }
    // exception
    if (is_eq_string(v, LETTER_EXCEPTION))
    {
        Parser_consumeAstException(this);
        return;
    }
    // EXPRESSION
    if ((is_eq_string(t, UG_TTYPE_NAM) || is_eq_string(t, UG_TTYPE_KEY)) && Parser_isWord(this, 1, LETTER_VALUE))
    {
        Parser_consumeAstConvert(this);
        return;
    }
    // calculate
    if ((is_eq_string(t, UG_TTYPE_NAM) || is_eq_string(t, UG_TTYPE_KEY)) && Parser_isWord(this, 1, SIGN_EQUAL))
    {
        if (Parser_isWord(this, 2, SIGN_OPEN_BIG)) {
            Parser_consumeAstGenerator(this);
        } else if (Parser_isWord(this, 2, SIGN_OPEN_MIDDLE)) {
            Parser_consumeAstGenerator(this);
        } else {
            Parser_consumeAstCalculator(this);
        }
        return;
    }
    // command
    if (is_eq_string(t, UG_TTYPE_WRD) && is_eq_string(v, LETTER_COMMAND))
    {
        Parser_consumeAstCommand(this);
        return;
    }
    //
    log_error("parser.error: %s | %s", t, v);
    Parser_error(this, NULL);
    Token_print(token);
}

Leaf *Parser_parseTokens(Parser *this, Token *tokens)
{
    Parser_reset(this);
    this->tokens = tokens;
    this->tree = Leaf_new(UG_ATYPE_PRG);
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
