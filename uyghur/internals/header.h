// header

#ifndef H_LIB_INNERNAL_HEADER
#define H_LIB_INNERNAL_HEADER

#include "global.c"
#include "number.c"
#include "string.c"
#include "list.c"
#include "dict.c"
#include "time.c"
#include "system.c"
#include "file.c"

void register_internal_libraries(Bridge *bridge)
{

    lib_global_register(bridge);
    // 
    // TODO:set proxies as fixed features
    Bridge_startBox(bridge);
    Bridge_bindString(bridge, "NESHIR_ISMI", UG_VERSION_NAME);
    Bridge_register(bridge, TVALUE_NIL);
    Bridge_startBox(bridge);
    Bridge_bindString(bridge, "NESHIR_ISMI", UG_VERSION_NAME);
    Bridge_register(bridge, TVALUE_BOL);
    // 
    lib_number_register(bridge);
    lib_string_register(bridge);
    lib_list_register(bridge);
    lib_dict_register(bridge);
    lib_time_register(bridge);
    lib_system_register(bridge);
    lib_file_register(bridge);

}

#endif
