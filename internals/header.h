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

bool check_more_than5(int a, int b)
{
    return (a + b) > 5;
}

void register_internal_libraries(Bridge *bridge)
{

    lib_global_register(bridge);
    lib_number_register(bridge);
    lib_string_register(bridge);
    lib_time_register(bridge);
    lib_system_register(bridge);
    lib_file_register(bridge);
    lib_box_register(bridge);

    // TODO: fix void* to float bug
    Bridge_startBox(bridge, NULL);
    Bridge_bind(bridge, "getMor", check_more_than5, UG_BOL);
    Bridge_bind(bridge, "getZon", time_get_zone, UG_NUM);
    Bridge_bind(bridge, "getSec", time_get_seconds, UG_NUM);
    Bridge_bind(bridge, "forSec", time_format_seconds, UG_STR);
    Bridge_register(bridge);
}

#endif
