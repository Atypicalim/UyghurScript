// box

#include "../uyghur/uyghur.c"

void test_box(Bridge *bridge)
{
    Value *box = Bridge_receiveValue(bridge, UG_TYPE_CNT);
    Value *fun = Bridge_receiveValue(bridge, UG_TYPE_FUN);
    
    printf("test:\n");
    Value_print(box);
    Value_print(fun);
    // todo
    Bridge_returnEmpty(bridge);
}

void lib_box_register(Bridge *bridge)
{
    Bridge_startBox(bridge, "sanduq");
    //
    Bridge_bindNative(bridge, "sanduqSinaq", test_box);
    //
    Bridge_register(bridge);
}
