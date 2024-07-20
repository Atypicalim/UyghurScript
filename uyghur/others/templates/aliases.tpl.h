
#ifndef YAML_ALIASES_FLAG
#define YAML_ALIASES_FLAG
//
typedef struct {
    char* key;
    char* val;
} PAIR_ALIASES;
//
// [M[ VARIABLES ]M]
//
// [M[ BODIES ]M]
//
int aliases_get_size(char *tp) {
    if (!tp) tp = "";
    // [M[ FILTER_SIZE ]M]
    return 0;
}
const PAIR_ALIASES* aliases_get_conf(char *tp) {
    if (!tp) tp = "";
    // [M[ FILTER_CONF ]M]
    return 0;
}
//
#endif // YAML_ALIASES_FLAG
