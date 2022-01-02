// library test

#include "../uyghur/uyghur.c"
 

void cFuncTest(Bridge *bridge)
{
    // // get arguments
    // printf("\n\n--c func arguments--\n");
    // printf("%s\n", Bridge_topType(bridge));
    // printf("%s\n", Bridge_nexString(bridge));
    // printf("%f\n", Bridge_nexNumber(bridge));
    // printf("%s\n", Bridge_nexBoolean(bridge) ? "yes" : "no");
    // printf("%s\n", Bridge_topType(bridge));
    // // do something
    // printf("\nc function body from bridge\n");
    // // return result
    // printf("\n--c func result--\n");
    // Bridge_startResult(bridge);
    // Bridge_pushNumber(bridge, 778899);
    // Bridge_return(bridge);
}

void lib_test_register(Bridge *bridge)
{
    // // register variables from c (NUll to register globals, name to register a box)
    // Bridge_startBox(bridge, NULL);
    // Bridge_pushKey(bridge, "cString");
    // Bridge_pushString(bridge, "c value from bridge!");
    // Bridge_pushKey(bridge, "cBoolean");
    // Bridge_pushBoolean(bridge, true);
    // Bridge_pushKey(bridge, "cNumber");
    // Bridge_pushNumber(bridge, 777);
    // Bridge_pushKey(bridge, "cFuncTest");
    // Bridge_pushFunction(bridge, cFuncTest);
    // Bridge_register(bridge);
}

void lib_test_call(Bridge *bridge)
{
    // // use values from c
    // Bridge_startFunc(bridge, "uyghurFuncTest");
    // Bridge_pushNumber(bridge, 333);
    // Bridge_pushNumber(bridge, 222);
    // Bridge_pushBoolean(bridge, true);
    // Bridge_pushString(bridge, "c value from bridge!");
    // Bridge_call(bridge);
    // // get result
    // printf("\n\n--uyghur func result--\n");
    // printf("%s\n", Bridge_topType(bridge));
    // printf("%f\n", Bridge_nexNumber(bridge));
}
