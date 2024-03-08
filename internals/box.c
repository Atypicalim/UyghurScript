// box

#include "../uyghur/uyghur.c"

void test_box(Bridge *bridge)
{
    // todo
    Bridge_returnEmpty(bridge);
}

void lib_box_register(Bridge *bridge)
{
    Bridge_startBox(bridge, NULL);
    //
    Bridge_bindNative(bridge, "sanduqSinaq", test_box);
    //
    Bridge_register(bridge);
}
