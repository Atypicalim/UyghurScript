// list

#include "../uyghur.c"

void native_list_count(Bridge *bridge)
{
    Value *lst = Bridge_receiveValue(bridge, UG_TYPE_LST);
    Value *wkr = Bridge_receiveValue(bridge, UG_TYPE_WKR);
    // 
    //
    Bridge_returnEmpty(bridge);
}

void lib_list_register(Bridge *bridge)
{
    Bridge_startBox(bridge);
    //
    BRIDGE_BIND_NATIVE(list_count);
    //
    Bridge_register(bridge, ALIAS_LIST);
}
