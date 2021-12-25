// implementation

#include "uyghur/uyghur.c"
#include "libraries/header.h"

int main(int argc, char const *argv[])
{
    //
    tools_assert(argc > 1, LANG_ERR_NO_INPUT_NAME);
    Uyghur *uyghur = Uyghur_new();
    register_external_libraries(uyghur->bridge);
    Uyghur_run(uyghur, argv[1]);
    Uyghur_free(uyghur);
    //
    return 0;
}
