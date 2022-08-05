// header

#ifndef H_LIB_EXTERNAL_HEADER
#define H_LIB_EXTERNAL_HEADER

#include "board.c"
#include "music.c"
#include "sound.c"
#include "painter.c"
#include "control.c"
#include "test.c"

void register_external_libraries(Bridge *bridge)
{
    // raylib
    lib_raylib_main_register(bridge);
    lib_raylib_music_register(bridge);
    lib_raylib_sound_register(bridge);
    lib_raylib_painter_register(bridge);
    lib_raylib_control_register(bridge);
    //
    lib_test_register(bridge);
}

#endif
