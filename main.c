// implementation

#include "uyghur/uyghur.c"
#include "libraries/test.c"

int main(int argc, char const *argv[])
{
    //
    tools_assert(argc > 1, LANG_ERR_NO_INPUT_NAME);
    Uyghur *uyghur = Uyghur_new();
    lib_test_register(uyghur->bridge);
    Uyghur_run(uyghur, argv[1]);
    lib_test_call(uyghur->bridge);
    Uyghur_free(uyghur);
    //
    return 0;
}
