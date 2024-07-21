// header

#ifndef H_LIB_EXTERNAL_HEADER
#define H_LIB_EXTERNAL_HEADER

#include "stage.c"
#include "board.c"
#include "music.c"
#include "sound.c"
#include "control.c"
#include "test.c"

void register_external_libraries(Bridge *bridge)
{
    //
    lib_stage_register(bridge);
    lib_board_register(bridge);
    lib_music_register(bridge);
    lib_sound_register(bridge);
    lib_control_register(bridge);
    //
    lib_test_register(bridge);
}

#endif
