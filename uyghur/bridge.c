// bridge

#include "uyghur.c"

// struct Params {
//     struct _Stack;
// };

#define BRIDGE_ITEM_TP_KEY "BRIDGE_ITEM_TP_KEY"
#define BRIDGE_ITEM_TP_VAL "BRIDGE_ITEM_TP_VAL"

void _bridge_release_stack(Bridge* this)
{
    Value *value = Stack_pop(this->stack);
    while(value != NULL)
    {
        Value_free(value);
        value = Stack_pop(this->stack);
    }
}

void _bridge_reset_stack(Bridge* this)
{
    if (this->cursor != NULL)
    {
        free(this->cursor);
        this->cursor = NULL;
    }
    this->cursor = Stack_reset(this->stack);
}

void Bridge_reset(Bridge *this)
{
    // TODO: ug free unused poiters
    Stack_free(this->stack);
    this->stack = Stack_new(); // make a call stack containing stack: call cFunc in ugCallback
    if (this->cursor != NULL)
    {
        free(this->cursor);
        this->cursor = NULL;
    }
    if (this->name != NULL)
    {
        free(this->name);
        this->name = NULL;
    }
    this->type = 0;
    this->last = NULL;
}

Bridge *Bridge_new(Uyghur *uyghur)
{
    Bridge *bridge = malloc(sizeof(Bridge));
    bridge->uyghur = uyghur;
    bridge->stack = Stack_new();
    bridge->cursor = NULL;
    bridge->name = NULL;
    bridge->type = 0;
    bridge->last = NULL;
    return bridge;
}

// get top type of the stack

char *Bridge_topType(Bridge *this)
{
    Block *tail = this->stack->tail;
    Value *value = Value_newEmpty(NULL);
    if (tail != NULL)
    {
        value = tail->data;
    }
    return value->type;
}

void Bridge_pushValue(Bridge *this, Value *value)
{
    tools_assert(this->type > 0, "invalid bridge status, not started");
    if (this->type == BRIDGE_STACK_TP_BOX)
    {
        tools_assert(this->last != BRIDGE_ITEM_TP_VAL, "invalid bridge status, key neceessary for value");
    }
    else
    {
        tools_assert(this->last != BRIDGE_ITEM_TP_KEY, "invalid bridge status, key unnecessary for value");
    }
    Stack_push(this->stack, value);
    this->last = BRIDGE_ITEM_TP_VAL;
}

// pop next value, you should release the value object manually
Value *Bridge_popValue(Bridge *this)
{
    Value *v = Stack_pop(this->stack);
    if (v == NULL) return Value_newEmpty(NULL);
    return v;
}

// get next value, value object auto released when reset the stack
Value *Bridge_nextValue(Bridge *this)
{
    if (this->cursor == NULL) return Value_newEmpty(NULL);
    Value *v = Stack_next(this->stack, this->cursor);
    if (v == NULL) return Value_newEmpty(NULL);
    return v;
}

// push key

void Bridge_pushKey(Bridge *this, char *key)
{
    tools_assert(this->type == BRIDGE_STACK_TP_BOX, "invalid bridge status, key available for only box");
    tools_assert(this->last != BRIDGE_ITEM_TP_KEY, "invalid bridge status, key neceessary for last value");
    Stack_push(this->stack, Value_newString(str_new(key), NULL));
    this->last = BRIDGE_ITEM_TP_KEY;
}

// push value

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
    Bridge_pushValue(this, Value_newString(str_new(value), NULL));
}

void Bridge_pushFunction(Bridge *this, void (*value)(Bridge *))
{
    Bridge_pushValue(this, Value_newCFunction(value, NULL));
}

// read variables

bool Bridge_nextBoolean(Bridge *this)
{
    Value *v = Bridge_nextValue(this);
    tools_assert(is_equal(v->type, RTYPE_BOOLEAN), "invalid bridge arguments, %s argument not found", RTYPE_BOOLEAN);
    return v->boolean;
}

double Bridge_nextNumber(Bridge *this)
{
    Value *v = Bridge_nextValue(this);
    tools_assert(is_equal(v->type, RTYPE_NUMBER), "invalid bridge arguments, %s argument not found", RTYPE_NUMBER);
    return v->number;
}

char *Bridge_nextString(Bridge *this)
{
    Value *v = Bridge_nextValue(this);
    tools_assert(is_equal(v->type, RTYPE_STRING), "invalid bridge arguments, %s argument not found", RTYPE_STRING);
    return v->string;
}

// rgister box or globals to script

void Bridge_startBox(Bridge *this, char *name)
{
    _bridge_release_stack(this);
    Bridge_reset(this);
    this->name = name == NULL ? NULL : str_new(name);
    this->type = BRIDGE_STACK_TP_BOX;
}

void Bridge_register(Bridge *this)
{
    tools_assert(this->type == BRIDGE_STACK_TP_BOX, "invalid bridge status, box expected for register");
    tools_assert(this->last == BRIDGE_ITEM_TP_VAL, "invalid bridge status");
    Cursor *cursor = Stack_reset(this->stack);
    Container *container = this->name == NULL ? this->uyghur->executer->globalContainer : Container_newBox();
    Value *item = Stack_next(this->stack, cursor);
    while (item != NULL)
    {
        Value *value = item;
        Value *key = Stack_next(this->stack, cursor);
        // Value_print(key);
        Container_set(container, key->string, value);
        item = Stack_next(this->stack, cursor);
    }
    Cursor_free(cursor);
    if (this->name != NULL)
    {
        Container_set(this->uyghur->executer->globalContainer, this->name, Value_newBox(container, NULL));
    }
    this->name = NULL;
    Bridge_reset(this);
}

// send arguments to c from script

void Bridge_startArgument(Bridge *this)
{
    _bridge_release_stack(this);
    Bridge_reset(this);
    this->type = BRIDGE_STACK_TP_ARG;
}

void *Bridge_send(Bridge *this)
{
    tools_assert(this->type == BRIDGE_STACK_TP_ARG, "invalid bridge status, argument expected for send");
    Cursor *cursor = Stack_reset(this->stack);
    Value *v = Stack_next(this->stack, cursor);
    while(v != NULL)
    {
        if (!is_values(v->type, RTYPE_GROUP_BASE))
        {
            tools_error("invalid bridge status, type %s not available in c", v->type);
        }
        v = Stack_next(this->stack, cursor);
    }
    Cursor_free(cursor);
    _bridge_reset_stack(this);
}

// return result to script from c

void Bridge_startResult(Bridge *this)
{
    _bridge_release_stack(this);
    Bridge_reset(this);
    this->type = BRIDGE_STACK_TP_RES;
}

void *Bridge_return(Bridge *this)
{
    tools_assert(this->type == BRIDGE_STACK_TP_RES, "invalid bridge status, result expected for return");
    if (this->stack->head == NULL) Bridge_pushValue(this, Value_newEmpty(NULL));
    tools_assert(this->stack->head == this->stack->tail, "invalid bridge status, can return only one value");
}

// call script func fom c

void Bridge_startFunc(Bridge *this, char *name)
{
    _bridge_release_stack(this);
    Bridge_reset(this);
    this->name = name == NULL ? NULL : str_new(name);
    this->type = BRIDGE_STACK_TP_FUN;
}

void Bridge_call(Bridge *this)
{
    tools_assert(this->type == BRIDGE_STACK_TP_FUN, "invalid bridge status, func expected for call");
    tools_assert(this->last != BRIDGE_ITEM_TP_KEY, "invalid bridge status, key unnecessary for call");
    tools_assert(this->name != NULL, "invalid bridge status, func name unnecessary for call");
    Cursor *cursor = Stack_reset(this->stack);
    // arguments
    Executer *executer = this->uyghur->executer;
    Stack *stack = executer->callStack;
    Stack_clear(stack);
    Value *item = Stack_next(this->stack, cursor);
    while (item != NULL)
    {
        Stack_push(stack, item);
        item = Stack_next(this->stack, cursor);
    }
    Cursor_free(cursor);
    // execute
    Token *funcName = Token_key(this->name, "_", TTYPE_STRING);
    Value *funcValue = Executer_getTRValue(executer, funcName, true);
    Value *r = NULL;
    if (is_equal(funcValue->type, RTYPE_FUNCTION))
    {
        r = Executer_runFunc(executer, funcName);
    }
    else
    {
        tools_warning("function not found for func name: %s", funcName->value);
        r = Value_newEmpty(NULL);
    }
    if (this->name != NULL)
    {
        free(this->name);
        this->name = NULL;
    }
    Token_free(funcName);
    // result
    Bridge_startResult(this);
    Bridge_pushValue(this, r);
    Bridge_return(this);
    // // TODO: ug free result when call
}
