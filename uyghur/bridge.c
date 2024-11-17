// bridge

#include "uyghur.c"

// struct Params {
//     struct _Stack;
// };

#define BRIDGE_ITEM_TP_KEY "BRIDGE_ITEM_TP_KEY"
#define BRIDGE_ITEM_TP_VAL "BRIDGE_ITEM_TP_VAL"

void Bridge_reset(Bridge *this)
{
    Value *value = Stack_pop(this->stack);
    while(value != NULL)
    {
        Object *o = (Object *)value;
        Machine_releaseObj(value);
        value = Stack_pop(this->stack);
    }
    Stack_RESTE(this->stack);
    this->type = 0;
    this->last = NULL;
}

Bridge *Bridge_new(Uyghur *uyghur)
{
    Bridge *bridge = malloc(sizeof(Bridge));
    bridge->uyghur = uyghur;
    bridge->stack = Stack_new(IS_RETAIN_VALUES);
    bridge->type = 0;
    bridge->last = NULL;
    return bridge;
}

// get top type of the stack
char Bridge_topType(Bridge *this)
{
    Block *tail = this->stack->tail;
    return tail != NULL ? ((Value *)tail->data)->type : UG_TYPE_NON;
}

// pop next value, you should release the value object manually
Value *Bridge_popValue(Bridge *this)
{
    Value *v = Stack_pop(this->stack);
    if (v == NULL) return Value_newEmpty(NULL);
    return v;
}

// nullable: get next value, value object auto released when reset the stack
Value *Bridge_nextValue(Bridge *this)
{
    tools_assert(this->stack != NULL, "invalid bridge status, stack required for values");
    return Stack_NEXT(this->stack);
}

// key value

void Bridge_pushKey(Bridge *this, char *key)
{
    tools_assert(this->type == BRIDGE_STACK_TP_BOX, "invalid bridge status, key available for only box");
    tools_assert(this->last != BRIDGE_ITEM_TP_KEY, "invalid bridge status, key neceessary for last value");
    Value *keyValue = Value_newString(key, NULL);
    Stack_push(this->stack, keyValue);
    this->last = BRIDGE_ITEM_TP_KEY;
}

void Bridge_pushValue(Bridge *this, Value *value)
{
    tools_assert(this->type > 0, "invalid bridge status, not started");
    if (this->type == BRIDGE_STACK_TP_BOX) {
        tools_assert(this->last != BRIDGE_ITEM_TP_VAL, "invalid bridge status, key neceessary for value");
    } else {
        tools_assert(this->last != BRIDGE_ITEM_TP_KEY, "invalid bridge status, key unnecessary for value");
    }
    Stack_push(this->stack, value);
    this->last = BRIDGE_ITEM_TP_VAL;
}

// push variables

void Bridge_pushEmpty(Bridge *this)
{
    Bridge_pushValue(this, Value_newEmpty(NULL));
}

void Bridge_pushBoolean(Bridge *this, bool value)
{
    Bridge_pushValue(this, Value_newBoolean(value, NULL));
}

void Bridge_pushNumber(Bridge *this, double value)
{
    Bridge_pushValue(this, Value_newNumber(value, NULL));
}

void Bridge_pushString(Bridge *this, char *value)
{
    Bridge_pushValue(this, Value_newString(value, NULL));
}

// rgister box or globals to script

void Bridge_startBox(Bridge *this)
{
    Bridge_reset(this);
    this->type = BRIDGE_STACK_TP_BOX;
}

void Bridge_bindBoolean(Bridge *this, CName name, bool bol)
{
    Bridge_pushKey(this, name);
    Bridge_pushValue(this, Value_newBoolean(bol, NULL));
}

void Bridge_bindNumber(Bridge *this, CName name, double num)
{
    Bridge_pushKey(this, name);
    Bridge_pushValue(this, Value_newNumber(num, NULL));
}

void Bridge_bindString(Bridge *this, CName name, char *str)
{
    Bridge_pushKey(this, name);
    Bridge_pushValue(this, Value_newString(str, NULL));
}

void Bridge_bindNative(Bridge *this, CName name, NATIVE fun)
{
    Bridge_pushKey(this, name);
    Token *token = Token_new(UG_TTYPE_NAM, name);
    token->file = "";
    token->line = 0;
    token->column = 0;
    Bridge_pushValue(this, Runnable_newNative(fun, name));
}

void Bridge_bindNativeExt(Bridge *this, CName name, NATIVE fun, CString file, int line)
{
    Bridge_pushKey(this, name);
    Token *token = Token_new(UG_TTYPE_NAM, name);
    token->file = file;
    token->line = line;
    token->column = 0;
    Bridge_pushValue(this, Runnable_newNative(fun, token));
}

void Bridge_register(Bridge *this, char *boxName)
{
    tools_assert(this->type == BRIDGE_STACK_TP_BOX, "invalid bridge status, box expected for register");
    tools_assert(this->last == BRIDGE_ITEM_TP_VAL, "invalid bridge status");
    Machine *machine = this->uyghur->machine;
    Holdable *global = this->uyghur->executer->globalScope;
    // 
    Holdable *holdable = NULL;
    if (holdable == NULL && boxName == NULL) holdable = global;
    if (holdable == NULL) holdable = Machine_readKind(machine, boxName);
    if (holdable == NULL) holdable = Machine_readProxy(machine, boxName);
    if (holdable == NULL) {
        holdable = Holdable_newProxy(boxName);
        Machine_retainObj(holdable);
        helper_set_aliased_key(global, boxName, holdable);
    }
    // 
    Value *value = Stack_pop(this->stack);
    while (value != NULL)
    {
        Value *key = Stack_pop(this->stack);
        char *_key =  key->string;
        helper_set_aliased_key(holdable, _key, value);
        Machine_releaseObj(key);
        value = Stack_pop(this->stack);
    }
}

// send native call arguments from script

void Bridge_startArgument(Bridge *this)
{
    Bridge_reset(this);
    this->type = BRIDGE_STACK_TP_ARG;
}

void *Bridge_send(Bridge *this)
{
    tools_assert(this->type == BRIDGE_STACK_TP_ARG, "invalid bridge status, argument expected for send");
    Stack_RESTE(this->stack);
}

// return native call results to script

void Bridge_startResult(Bridge *this)
{
    Bridge_reset(this);
    this->type = BRIDGE_STACK_TP_RES;
}

void *Bridge_return(Bridge *this)
{
    tools_assert(this->type == BRIDGE_STACK_TP_RES, "invalid bridge status, result expected for return");
    if (this->stack->head == NULL) Bridge_pushValue(this, Value_newEmpty(NULL));
    tools_assert(this->stack->head == this->stack->tail, "invalid bridge status, can return only one value");
}

// receive arguments from script to c

char Bridge_nextType(Bridge *this) {
    Block *temp = Cursor_get(this->stack->cursor);
    if (temp == NULL) return UG_TYPE_NON;
    Value *data = temp->data;
    if (data == NULL) return UG_TYPE_NON;
    return data->type;
}

Value *Bridge_receiveValue(Bridge *this, char tp)
{
    Value *v = Bridge_nextValue(this);
    if (v == NULL) v = Value_newEmpty(NULL);
    if (tp != UG_TYPE_NON) {
        tools_assert(v->type == tp, "invalid bridge arguments, %c argument not found", tp);
    }
    return v;
}

void Bridge_receiveEmpty(Bridge *this)
{
    Value *v = Bridge_receiveValue(this, UG_TYPE_NIL);
}

bool Bridge_receiveBoolean(Bridge *this)
{
    Value *v = Bridge_receiveValue(this, UG_TYPE_BOL);
    return v->boolean;
}

bool Bridge_receiveBooleanWithDefault(Bridge *this, bool _default)
{
    Value *v = Bridge_nextValue(this);
    return Value_isBoolean(v) ? v->boolean : _default;
}

double Bridge_receiveNumber(Bridge *this)
{
    Value *v = Bridge_receiveValue(this, UG_TYPE_NUM);
    return v->number;
}

double Bridge_receiveNumberWithDefault(Bridge *this, double _default)
{
    Value *v = Bridge_nextValue(this);
    return Value_isNumber(v) ? v->number : _default;
}

char *Bridge_receiveString(Bridge *this)
{
    Value *v = Bridge_receiveValue(this, UG_TYPE_STR);
    return v->string;
}

char *Bridge_receiveStringWithDefault(Bridge *this, CString _default)
{
    Value *v = Bridge_nextValue(this);
    return Value_isString(v) ? v->string : _default;
}

// return result to script from c

void Bridge_returnValue(Bridge *this, Value *value)
{
    Bridge_startResult(this);
    Bridge_pushValue(this, value);
    Bridge_return(this);
}

void Bridge_returnEmpty(Bridge *this)
{
    Value *v = Value_newEmpty(NULL);
    Bridge_returnValue(this, v);
}

void Bridge_returnBoolean(Bridge *this, bool val)
{
    Value *v = Value_newBoolean(val, NULL);
    Bridge_returnValue(this, v);
}

void Bridge_returnNumber(Bridge *this, double val)
{
    Value *v = Value_newNumber(val, NULL);
    Bridge_returnValue(this, v);
}

void Bridge_returnString(Bridge *this, char *val)
{
    Value *v = val == NULL ? Value_newEmpty(NULL) : Value_newString(val, NULL);
    Bridge_returnValue(this, v);
}

// return results to script from c

void Bridge_returnBooleans(Bridge *this, int num, bool val, ...)
{
    Bridge_startResult(this);
    va_list args;
    va_start(args, val);
    int i;
    for (i = 0; i < num; i++) {
        Bridge_pushBoolean(this, val);
       val = va_arg(args, int);
    }
    va_end(args);
    Bridge_return(this);
}

void Bridge_returnNumbers(Bridge *this, int num, double val, ...)
{
    Bridge_startResult(this);
    va_list args;
    va_start(args, val);
    int i;
    for (i = 0; i < num; i++) {
        Bridge_pushNumber(this, val);
       val = va_arg(args, double);
    }
    va_end(args);
    Bridge_return(this);
}

void Bridge_returnStrings(Bridge *this, int num, char *val, ...)
{
    Bridge_startResult(this);
    va_list args;
    va_start(args, val);
    int i;
    for (i = 0; i < num; i++) {
        Bridge_pushString(this, val);
       val = va_arg(args, char *);
    }
    va_end(args);
    Bridge_return(this);
}

// call script func fom c

void Bridge_startFunc(Bridge *this)
{
    Bridge_reset(this);
    this->type = BRIDGE_STACK_TP_FUN;
}

void Bridge_call(Bridge *this, char *funcName)
{
    tools_assert(funcName != NULL, "invalid bridge status, func name not found for call");
    tools_assert(this->type == BRIDGE_STACK_TP_FUN, "invalid bridge status, func expected for call");
    tools_assert(this->last != BRIDGE_ITEM_TP_KEY, "invalid bridge status, key unnecessary for call");
    Stack_RESTE(this->stack);
    // arguments
    Executer *executer = this->uyghur->executer;
    Stack *stack = executer->callStack;
    Stack_clear(stack);
    Value *item = Stack_NEXT(this->stack);
    while (item != NULL)
    {
        Executer_pushStack(executer, item);
        Object *o = (Object *)item;
        item = Stack_NEXT(this->stack);
    }
    // execute
    Value *r = NULL;
    //
    Value *f = helper_get_aliased_key(executer->globalScope, funcName);
    if (f == NULL) {
        log_warn("%s:%s", LANG_ERR_BRIDGE_FUNCTION_NOT_FOUND, funcName);
    } else if (Runnable_isWorker(f)) {
        r = Executer_applyWorker(executer, Token_empty(), f, NULL);
    } else {
        tools_error("%s:%s", LANG_ERR_BRIDGE_FUNCTION_NOT_VALID, funcName);
    }
    // result
    Stack_clear(stack);
    Bridge_startResult(this);
    if (r) Bridge_pushValue(this, r);
    Bridge_return(this);
    Machine_tryGC(this->uyghur->machine);
}

void Bridge_run(Bridge *this, Value *value) {
    void *func = value->obj;
    void (*function)() = func;
    function(this);
}
