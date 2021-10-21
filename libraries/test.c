// library test

#include "../uyghur/uyghur.c"

void test_register(Bridge *bridge)
{
    Bridge_reset(bridge);
    Bridge_pushString(bridge, "cString", "c value from bridge!");
    Bridge_pushBoolean(bridge, "cBoolean", true);
    Bridge_pushNumber(bridge, "cNumber", 777);
    Bridge_commit(bridge);

    // TODO: push function
}