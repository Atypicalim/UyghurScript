
#ifndef YAML_CONFIGS_FLAG
#define YAML_CONFIGS_FLAG

#define UG_PROJECT_NAME "{}" // [M[ PROJECT_NAME ]M]
#define UG_VERSION_CODE {} // [M[ VERSION_CODE ]M]
#define UG_VERSION_NAME "{}" // [M[ VERSION_NAME ]M]

#define UG_SCRIPT_PATH "{}" // [M[ SCRIPT_PATH ]M]
#define UG_SCRIPT_NAME "{}" // [M[ SCRIPT_NAME ]M]

#define UG_LANGUAGE_COUNT {0} // [M[ LANGUAGE_COUNT ]M]
char *UG_LANGUAGE_ARRAY[] = {{"{0}"}}; // [M[ LANGUAGE_ARRAY ]M]

//
typedef struct {
    char* key;
    char* val;
} PAIR_CONFIGS;

////////////////////////////////////////////////////////////////

// [M[ VARIABLES ]M]

////////////////////////////////////////////////////////////////

// [M[ BODIES ]M]

////////////////////////////////////////////////////////////////

// [M[ FOOTERS ]M]

////////////////////////////////////////////////////////////////

int configs_get_size(char *tp) {
    if (!tp) tp = "";
    // [M[ FILTER_SIZE_BY_LANG ]M]
    return 0;
}
const PAIR_CONFIGS* configs_get_conf(char *tp) {
    if (!tp) tp = "";
    // [M[ FILTER_CONF_BY_LANG ]M]
    return 0;
}
int configs_get_size_by_name(char *tp) {
    if (!tp) tp = "";
    // [M[ FILTER_SIZE_BY_NAME ]M]
    return 0;
}
const PAIR_CONFIGS* configs_get_conf_by_name(char *tp) {
    if (!tp) tp = "";
    // [M[ FILTER_CONF_BY_NAME ]M]
    return 0;
}
//
#endif // YAML_CONFIGS_FLAG

