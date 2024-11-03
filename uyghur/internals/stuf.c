// stuf

#include "../uyghur.c"

void native_stuf_place(Bridge *bridge)
{
    Value *value = Bridge_receiveValue(bridge, UG_TYPE_STF);
    char *path = value->extra;
    Bridge_returnString(bridge, path);
}

void native_stuf_type(Bridge *bridge)
{
    Value *value = Bridge_receiveValue(bridge, UG_TYPE_STF);
    char *name = value->token->value;
    Bridge_returnString(bridge, name);
}
