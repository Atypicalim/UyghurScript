// header

#ifndef H_LIB_EXTERNAL_HEADER
#define H_LIB_EXTERNAL_HEADER

#include "board.c"
#include "test.c"

void register_external_libraries(Bridge *bridge)
{
    lib_raylib_register(bridge);
    lib_test_register(bridge);
}

#endif
