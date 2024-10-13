// list

#include "../uyghur.c"

void native_list_count(Bridge *bridge)
{
    Listable *lst = Bridge_receiveValue(bridge, UG_TYPE_LST);
    int count = Listable_getCount(lst);
    Bridge_returnNumber(bridge, count);
}
