// dict

#include "../uyghur.c"

void native_dict_count(Bridge *bridge)
{
    Value *dct = Bridge_receiveValue(bridge, UG_TYPE_DCT);
    Value *wkr = Bridge_receiveValue(bridge, UG_TYPE_WKR);
    //
    //
    Bridge_returnEmpty(bridge);
}

void lib_dict_register(Bridge *bridge)
{
    Bridge_startBox(bridge);
    //
    BRIDGE_BIND_NATIVE(dict_count);
    //
    Bridge_register(bridge, ALIAS_DICT);
}
