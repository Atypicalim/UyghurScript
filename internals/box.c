// box

#include "../uyghur/uyghur.c"

void test_box(Bridge *bridge)
{
    Value *value = Bridge_nextValue(bridge);
    // todo
    Bridge_startResult(bridge);
    Bridge_pushValue(bridge, NULL);
    Bridge_return(bridge);
}

void lib_box_register(Bridge *bridge)
{
    Bridge_startBox(bridge, NULL);
    //
    Bridge_pushKey(bridge, "sanduqSinaq");
    Bridge_pushFunction(bridge, test_box);
    //
    Bridge_register(bridge);
}
