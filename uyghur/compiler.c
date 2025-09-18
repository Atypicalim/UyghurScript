// compiler

#include "others/header.h"

CPointer Compiler_popPass(Compiler *);
void Compiler_bindGenerate(Compiler *, CName, CGenerator);
void Compiler_callGenerateNewLine(Compiler *, CName, bool);
void Compiler_callGenerateSameLine(Compiler *, CName, bool);

#include "generators/generate_js.c"
#include "generators/generate_go.c"

void _compiler_prepareApply(Compiler *, Leaf *);
void Compiler_consumeLeaf(Compiler *, Leaf *);
void Compiler_consumeTree(Compiler *, Leaf *);

void Compiler_reset(Compiler *this)
{
    this->uyghur = NULL;
    this->dialect = "";
    this->draft = NULL;
    this->deepth = 0;
    this->passQueue = NULL;
    this->generatorsMap = NULL;
    this->isReturn = false;
    this->isCatch = false;
    this->errorMsg = NULL;
}

Compiler *Compiler_new(Uyghur *uyghur)
{
    Compiler *compiler = malloc(sizeof(Compiler));
    Compiler_reset(compiler);
    compiler->uyghur = uyghur;
    compiler->dialect = "";
    compiler->draft = Draft_new(UG_ATYPE_PRG);
    compiler->passQueue = Queue_new(false);
    compiler->generatorsMap = Hashmap_new(false);
    generator_js_register(compiler);
    generator_go_register(compiler);
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
    Compiler_callGenerateSameLine(this, "then", false);
    this->deepth++;
    if (this->deepth > 10) this->deepth = 10;
}

void Compiler_popScope(Compiler *this)
{
    this->deepth--;
    if (this->deepth < 0) this->deepth = 0;
    Compiler_callGenerateNewLine(this, "end", false);
    Draft_pushLine(this->draft, "");
}

void Compiler_bindGenerate(Compiler *this, CName name, CGenerator func) {
    Hashmap_set(this->generatorsMap, name, func);
}

CString _compiler_callGenerate(Compiler *this, CName _name) {
    CName name = tools_format("generate_%s_%s", this->dialect, _name);
    CFunction func = Hashmap_get(this->generatorsMap, name);
    pct_free(name);
    tools_assert(func != NULL, "%s:%s", LANG_ERR_COMPILER_EXCEPTION, name);
    log_debug("generate:%s", _name);
    CString text = func(this);
    return text;
}

CString Compiler_callGenerateReadText(Compiler *this, CName _name) {
    CString text = _compiler_callGenerate(this, _name);
    return text;
}

void Compiler_callGenerateNewLine(Compiler *this, CName _name, bool withSemicolon) {
    Draft_pushString(this->draft, "\n");
    int deepth = this->deepth;
    while(deepth > 0) {
        Draft_pushString(this->draft, "  ");
        deepth--;
    }
    CString text = _compiler_callGenerate(this, _name);
    Draft_pushString(this->draft, text);
    if (withSemicolon) {
        Draft_pushString(this->draft, ";");
    } else {
        Draft_pushString(this->draft, "");
    }
}

void Compiler_callGenerateSameLine(Compiler *this, CName _name, bool withSemicolon) {
    CString text = _compiler_callGenerate(this, _name);
    Draft_pushString(this->draft, text);
    if (withSemicolon) {
        Draft_pushString(this->draft, ";");
    } else {
        Draft_pushString(this->draft, "");
    }
}

void Compiler_buildPass(Compiler *this, int num, CPointer val, ...)
{
    Queue_clear(this->passQueue);
    va_list args;
    va_start(args, val);
    int i;
    for (i = 0; i < num; i++) {
        Queue_push(this->passQueue, val);
        val = va_arg(args, CPointer);
    }
    va_end(args);
}

void Compiler_pushPass(Compiler *this, CPointer val)
{
    Queue_push(this->passQueue, val);
}

CPointer Compiler_popPass(Compiler *this)
{
    return Queue_pop(this->passQueue);
}

void Compiler_consumeVariable(Compiler *this, Leaf *leaf)
{
    Stack_RESTE(leaf->tokens);
    Token *token = Stack_NEXT(leaf->tokens);
    Token *name = Stack_NEXT(leaf->tokens);
    Compiler_buildPass(this, 2, name, token);
    Compiler_callGenerateNewLine(this, "variable", true);
}

void Compiler_consumeCommand(Compiler *this, Leaf *leaf)
{
    Stack_RESTE(leaf->tokens);
    Token *action = Stack_NEXT(leaf->tokens);
    Token *target = Stack_NEXT(leaf->tokens);
    Compiler_buildPass(this, 1, action);
    Queue *args = action->extra;
    Queue_RESTE(args);
    Token *name = Queue_NEXT(args);
    while (name) {
        Compiler_pushPass(this, name);
        name = Queue_NEXT(args);
    }
    Compiler_callGenerateNewLine(this, "command", true);
}

void Compiler_consumeConvert(Compiler *this, Leaf *leaf)
{
    Stack_RESTE(leaf->tokens);
    Token *action = Stack_NEXT(leaf->tokens);
    Token *target = Stack_NEXT(leaf->tokens);
    char *act = action->value;
    Compiler_callGenerateNewLine(this, "convert", true);
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
Token *_compiler_calculateJudge(Compiler *this, Object *some, Token *judge) {
    if (some == NULL) {
        return NULL;
    } if (some->objType == PCT_OBJ_LEAF) {
        _compiler_prepareApply(this, some);
        CString appiement = Compiler_callGenerateReadText(this, "apply");
        return Token_name(appiement);
    } else {
        return some;
    }
}

void Compiler_checkJudge(Compiler *this, Leaf *leaf)
{
    Stack_RESTE(leaf->tokens);
    Token *judge = Stack_NEXT(leaf->tokens);
    Token *first = Stack_NEXT(leaf->tokens);
    Token *clcltn = Stack_NEXT(leaf->tokens);
    Token *second = Stack_NEXT(leaf->tokens);
    //
    first = _compiler_calculateJudge(this, first, judge); 
    second = _compiler_calculateJudge(this, second, judge); 
    //
    tools_assert((clcltn != NULL) == (second != NULL), LANG_ERR_COMPILER_EXCEPTION);
    Compiler_buildPass(this, 4, judge, first, clcltn, second);
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
    Compiler_checkJudge(this, ifNode);
    Draft_pushLine(this->draft, "");
    Compiler_callGenerateNewLine(this, "if", false);
    //
    Compiler_pushScope(this, LETTER_IF);
    Compiler_consumeTree(this, ifNode);
    Compiler_popScope(this);
    // elseif
    ifNode = Queue_NEXT(leaf->leafs);
    if (ifNode && ifNode->type == UG_ATYPE_IF_M) {
        Compiler_checkJudge(this, ifNode);
        Compiler_callGenerateNewLine(this, "elif", false);
        //
        Compiler_pushScope(this, LETTER_ELIF);
        Compiler_consumeTree(this, ifNode);
        Compiler_popScope(this);
        //
        ifNode = Queue_NEXT(leaf->leafs);
        tools_assert(ifNode != NULL, LANG_ERR_GRAMMAR_INVALID_IF);
    }
    // else
    if (ifNode && ifNode->type == UG_ATYPE_IF_L)
    {
        Stack_RESTE(ifNode->tokens);
        Token *token = Stack_NEXT(ifNode->tokens);
        tools_assert(is_eq_string(token->value, LETTER_ELSE), LANG_ERR_GRAMMAR_INVALID_IF);
        Compiler_callGenerateNewLine(this, "else", false);
        //
        Compiler_pushScope(this, LETTER_ELSE);
        Compiler_consumeTree(this, ifNode);
        Compiler_popScope(this);
        //
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
    Draft_pushLine(this->draft, "");
    Compiler_callGenerateNewLine(this, "while", false);
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
    Compiler_buildPass(this, 3, target, iter1, iter2);
    Draft_pushLine(this->draft, "");
    Compiler_callGenerateNewLine(this, "spread", false);
    Compiler_pushScope(this, LETTER_WHILE);
    Compiler_consumeTree(this, leaf);
    Compiler_popScope(this);
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
    Compiler_callGenerateNewLine(this, "try-catch", false);
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
    Compiler_callGenerateNewLine(this, "define_appliable", false);
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

void _compiler_prepareApply(Compiler *this, Leaf *leaf)
{
    Stack_RESTE(leaf->tokens);
    Token *runnableName = Stack_NEXT(leaf->tokens);
    Token *resultName = Stack_NEXT(leaf->tokens);
    Compiler_buildPass(this, 2, runnableName, resultName);
    Token *arg = Stack_NEXT(leaf->tokens);
    while(arg != NULL)
    {
        Compiler_pushPass(this, arg);
        arg = Stack_NEXT(leaf->tokens);
    }
}

void Compiler_consumeApply(Compiler *this, Leaf *leaf)
{
    _compiler_prepareApply(this, leaf);
    Compiler_callGenerateNewLine(this, "apply", true);
}

void Compiler_consumeResult(Compiler *this, Leaf *leaf)
{
    Stack_RESTE(leaf->tokens);
    Token *result = Stack_NEXT(leaf->tokens);
    //
    this->isReturn = true;
    Compiler_callGenerateNewLine(this, "result", true);
}

void Compiler_consumeCalculator(Compiler *this, Leaf *leaf)
{
    Stack_RESTE(leaf->tokens);
    Token *body = Stack_NEXT(leaf->tokens);
    Token *target = Stack_NEXT(leaf->tokens);
    Foliage *root = (Foliage *)body->value;
    // 
    Compiler_callGenerateNewLine(this, "calculate", true);
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
    Compiler_callGenerateNewLine(this, "generate", true);
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

void *Compiler_compileCode(Compiler *this, Leaf *tree, CString dialect) {
    this->dialect = dialect;
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
