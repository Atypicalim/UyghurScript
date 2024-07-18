// implementation

#include "incbin.h"
#include "uyghur/uyghur.c"
#include "internals/header.h"
#include "externals/header.h"

#include "utf8.h"
#include <locale.h>
#include <wchar.h>

int main(int argc, char const *argv[])
{
    INCBIN(Script, UG_SCRIPT_PATH);
    //
    log_set_file("./test.log");
    log_set_level(0);
    log_set_color(true);
    log_set_quiet(false);
    // 
    setlocale(LC_ALL, "en_US.utf8");
    printf("-----------------------------\n");
    char *txt = "boş";
    printf("txt: %s \n", txt);
    printf("sizeof: %i -> %i \n", sizeof(txt), utf8size(txt));
    printf("strlen: %i -> %i \n", strlen(txt), utf8len(txt));
    for (size_t i = 0; i < utf8len(txt); i++)
    {
        printf("-> %i \n", i);
    }
    printf("-----------------------------\n");
    //
    // Uyghur *uyghur = Uyghur_new();
    // register_internal_libraries(uyghur->bridge);
    // register_external_libraries(uyghur->bridge);
    // if (argc == 1) Uyghur_runCode(uyghur, gScriptData, NULL);
    // if (argc >= 2) Uyghur_runArgs(uyghur, argc, argv);
    // Uyghur_free(uyghur);
    //
    return 0;
}
