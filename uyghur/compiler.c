// compiler

#include "others/header.h"

Draft *Compiler_consumeApply(Compiler *, Leaf *);
void Compiler_consumeLeaf(Compiler *, Leaf *);
void Compiler_consumeTree(Compiler *, Leaf *);

void Compiler_reset(Compiler *this)
{
    this->uyghur = NULL;
    this->draft = NULL;
    this->isReturn = false;
    this->isCatch = false;
    this->errorMsg = NULL;
}

Compiler *Compiler_new(Uyghur *uyghur)
{
    Compiler *compiler = malloc(sizeof(Compiler));
    Compiler_reset(compiler);
    compiler->uyghur = uyghur;
    compiler->draft = Draft_new(UG_ATYPE_PRG);
    return compiler;
}

void Compiler_error(Compiler *this, Token *token, char *msg)
{
    char *m = msg != NULL ? msg : LANG_ERR_COMPILER_EXCEPTION;
    char *s = token == NULL ? UG_TAG_UNKNOWN : format_token_place(token);
    char *err = tools_format("%s, %s", m, s);
    log_error("Compiler: %s, %s", m, s);
    exit(1);
}

void Compiler_assert(Compiler *this, bool value, Token *token, char *msg)
{
    if (value == true) return;
    Compiler_error(this, token, msg);
}

void Compiler_pushScope(Compiler *this, CString name)
{
    log_info("{");
}

void Compiler_popScope(Compiler *this)
{
    log_info("}");
}

void Compiler_consumeVariable(Compiler *this, Leaf *leaf)
{
    Stack_RESTE(leaf->tokens);
    Token *token = Stack_NEXT(leaf->tokens);
    Token *name = Stack_NEXT(leaf->tokens);
    //
    if (Token_isEmpty(token)) {
        
    } else if (Token_isWord(token) && is_eq_string(token->value, LETTER_BOL)) {
        
    } else if (Token_isWord(token) && is_eq_string(token->value, LETTER_NUM)) {
        
    } else if (Token_isWord(token) && is_eq_string(token->value, LETTER_STR)) {
        
    } else if (Token_isWord(token) && is_eq_string(token->value, LETTER_LST)) {
        
    } else if (Token_isWord(token) && is_eq_string(token->value, LETTER_DCT)) {
        
    } else {
        
    }
}

void Compiler_consumeCommand(Compiler *this, Leaf *leaf)
{
    Stack_RESTE(leaf->tokens);
    Token *action = Stack_NEXT(leaf->tokens);
    Token *target = Stack_NEXT(leaf->tokens);
    Queue *args = action->extra;
    Queue_RESTE(args);
    // 
    if (is_eq_string(action->value, LETTER_CMD_OUTPUT))
    {
        Token *name = Queue_NEXT(args);
        while (name) {
            //
            name = Queue_NEXT(args);
        }
    }
    else if (is_eq_string(action->value, LETTER_CMD_INPUT))
    {
        Token *name = Queue_NEXT(args);
        while (name) {
            //
            name = Queue_NEXT(args);
        }
    }
}

void Compiler_consumeConvert(Compiler *this, Leaf *leaf)
{
    Stack_RESTE(leaf->tokens);
    Token *action = Stack_NEXT(leaf->tokens);
    Token *target = Stack_NEXT(leaf->tokens);
    char *act = action->value;
    //
    if (is_eq_string(action->type, UG_TTYPE_WRD))
    {
        if (is_eq_string(act, LETTER_NIL))
        {
            //
        }
        else if (is_eq_string(act, LETTER_OPPOSITE))
        {
            
        }
        else if (is_eq_string(act, LETTER_NUM))
        {
            
        }
        else if (is_eq_string(act, LETTER_STR))
        {
            
        }
        else if (is_eq_string(act, LETTER_BOL))
        {
            
        }
        else if (is_apply_action(act))
        {
            
        }
    }
    else
    {
        //
    }
}

Draft *_compiler_calculateJudge(Compiler *this, Object *some, Token *judge) {
    if (some->objType == PCT_OBJ_LEAF) {
        Leaf *_leaf = (Leaf *)some;
        Compiler_assert(this, _leaf->type == UG_ATYPE_APPLY, judge, LANG_ERR_COMPILER_EXCEPTION);
        return Compiler_consumeApply(this, _leaf);
    } else {
        // token
    }
}

bool Compiler_checkJudge(Compiler *this, Leaf *leaf)
{
    //
    Stack_RESTE(leaf->tokens);
    Token *judge = Stack_NEXT(leaf->tokens);
    Token *first = Stack_NEXT(leaf->tokens);
    Token *clcltn = Stack_NEXT(leaf->tokens);
    Token *second = Stack_NEXT(leaf->tokens);
    //
    //
    if (clcltn == NULL) {
        //
    } else {
        //
    }
    // 
    bool shouldOk = is_eq_string(judge->value, LETTER_THEN);
    bool isOk = true;
    //
    return isOk == shouldOk;
}

void Compiler_consumeIf(Compiler *this, Leaf *leaf)
{
    //
    bool isFinish = false;
    Queue_RESTE(leaf->leafs);
    // if
    Leaf *ifNode = Queue_NEXT(leaf->leafs);
    tools_assert(ifNode != NULL, LANG_ERR_GRAMMAR_INVALID_IF);
    tools_assert(ifNode->type == UG_ATYPE_IF_F, LANG_ERR_GRAMMAR_INVALID_IF);
    if (!isFinish && Compiler_checkJudge(this, ifNode))
    {
        Compiler_pushScope(this, LETTER_IF);
        Compiler_consumeTree(this, ifNode);
        Compiler_popScope(this);
        isFinish = true;
    }
    // elseif
    ifNode = Queue_NEXT(leaf->leafs);
    while (ifNode && ifNode->type == UG_ATYPE_IF_M)
    {
        if (!isFinish && Compiler_checkJudge(this, ifNode))
        {
            Compiler_pushScope(this, LETTER_ELIF);
            Compiler_consumeTree(this, ifNode);
            Compiler_popScope(this);
            isFinish = true;
        }
        ifNode = Queue_NEXT(leaf->leafs);
        tools_assert(ifNode != NULL, LANG_ERR_GRAMMAR_INVALID_IF);
    }
    // else
    if (ifNode && ifNode->type == UG_ATYPE_IF_L)
    {
        Stack_RESTE(ifNode->tokens);
        Token *token = Stack_NEXT(ifNode->tokens);
        tools_assert(is_eq_string(token->value, LETTER_ELSE), LANG_ERR_GRAMMAR_INVALID_IF);
        if (!isFinish)
        {
            Compiler_pushScope(this, LETTER_ELSE);
            Compiler_consumeTree(this, ifNode);
            Compiler_popScope(this);
            isFinish = true;
        }
        ifNode = Queue_NEXT(leaf->leafs);
        tools_assert(ifNode != NULL, LANG_ERR_GRAMMAR_INVALID_IF);
    }
    // end
    tools_assert(ifNode && ifNode->type == UG_ATYPE_END, LANG_ERR_GRAMMAR_INVALID_IF);
    Leaf *nullValue = Queue_NEXT(leaf->leafs);
    tools_assert(nullValue == NULL, LANG_ERR_GRAMMAR_INVALID_IF);
}

void Compiler_consumeWhile(Compiler *this, Leaf *leaf)
{
    Compiler_checkJudge(this, leaf);
    Compiler_pushScope(this, LETTER_WHILE);
    Compiler_consumeTree(this, leaf);
    Compiler_popScope(this);
}

void Compiler_consumeSpread(Compiler *this, Leaf *leaf)
{
    //
    Stack_RESTE(leaf->tokens);
    Token *target = Stack_NEXT(leaf->tokens);
    Token *iter1 = Stack_NEXT(leaf->tokens);
    Token *iter2 = Stack_NEXT(leaf->tokens);
    //
    // Compiler_error(this, target, LANG_ERR_GRAMMAR_INVALID_SPREAD);
}

void Compiler_consumeException(Compiler *this, Leaf *leaf)
{
    
    Stack_RESTE(leaf->tokens);
    Token *name = Stack_NEXT(leaf->tokens);
    this->isCatch = true;
    // 
    Compiler_pushScope(this, LETTER_EXCEPTION);
    Compiler_consumeTree(this, leaf);
    Compiler_popScope(this);
    // 
    this->isCatch = false;
    void *error = this->errorMsg;
    this->errorMsg = NULL;
}

void _Compiler_parseAppliable(Compiler *this, Leaf *leaf, char type, Token **func, Leaf **code) {
    // func name
    Stack_RESTE(leaf->tokens);
    *func = Stack_NEXT(leaf->tokens);
    // func body
    Queue_RESTE(leaf->leafs);
    *code = Queue_NEXT(leaf->leafs);
    // with key
    if (!Token_isKey(*func)) return;
    // validate place
    
    Compiler_assert(this, Token_isName(*func), *func, LANG_ERR_GRAMMAR_INVALID_NAME);
}

void Compiler_consumeWorker(Compiler *this, Leaf *leaf)
{
    Token *func; Leaf *code;
    _Compiler_parseAppliable(this, leaf, UG_TYPE_WKR, &func, &code);
    //
}

void Compiler_consumeCreator(Compiler *this, Leaf *leaf)
{
    Token *func; Leaf *code;
    _Compiler_parseAppliable(this, leaf, UG_TYPE_CTR, &func, &code);
    //
}

void Compiler_consumeAssister(Compiler *this, Leaf *leaf)
{
    Token *func; Leaf *code;
    _Compiler_parseAppliable(this, leaf, UG_TYPE_ATR, &func, &code);
    //
}

void Compiler_consumeCode(Compiler *this, Leaf *leaf)
{
    Stack_RESTE(leaf->tokens);
    Token *funcName = Stack_NEXT(leaf->tokens);
    Token *arg = Stack_NEXT(leaf->tokens);
    while(arg != NULL)
    {
        //
        arg = Stack_NEXT(leaf->tokens);
    }
    //
    Compiler_consumeTree(this, leaf);
}

Draft *Compiler_consumeApply(Compiler *this, Leaf *leaf)
{
    Stack_RESTE(leaf->tokens);
    // get runnable name and result name
    Token *runnableName = Stack_NEXT(leaf->tokens);
    Token *resultName = Stack_NEXT(leaf->tokens);
    // push args
    Token *arg = Stack_NEXT(leaf->tokens);
    while(arg != NULL)
    {
        //
        arg = Stack_NEXT(leaf->tokens);
    }
    //
    // Compiler_error(this, runnableName, LANG_ERR_SOME_RUNNABLE_INVALID);
    return NULL;
}

void Compiler_consumeResult(Compiler *this, Leaf *leaf)
{
    Stack_RESTE(leaf->tokens);
    Token *result = Stack_NEXT(leaf->tokens);
    //
    this->isReturn = true;
}

void Compiler_consumeCalculator(Compiler *this, Leaf *leaf)
{
    Stack_RESTE(leaf->tokens);
    Token *body = Stack_NEXT(leaf->tokens);
    Token *target = Stack_NEXT(leaf->tokens);
    Foliage *root = (Foliage *)body->value;
    // 
    // Compiler_assert(this, NULL, target, LANG_ERR_SOME_CALCULATION_INVALID);
}

Draft *Compiler_generateContainer(Compiler *this, Object *, Token *);
Draft *Compiler_generateContainer(Compiler *this, Object *object, Token *token)
{
    bool isArr = object->objType == PCT_OBJ_QUEUE;
    bool isMap = object->objType == PCT_OBJ_STACK;
    Block *block = NULL;
    if (isArr) {
        Queue_RESTE(object);
        block = Queue_NEXT(object);
    } else if (isMap) {
        Stack_reverse(object);
        Stack_RESTE(object);
        block = Stack_NEXT(object);
    }
    //
    Draft *result = Draft_new(UG_TYPE_NON);
    while(block != NULL)
    {
        Token *key = block->next;
        Object *val = block->data;
        bool noKey = key == NULL;
        Compiler_assert(this, isArr == noKey, NULL, LANG_ERR_GRAMMAR_INVALID_KEY);
        //
        if (val->objType == PCT_OBJ_QUEUE) {
            
        } else if (val->objType == PCT_OBJ_STACK) {
            
        } else {
            
        }
        //
        if (isArr) {
            // 
        } else if (isMap) {
            // 
        } else {
            // Compiler_error(this, NULL, LANG_ERR_GRAMMAR_INVALID_GENERATION);
        }
        //
        if (isArr) {
            block = Queue_NEXT(object);
        } else if (isMap) {
            block = Stack_NEXT(object);
        }
    }
    return result;
}

void Compiler_consumeGenerator(Compiler *this, Leaf *leaf)
{
    Stack_RESTE(leaf->tokens);
    Token *body = Stack_NEXT(leaf->tokens);
    Token *target = Stack_NEXT(leaf->tokens);
    Object *root = (Object *)body->value;
    //
    // Compiler_assert(this, NULL, target, LANG_ERR_GRAMMAR_INVALID_GENERATION);
}

void Compiler_consumeLeaf(Compiler *this, Leaf *leaf)
{
    char tp = leaf->type;
    // log_debug("compiler.next: %c", tp);
    // throwing
    if (this->errorMsg != NULL && tp != UG_ATYPE_EXC) {
        return;
    }
    // variable
    if (tp == UG_ATYPE_VAR)
    {
        Compiler_consumeVariable(this, leaf);
        return;
    }
    // command
    if (tp == UG_ATYPE_CMD)
    {
        Compiler_consumeCommand(this, leaf);
        return;
    }
    // expression
    if (tp == UG_ATYPE_CVT)
    {
        Compiler_consumeConvert(this, leaf);
        return;
    }
    // if
    if (tp == UG_ATYPE_IF)
    {
        Compiler_consumeIf(this, leaf);
        return;
    }
    // while
    if(tp == UG_ATYPE_WHL)
    {
        Compiler_consumeWhile(this, leaf);
        return;
    }
    // spread
    if(tp == UG_ATYPE_SPR)
    {
        Compiler_consumeSpread(this, leaf);
        return;
    }
    // exception
    if(tp == UG_ATYPE_EXC)
    {
        Compiler_consumeException(this, leaf);
        return;
    }
    // worker
    if(tp == UG_ATYPE_WRKR) {
        Compiler_consumeWorker(this, leaf);
        return;
    }
    // creator
    if(tp == UG_ATYPE_CRTR) {
        Compiler_consumeCreator(this, leaf);
        return;
    }
    // assister
    if(tp == UG_ATYPE_ASTR) {
        Compiler_consumeAssister(this, leaf);
        return;
    }
    // apply
    if(tp == UG_ATYPE_APPLY)
    {
        Compiler_consumeApply(this, leaf);
        return;
    }
    // code
    if(tp == UG_ATYPE_CODE)
    {
        Compiler_consumeCode(this, leaf);
        return;
    }
    // result
    if(tp == UG_ATYPE_RSLT)
    {
        Compiler_consumeResult(this, leaf);
        return;
    }
    // calculator
    if (tp == UG_ATYPE_CLC)
    {
        Compiler_consumeCalculator(this, leaf);
        return;
    }
    // generator
    if (tp == UG_ATYPE_GNR)
    {
        Compiler_consumeGenerator(this, leaf);
        return;
    }
    // end
    if(tp == UG_ATYPE_END)
    {
        return;
    }
    //
    log_error("compiler.error: %c", tp);
    helper_print_leaf(leaf, " ");
    tools_error("%s:[%c]", LANG_ERR_COMPILER_NOT_IMPLEMENTED, tp);
}

void Compiler_consumeTree(Compiler *this, Leaf *tree)
{
    Queue_RESTE(tree->leafs);
    Leaf *leaf = Queue_NEXT(tree->leafs);
    while (leaf != NULL)
    {
        Compiler_consumeLeaf(this, leaf);
        if (this->isReturn) break;
        leaf = Queue_NEXT(tree->leafs);
    }
}

void *Compiler_compileCode(Compiler *this, Leaf *tree) {
    Compiler_consumeTree(this, tree);
}

void Compiler_free(Compiler *this)
{
    // 
    Draft_print(this->draft);
    Draft_free(this->draft);
    // 
    Compiler_reset(this);
    free(this);
}
