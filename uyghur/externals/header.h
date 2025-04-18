// header

#ifndef H_LIB_EXTERNAL_HEADER
#define H_LIB_EXTERNAL_HEADER

#ifndef BUILDER_NO_BACKEND
#include "../../build/bind/externals/stage.c"
#include "../../build/bind/externals/mouse.c"
#include "../../build/bind/externals/board.c"
#endif
#include "../../build/bind/externals/paper.c"
#include "../../build/bind/externals/pencil.c"
#include "../../build/bind/externals/music.c"
#include "../../build/bind/externals/sound.c"
#include "../../build/bind/externals/dialog.c"
// #include "../../build/bind/externals/control.c"
// #include "../../build/bind/externals/voyager.c"


void cFuncTest(Bridge *bridge)
{
    // // get arguments
    // printf("\n\n--c func arguments--\n");
    // printf("%s\n", Bridge_topType(bridge));
    // printf("%s\n", Bridge_receiveBoolean(bridge) ? "yes" : "no");
    // printf("%f\n", Bridge_receiveNumber(bridge));
    // printf("%s\n", Bridge_receiveString(bridge));
    // printf("%s\n", Bridge_topType(bridge));
    // // do something
    // printf("\nc function body from bridge\n");
    // Bridge_returnNumbers(bridge, 2, 111, 222);
    Bridge_returnString(bridge, "abc...");
}

void lib_test_func(Bridge *bridge)
{
    // // register variables from c (NUll to register globals, name to register a box)
    Bridge_startBox(bridge);
    // Bridge_bindString(bridge, "cString", "c value from bridge!");
    // Bridge_bindBoolean(bridge, "cBoolean", true);
    // Bridge_bindNumber(bridge, "cNumber", 123);
    Bridge_bindNative(bridge, "cFuncTest", cFuncTest);
    Bridge_register(bridge, NULL);
}

void lib_test_call(Bridge *bridge)
{
    // // use values from c
    // Bridge_startFunc(bridge);
    // Bridge_pushNumber(bridge, 333);
    // Bridge_pushNumber(bridge, 222);
    // Bridge_pushBoolean(bridge, true);
    // Bridge_pushString(bridge, "c value from bridge!");
    // Bridge_call(bridge, "uyghurFuncTest");
    // // get result
    // printf("\n\n--uyghur func result--\n");
    // printf("%s\n", Bridge_topType(bridge));
    // printf("%f\n", Bridge_receiveNumber(bridge));
}

void register_external_libraries(Bridge *bridge)
{
    //
    #ifndef BUILDER_NO_BACKEND
    lib_stage_register(bridge);
    lib_mouse_register(bridge);
    lib_board_register(bridge);
    #endif
    lib_paper_register(bridge);
    lib_pencil_register(bridge);
    lib_music_register(bridge);
    lib_sound_register(bridge);
    lib_dialog_register(bridge);
    // lib_control_register(bridge);
    // lib_voyager_register(bridge);
}

#endif
