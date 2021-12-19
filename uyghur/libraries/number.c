// number

#include "../uyghur.c"

void test_number(Bridge *bridge)
{
    Value *value = Bridge_popValue(bridge);
    // todo
    Bridge_startResult(bridge);
    Bridge_pushValue(bridge, NULL);
    Bridge_return(bridge);
}

void lib_number_register(Bridge *bridge)
{
    Bridge_startBox(bridge, NULL);
    //
    Bridge_pushKey(bridge, "sanSinaq");
    Bridge_pushFunction(bridge, test_number);
    //
    Bridge_register(bridge);
}
