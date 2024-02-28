// implementation

#include "build/script.h"
#include "uyghur/uyghur.c"
#include "internals/header.h"
// #include "externals/header.h"

// unsigned char __build_script_ug[] = {};

int main(int argc, char const *argv[])
{
    //
    Uyghur *uyghur = Uyghur_new();
    register_internal_libraries(uyghur->bridge);
    // register_external_libraries(uyghur->bridge);
    if (argc >= 2) Uyghur_execute(uyghur, (char *)argv[1]);
    if (argc == 1) Uyghur_run(uyghur, "script.ug", __build_script_ug);
    Uyghur_free(uyghur);
    //
    return 0;
}
