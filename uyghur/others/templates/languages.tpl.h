
#ifndef YAML_LANGUAGES_FLAG
#define YAML_LANGUAGES_FLAG
//
typedef struct {
    char** key;
    char* val;
} PAIR_LANGUAGES;
//
// [M[ VARIABLES ]M]
//
// [M[ BODIES ]M]
//
int languages_get_size(char *tp) {
    if (!tp) tp = "";
    // [M[ FILTER_SIZE ]M]
    return 0;
}
const PAIR_LANGUAGES* languages_get_conf(char *tp) {
    // printf("-->get_conf:%p\n", conf);
    if (!tp) tp = "";
    // [M[ FILTER_CONF ]M]
    return 0;
}
//
#endif // YAML_LANGUAGES_FLAG
