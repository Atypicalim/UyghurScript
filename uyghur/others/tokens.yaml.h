
#ifndef YAML_TOKENS_FLAG
#define YAML_TOKENS_FLAG
//
typedef struct {
    char* key;
    char* val;
} PAIR_TOKENS;
//
// [M[ VARIABLES ]M]
//
// [M[ BODIES ]M]
//
int tokens_get_size(char *tp) {
    if (!tp) tp = "";
    // [M[ FILTER_SIZE ]M]
    return 0;
}
const PAIR_TOKENS* tokens_get_conf(char *tp) {
    if (!tp) tp = "";
    // [M[ FILTER_CONF ]M]
    return 0;
}
//
#endif // YAML_TOKENS_FLAG
