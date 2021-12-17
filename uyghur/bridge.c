// bridge

#include "uyghur.c"

// struct Params {
//     struct _Stack;
// };

#define BRIDGE_ITEM_TP_KEY "BRIDGE_ITEM_TP_KEY"
#define BRIDGE_ITEM_TP_VAL "BRIDGE_ITEM_TP_VAL"

void Bridge_reset(Bridge *this)
{
    this->stack = Stack_new();
    this->name = NULL;
    this->type = 0;
    this->last = NULL;
}

void Bridge_startBox(Bridge *this, char *name)
{
    Bridge_reset(this);
    this->name = name;
    this->type = BRIDGE_STACK_TP_BOX;
}

void Bridge_startFunc(Bridge *this, char *name)
{
    Bridge_reset(this);
    this->name = name;
    this->type = BRIDGE_STACK_TP_FUN;
}

void Bridge_startArgument(Bridge *this)
{
    Bridge_reset(this);
    this->type = BRIDGE_STACK_TP_ARG;
}

void Bridge_startResult(Bridge *this)
{
    Bridge_reset(this);
    this->type = BRIDGE_STACK_TP_RES;
}

Bridge *Bridge_new(Uyghur *uyghur)
{
    Bridge *bridge = malloc(sizeof(Bridge));
    bridge->uyghur = uyghur;
    Bridge_reset(bridge);
    return bridge;
}

void Bridge_pushKey(Bridge *this, char *key)
{
    tools_assert(this->type == BRIDGE_STACK_TP_BOX, "invalid bridge status, key available for only box");
    tools_assert(this->last != BRIDGE_ITEM_TP_KEY, "invalid bridge status, key neceessary for last value");
    Stack_push(this->stack, Value_newString(key, NULL));
    this->last = BRIDGE_ITEM_TP_KEY;
}

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

Value *Bridge_popValue(Bridge *this)
{
    return Stack_pop(this->stack);
}

Value *Bridge_popValueForRType(Bridge *this, char *tp)
{

    Value *v = Bridge_popValue(this);
    tools_assert(v != NULL, "invalid bridge arguments, next argument not found");
    tools_assert(is_equal(v->type, tp), "invalid bridge arguments, %s argument not found", tp);
    return v;
}

void Bridge_pushBoolean(Bridge *this, bool value)
{
    Bridge_pushValue(this, Value_newBoolean(value, NULL));
}

bool Bridge_popBoolean(Bridge *this)
{
    return Bridge_popValueForRType(this, RTYPE_BOOLEAN)->boolean;
}

void Bridge_pushNumber(Bridge *this, double value)
{
    Bridge_pushValue(this, Value_newNumber(value, NULL));
}

double Bridge_popNumber(Bridge *this)
{
    return Bridge_popValueForRType(this, RTYPE_NUMBER)->number;
}

void Bridge_pushString(Bridge *this, char *value)
{
    Bridge_pushValue(this, Value_newString(value, NULL));
}

char *Bridge_popString(Bridge *this)
{
    return Bridge_popValueForRType(this, RTYPE_STRING)->string;
}

void Bridge_pushFunction(Bridge *this, void (*value)(Bridge *))
{
    Bridge_pushValue(this,  Value_newCFunction(value, NULL));
}

void Bridge_register(Bridge *this)
{
    tools_assert(this->type == BRIDGE_STACK_TP_BOX, "invalid bridge status, box expected for register");
    tools_assert(this->last == BRIDGE_ITEM_TP_VAL, "invalid bridge status");
    Stack_reset(this->stack);
    Hashmap *container = this->name == NULL ? this->uyghur->executer->globalScope : Hashmap_new();
    Value *item = Stack_next(this->stack);
    while (item != NULL)
    {
        Value *value = item;
        Value *key = Stack_next(this->stack);
        Hashmap_set(container, key->string, value);
        item = Stack_next(this->stack);
    }
    if (this->name != NULL)
    {
        tools_error("TODO register box");
        // Hashmap_set(this->uyghur->executer->rootScope, this->name, Value_newObject(map, NULL));
    }
    Bridge_reset(this);
}

void Bridge_call(Bridge *this)
{
    tools_assert(this->type == BRIDGE_STACK_TP_FUN, "invalid bridge status, func expected for call");
    tools_assert(this->last != BRIDGE_ITEM_TP_KEY, "invalid bridge status, key unnecessary for call");
    tools_assert(this->name != NULL, "invalid bridge status, func name unnecessary for call");
    Stack_reset(this->stack);
    // arguments
    Executer *executer = this->uyghur->executer;
    Stack *stack = executer->callStack;
    Stack_clear(stack);
    Value *item = Stack_next(this->stack);
    while (item != NULL)
    {
        Stack_push(stack, item);
        item = Stack_next(this->stack);
    }
    // execute
    Token *funcName = Token_new(TTYPE_NAME, this->name);
    Value *result = Executer_runFunc(executer, funcName);
    // result
    Bridge_startResult(this);
    Bridge_pushValue(this,result);
    Bridge_return(this);
}

void *Bridge_send(Bridge *this)
{
    tools_assert(this->type == BRIDGE_STACK_TP_ARG, "invalid bridge status, argument expected for send");
    Stack_reset(this->stack);
    Value *v = Stack_next(this->stack);
    while(v != NULL)
    {
        if (!is_values(v->type, RTYPE_GROUP_BASE))
        {
            tools_error("invalid bridge status, type %s not available in c", v->type);
        }
        v = Stack_next(this->stack);
    }
}

void *Bridge_return(Bridge *this)
{
    tools_assert(this->type == BRIDGE_STACK_TP_RES, "invalid bridge status, result expected for return");
    if (this->stack->head == NULL) Bridge_pushValue(this, Value_newEmpty(NULL));
    tools_assert(this->stack->head == this->stack->tail, "invalid bridge status, can return only one value");
}
