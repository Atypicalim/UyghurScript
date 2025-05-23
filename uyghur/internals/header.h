// header

#ifndef H_LIB_INNERNAL_HEADER
#define H_LIB_INNERNAL_HEADER

#include "../../build/bind/internals/global.c"
#include "../../build/bind/internals/number.c"
#include "../../build/bind/internals/string.c"
#include "../../build/bind/internals/list.c"
#include "../../build/bind/internals/dict.c"
#include "../../build/bind/internals/time.c"
#include "../../build/bind/internals/system.c"
#include "../../build/bind/internals/file.c"
#include "../../build/bind/internals/cipher.c"
// #include "../../build/bind/internals/stuf.c"
// #include "../../build/bind/internals/task.c"

void register_internal_libraries(Bridge *bridge)
{

    //
    // Bridge_bindString(bridge, "NESHIR_ISMI", UG_VERSION_NAME);
    // Bridge_bindNumber(bridge, "NESHIR_NUMIRI", UG_VERSION_CODE);
    // 
    // Bridge_bindNumber(bridge, "pi", acos(-1));
    // Bridge_bindNumber(bridge, "e", exp(1));
    // 
    lib_global_register(bridge);
    // 
    // TODO:set proxies as fixed features
    Bridge_startBox(bridge);
    Bridge_bindString(bridge, "NESHIR_ISMI", UG_VERSION_NAME);
    Bridge_register(bridge, NULL);
    Bridge_startBox(bridge);
    Bridge_bindString(bridge, "NESHIR_ISMI", UG_VERSION_NAME);
    Bridge_register(bridge, NULL);
    // 
    lib_number_register(bridge);
    lib_string_register(bridge);
    lib_list_register(bridge);
    lib_dict_register(bridge);
    lib_time_register(bridge);
    lib_system_register(bridge);
    lib_file_register(bridge);
    lib_cipher_register(bridge);
    // lib_stuf_register(bridge);
    // lib_task_register(bridge);

}

#endif
