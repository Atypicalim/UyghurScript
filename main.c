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
    // char *s = "abcdeabcdeabcdeabcdeabcdeabcdeabcde";
    // char *r = str_replace(s, "bc", "12", 1, -1);
    // printf("%s\n%s\n", s, r);
    // printf("%d\n", str_count(s));
    // printf("%s\n", str_link(s, "--122"));
    // printf("%s\n%s\n", s, str_cut(s, 1, -2));
    // printf("-------------------------------%s %d ", s, str_find(s, "bcd", 1, 35, -100000));
    //
    return 0;
}
