// header

#ifndef H_LIB_INNERNAL_HEADER
#define H_LIB_INNERNAL_HEADER

#include "global.c"
#include "number.c"
#include "string.c"
#include "time.c"
#include "system.c"
#include "file.c"
#include "box.c"

void register_internal_libraries(Bridge *bridge)
{

    lib_global_register(bridge);
    lib_number_register(bridge);
    lib_string_register(bridge);
    lib_time_register(bridge);
    lib_system_register(bridge);
    lib_file_register(bridge);
    lib_box_register(bridge);

}

#endif
