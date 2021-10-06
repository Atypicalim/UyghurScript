// implementation

#include "uyghur.c"

int main(int argc, char const *argv[])
{
    //
    printf("hello uyghur script!\n");
    //
    tools_check(argc > 1, LANG_ERR_NO_INPUT_NAME);
    Uyghur *uyghur = Uyghur_new();
    Uyghur_run(uyghur, argv[1]);
    Uyghur_free(uyghur);
    //
    // Token *token1 = Token_new("test.txt", 1, 1, "", "111");
    // Token_print(token1);
    // Token *token2 = Token_new("test.txt", 1, 1, "", "222");
    // Token_print(token2);
    // Token *token3 = Token_new("test.txt", 1, 1, "", "333");
    // Token_print(token3);
    // Token_append(token2, token3);
    // Token_prepend(token2, token1);
    // Token_print(Token_last(Token_last(token3)));
    // Token_print(token3->last->last);
    //
    // hash map test
    // Hashmap *map = Hashmap_new();
    // Hashmap_set(map, "aaa", "aaa-aaa");
    // Hashmap_set(map, "bbb", "bbb-bbb");
    // Hashmap_print(map);
    // Hashmap_free(map);
    //
    return 0;
}
