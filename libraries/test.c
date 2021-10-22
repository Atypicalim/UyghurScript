// library test

#include "../uyghur/uyghur.c"
 

void testFunc(Queue *queue)
{
    // TODO pop args and handle
    printf("\nc function from bridge\n");
    // TODO push result to queue
}

void test_register(Bridge *bridge)
{
    Bridge_startBox(bridge, NULL);
    // Bridge_pushKey(bridge, "cString");
    // Bridge_pushString(bridge, "c value from bridge!");
    // Bridge_pushKey(bridge, "cBoolean");
    // Bridge_pushBoolean(bridge, true);
    // Bridge_pushKey(bridge, "cNumber");
    // Bridge_pushNumber(bridge, 777);
    Bridge_pushKey(bridge, "cFunction");
    Bridge_pushFunction(bridge, testFunc);
    Bridge_register(bridge);

    // TODO rgister func to root scope in script
    // Bridge_startFunc(bridge, "funcName");
    // Bridge_pushString(bridge, "c value from bridge!");
    // Bridge_pushBoolean(bridge, true);
    // Bridge_pushNumber(bridge, 777);
    // Bridge_call(bridge);

    // TODO: push function
}