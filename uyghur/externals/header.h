// header

#ifndef H_LIB_EXTERNAL_HEADER
#define H_LIB_EXTERNAL_HEADER

#ifndef UG_NO_EXTERNALS
#ifndef UG_NO_STAGE
#include "../../build/bind/externals/stage.c"
#include "../../build/bind/externals/mouse.c"
#include "../../build/bind/externals/board.c"
#endif
#include "../../build/bind/externals/paper.c"
#include "../../build/bind/externals/pencil.c"
#include "../../build/bind/externals/music.c"
#include "../../build/bind/externals/sound.c"
#include "../../build/bind/externals/dialog.c"
#endif
// #include "../../build/bind/externals/control.c"
// #include "../../build/bind/externals/voyager.c"

void register_external_libraries(Bridge *bridge)
{
    //
    #ifndef UG_NO_EXTERNALS
    #ifndef UG_NO_STAGE
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
    #endif
}

#endif
