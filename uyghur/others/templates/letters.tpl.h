
#ifndef YAML_LETTERS_FLAG
#define YAML_LETTERS_FLAG
//
typedef struct {
    char* key;
    char* val;
} PAIR_LETTERS;

////////////////////////////////////////////////////////////////

// [M[ VARIABLES ]M]

////////////////////////////////////////////////////////////////

// [M[ BODIES ]M]

////////////////////////////////////////////////////////////////

// [M[ FOOTERS ]M]

////////////////////////////////////////////////////////////////

int letters_get_size(char *tp) {
    if (!tp) tp = "";
    // [M[ FILTER_SIZE_BY_LANG ]M]
    return 0;
}
const PAIR_LETTERS* letters_get_conf(char *tp) {
    if (!tp) tp = "";
    // [M[ FILTER_CONF_BY_LANG ]M]
    return 0;
}
int letters_get_size_by_name(char *tp) {
    if (!tp) tp = "";
    // [M[ FILTER_SIZE_BY_NAME ]M]
    return 0;
}
const PAIR_LETTERS* letters_get_conf_by_name(char *tp) {
    if (!tp) tp = "";
    // [M[ FILTER_CONF_BY_NAME ]M]
    return 0;
}
//
#endif // YAML_LETTERS_FLAG
