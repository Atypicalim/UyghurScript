// implementation

#include "uyghur.c"

int main(int argc, char const *argv[])
{
    //
    printf("hello uyghur script!\n");
    //
    if (argc < 2)
    {
        tools_error(LANG_ERR_NO_INPUT_NAME);
        return 1;
    }
    //
    Uyghur *uyghur = Uyghur_new();
    Uyghur_execute(uyghur, argv[1]);
    Uyghur_free(uyghur);
    //
    Token *token1 = Token_new("test.txt", 1, 1, 1, "111");
    Token_print(token1);
    Token *token2 = Token_new("test.txt", 1, 1, 1, "222");
    Token_print(token2);
    Token *token3 = Token_new("test.txt", 1, 1, 1, "333");
    Token_print(token3);
    Token_append(token2, token3);
    Token_prepend(token2, token1);
    Token_print(Token_last(Token_last(token3)));
    Token_print(token3->last->last);
    
    return 0;
}
