// implementation

#include "incbin.h"
#include "uyghur/uyghur.c"
#include "uyghur/internals/header.h"
#include "uyghur/externals/header.h"

#include <locale.h>
#include <wchar.h>

int main(int argc, char const *argv[])
{
    INCBIN(Script, UG_SCRIPT_PATH);
    //
    log_set_file("./test.log");
    log_set_level(1);
    log_set_color(true);
    log_set_quiet(false);
    // 
    popen("chcp 65001", "r");
    setlocale(LC_ALL, "en_US.utf8");
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
