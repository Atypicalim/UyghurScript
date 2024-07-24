
#ifndef YAML_LANGUAGES_FLAG
#define YAML_LANGUAGES_FLAG
//
typedef struct {
    char** key;
    char* val;
} PAIR_LANGUAGES;

////////////////////////////////////////////////////////////////

// [M[ VARIABLES ]M]

////////////////////////////////////////////////////////////////

// [M[ BODIES ]M]

////////////////////////////////////////////////////////////////

// [M[ FOOTERS ]M]

////////////////////////////////////////////////////////////////

int languages_get_size(char *tp) {
    if (!tp) tp = "";
    // [M[ FILTER_SIZE_BY_LANG ]M]
    return 0;
}
const PAIR_LANGUAGES* languages_get_conf(char *tp) {
    // printf("-->get_conf:%p\n", conf);
    if (!tp) tp = "";
    // [M[ FILTER_CONF_BY_LANG ]M]
    return 0;
}
int languages_get_size_by_name(char *tp) {
    if (!tp) tp = "";
    // [M[ FILTER_SIZE_BY_NAME ]M]
    return 0;
}
const PAIR_LANGUAGES* languages_get_conf_by_name(char *tp) {
    if (!tp) tp = "";
    // [M[ FILTER_CONF_BY_NAME ]M]
    return 0;
}
//
#endif // YAML_LANGUAGES_FLAG
