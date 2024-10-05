// dict

#include "../uyghur.c"

// count given dict size
void native_dict_count(Bridge *bridge)
{
    Dictable *dct = Bridge_receiveValue(bridge, UG_TYPE_DCT);
    int count = Dictable_getCount(dct);
    Bridge_returnNumber(bridge, count);
}

void lib_dict_register(Bridge *bridge)
{
    Bridge_startBox(bridge);
    //
    BRIDGE_BIND_NATIVE(dict_count);
    //
    Bridge_register(bridge, TVALUE_DCT);
}
