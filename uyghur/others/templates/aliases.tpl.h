
#ifndef YAML_ALIASES_FLAG
#define YAML_ALIASES_FLAG
//
typedef struct {
    char* key;
    char* val;
} PAIR_ALIASES;

////////////////////////////////////////////////////////////////

// [M[ VARIABLES ]M]

////////////////////////////////////////////////////////////////

// [M[ BODIES ]M]

////////////////////////////////////////////////////////////////

// [M[ FOOTERS ]M]

////////////////////////////////////////////////////////////////

int aliases_get_size_by_lang(char *tp) {
    if (!tp) tp = "";
    // [M[ FILTER_SIZE_BY_LANG ]M]
    return 0;
}
const PAIR_ALIASES* aliases_get_conf_by_lang(char *tp) {
    if (!tp) tp = "";
    // [M[ FILTER_CONF_BY_LANG ]M]
    return 0;
}

int aliases_get_size_by_name(char *tp) {
    if (!tp) tp = "";
    // [M[ FILTER_SIZE_BY_NAME ]M]
    return 0;
}
const PAIR_ALIASES* aliases_get_conf_by_name(char *tp) {
    if (!tp) tp = "";
    // [M[ FILTER_CONF_BY_NAME ]M]
    return 0;
}
//
#endif // YAML_ALIASES_FLAG
