// list

#include "../uyghur.c"

void native_list_count(Bridge *bridge)
{
    Listable *lst = Bridge_receiveValue(bridge, UG_TYPE_LST);
    int count = Listable_getCount(lst);
    Bridge_returnNumber(bridge, count);
}

void lib_list_register(Bridge *bridge)
{
    Bridge_startBox(bridge);
    //
    BRIDGE_BIND_NATIVE(list_count);
    //
    Bridge_register(bridge, TVALUE_LST);
}
