// languages

#define LANG_ERR_SOME_PLACE "hojjet:[%s], qur:[%d], qatar:[%d]"
#define LANG_ERR_SIGN_PLACE LANG_ERR_SOME_PLACE ", herip:[%c]"
#define LANG_ERR_TOKEN_PLACE LANG_ERR_SOME_PLACE ", belge:[%s]"

void languages_set_type(char *tp) {
    for (int i = 0; i < YAML_SIZE_LANGUAGES_EN; i++) {
        YAML_PAIRS pair = YAML_MAP_LANGUAGES_EN[i];
        *pair.key = pair.val;
    }
}
