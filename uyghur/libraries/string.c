// string

#include "../uyghur.c"

void test_string(Bridge *bridge)
{
    Value *value = Bridge_popValue(bridge);
    // todo
    Bridge_startResult(bridge);
    Bridge_pushValue(bridge, NULL);
    Bridge_return(bridge);
}

void lib_string_register(Bridge *bridge)
{
    Bridge_startBox(bridge, NULL);
    //
    Bridge_pushKey(bridge, "xetSinaq");
    Bridge_pushFunction(bridge, test_string);
    //
    Bridge_register(bridge);
}
