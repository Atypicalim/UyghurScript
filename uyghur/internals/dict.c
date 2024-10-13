// dict

#include "../uyghur.c"

// count given dict size
void native_dict_count(Bridge *bridge)
{
    Dictable *dct = Bridge_receiveValue(bridge, UG_TYPE_DCT);
    int count = Dictable_getCount(dct);
    Bridge_returnNumber(bridge, count);
}
