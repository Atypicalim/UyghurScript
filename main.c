// implementation

#include "incbin.h"
#include "uyghur/uyghur.c"
#include "internals/header.h"
#include "externals/header.h"

int main(int argc, char const *argv[])
{
    INCBIN(Script, "./build/uyghur.ug");
    //
    Uyghur *uyghur = Uyghur_new();
    register_internal_libraries(uyghur->bridge);
    register_external_libraries(uyghur->bridge);
    if (argc == 1) Uyghur_runCode(uyghur, gScriptData, NULL);
    if (argc >= 2) Uyghur_runArgs(uyghur, argc, argv);
    Uyghur_free(uyghur);
    //
    return 0;
}
