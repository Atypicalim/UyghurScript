// bridge

#include "uyghur.c"

// struct Params {
//     struct _Stack;
// };

#define BRIDGE_STACK_TP_BOX 1
#define BRIDGE_STACK_TP_FUN 2
#define BRIDGE_ITEM_TP_KEY "BRIDGE_ITEM_TP_KEY"
#define BRIDGE_ITEM_TP_VAL "BRIDGE_ITEM_TP_VAL"

struct Bridge
{
    Uyghur *uyghur;
    Stack *stack;
    char *name;
    int type;
    char *last;
};

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

void Bridge_pushValue(Bridge *this, Value *value)
{
    if (this->type == BRIDGE_STACK_TP_BOX)
    {
        tools_assert(this->last != BRIDGE_ITEM_TP_VAL, "invalid bridge status, key neceessary for value");
    }
    else if (this->type == BRIDGE_STACK_TP_FUN)
    {
        tools_assert(this->last != BRIDGE_ITEM_TP_KEY, "invalid bridge status, key unnecessary for value");
    }
    else
    {
        tools_error("invalid bridge status, not started");
    }
    Stack_push(this->stack, value);
    this->last = BRIDGE_ITEM_TP_VAL;
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

void Bridge_pushFunction(Bridge *this, void (*value)(Queue *))
{
    Bridge_pushValue(this,  Value_newCFunction(value, NULL));
}

void Bridge_register(Bridge *this)
{
    tools_assert(this->type == BRIDGE_STACK_TP_BOX, "invalid bridge status, box expected for register");
    tools_assert(this->last == BRIDGE_ITEM_TP_VAL, "invalid bridge status");
    Stack_reset(this->stack);
    Hashmap *map = this->name == NULL ? this->uyghur->executer->rootScope : Hashmap_new();
    Value *item = Stack_next(this->stack);
    while (item != NULL)
    {
        Value *value = item;
        Value *key = Stack_next(this->stack);
        Hashmap_set(map, key->string, value);
        item = Stack_next(this->stack);
    }
    if (this->name != NULL)
    {
        tools_error("TODO register box");
        // Hashmap_set(this->uyghur->executer->rootScope, this->name, Value_newObject(map, NULL));
    }
    Bridge_reset(this);
}

Value *Bridge_call(Bridge *this)
{
    tools_assert(this->type == BRIDGE_STACK_TP_FUN, "invalid bridge status, func expected for call");
    tools_assert(this->last != BRIDGE_ITEM_TP_KEY, "invalid bridge status, key unnecessary for call");
    tools_assert(this->name != NULL, "invalid bridge status, func name unnecessary for call");
    Stack_reset(this->stack);
    Executer *executer = this->uyghur->executer;
    Stack *stack = executer->callStack;
    Stack_clear(stack);
    Value *item = Stack_next(this->stack);
    while (item != NULL)
    {
        Stack_push(stack, item);
        item = Stack_next(this->stack);
    }
    Token *funcName = Token_new("", 0, 0, TTYPE_NAME, this->name);
    Value *result = Executer_runFunc(executer, funcName);
    Bridge_reset(this);
    return result;
}
