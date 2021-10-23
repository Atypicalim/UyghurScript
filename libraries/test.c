// library test

#include "../uyghur/uyghur.c"
 

void testFunc(Bridge *bridge)
{
    // // get arguments
    // printf("\n\n--c func arguments--\n");
    // printf("%s\n", Bridge_topType(bridge));
    // printf("%s\n", Bridge_popString(bridge));
    // printf("%f\n", Bridge_popNumber(bridge));
    // printf("%s\n", Bridge_popBoolean(bridge) ? "yes" : "no");
    // printf("%s\n", Bridge_topType(bridge));
    // // do something
    // printf("\nc function body from bridge\n");
    // // return result
    // printf("\n--c func result--\n");
    // Bridge_startResult(bridge);
    // Bridge_pushNumber(bridge, 778899);
    // Bridge_return(bridge);
}

void test_register(Bridge *bridge)
{
    // // register values from c
    // Bridge_startBox(bridge, NULL);
    // Bridge_pushKey(bridge, "cString");
    // Bridge_pushString(bridge, "c value from bridge!");
    // Bridge_pushKey(bridge, "cBoolean");
    // Bridge_pushBoolean(bridge, true);
    // Bridge_pushKey(bridge, "cNumber");
    // Bridge_pushNumber(bridge, 777);
    // Bridge_pushKey(bridge, "cFunction");
    // Bridge_pushFunction(bridge, testFunc);
    // Bridge_register(bridge);

    // // TODO rgister func to root scope in script
    // // use values from c
    // Bridge_startFunc(bridge, "funcName");
    // Bridge_pushString(bridge, "c value from bridge!");
    // Bridge_pushBoolean(bridge, true);
    // Bridge_pushNumber(bridge, 777);
    // Bridge_call(bridge);
}