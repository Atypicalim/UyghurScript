// constant

#define UG_VERSION_NAME "0.1" 
#define UG_VERSION_CODE 0.1 

// common type
#define UG_TYPE_NON ' '
#define UG_TYPE_NIL '~'
#define UG_TYPE_BOL 'b'
#define UG_TYPE_NUM 'n'
#define UG_TYPE_STR 's'
#define UG_TYPE_OBJ 'o'
#define UG_TYPE_FUN 'f'
#define UG_TYPE_NTV 't'
#define UG_TYPE_CNT '@'

// token type
#define UG_TTYPE_NAM "NAM"
#define UG_TTYPE_KEY "KEY"
#define UG_TTYPE_WRD "WRD"
#define UG_TTYPE_STR "STR"
#define UG_TTYPE_NUM "NUM"
#define UG_TTYPE_BOL "BOL"
#define UG_TTYPE_EMP "EMP"
#define UG_TTYPE_CLC "CLC"


// ast type
#define UG_ATYPE_PRG 'P'
#define UG_ATYPE_END 'E'
#define UG_ATYPE_VAR '$'
#define UG_ATYPE_RSLT 'R'
#define UG_ATYPE_FUN 'F'
#define UG_ATYPE_CODE '~'
#define UG_ATYPE_CALL '&'
#define UG_ATYPE_IF 'I'
#define UG_ATYPE_IF_F '?'
#define UG_ATYPE_IF_M ':'
#define UG_ATYPE_IF_L ';'
#define UG_ATYPE_WHL 'W'
#define UG_ATYPE_EXC 'X'
#define UG_ATYPE_CVT 'C'
#define UG_ATYPE_OPRT '%'
#define UG_ATYPE_CLC '='

// container type
#define UG_CTYPE_BOX 'x'
#define UG_CTYPE_SCP 's'
#define UG_CTYPE_MDL 'm'

// object type
#define PCT_OBJ_VALUE '$'
#define PCT_OBJ_CNTNR '@'
#define PCT_OBJ_TOKEN 't'
#define PCT_OBJ_LEAF 'l'

// token value of keywords
//
#define TVALUE_EMPTY "quruq"
#define TVALUE_LOGIC "logika"
#define TVALUE_NUM "san"
#define TVALUE_STR "xet"
#define TVALUE_BOX "sanduq"
//
#define TVALUE_WHILE "nawada"
#define TVALUE_IF "eger"
#define TVALUE_IF_ELSE "egerde"
#define TVALUE_IF_OK "bolsa"
#define TVALUE_IF_NO "bolmisa"
#define TVALUE_CODE_END "tamamlansun"
// 
#define TVALUE_OUTPUT "yezilsun"
#define TVALUE_INPUT "oqulsun"
// 
#define TVALUE_DOT "."
#define TVALUE_AT "@"
//
#define TVALUE_TRUE "rast"
#define TVALUE_FALSE "yalghan"
#define TVALUE_NOT "ekische"

// calculate
#define TVALUE_CALCULATOR "="
// number
#define TVALUE_SIGN_ADD "+"
#define TVALUE_SIGN_SUB "-"
#define TVALUE_SIGN_MUL "*"
#define TVALUE_SIGN_DIV "/"
#define TVALUE_SIGN_POW "^"
#define TVALUE_SIGN_PER "%"
// 
#define TVALUE_SIGN_LESS "<"
#define TVALUE_SIGN_MORE ">"
#define TVALUE_SIGN_EQUAL "?"
// bool
#define TVALUE_SIGN_NOT "~" // duplicate
#define TVALUE_SIGN_AND "&"
#define TVALUE_SIGN_ORR "|"
// string
#define TVALUE_SIGN_LNK "+"
#define TVALUE_SIGN_RPT "*"
// border
#define TVALUE_OPEN "("
#define TVALUE_CLOSE ")"

//
#define TVALUE_TARGET_FROM "ikrandin" // TODO implement with standart library -> box+func
#define TVALUE_TARGET_TO "ikrangha" // TODO implement with standart library -> box+func (screen, file, etc)
//
#define TVALUE_VARIABLE "miqdar"
#define TVALUE_INITIAL "qimmiti" // beshida
#define TVALUE_CONVERT "qimmiti" // tipi
#define TVALUE_MADE "bolsun"
// 
#define TVALUE_EXCEPTION "xataliq"
// 
#define TVALUE_FUNCTION "fonkisiye"
#define TVALUE_CONTENT "mezmuni"
#define TVALUE_WITH "bilen"
#define TVALUE_CALL "ishlitilsun"
#define TVALUE_RETURN "qayturulsun"
#define TVALUE_FURTHER "we"
#define TVALUE_RESULT "netije"

const UG_PAIRS UG_ALIAS_MAP[] = {
    // 
    {"qushulghan", TVALUE_SIGN_ADD},
    {"elinghan", TVALUE_SIGN_SUB},
    {"kupeytilgen", TVALUE_SIGN_MUL},
    {"bulungen", TVALUE_SIGN_DIV},
    // 
    {"kichik", TVALUE_SIGN_LESS},
    {"chong", TVALUE_SIGN_MORE},
    {"teng", TVALUE_SIGN_EQUAL},
    // 
    {"ekische", TVALUE_SIGN_NOT},
    {"hemde", TVALUE_SIGN_AND},
    {"yaki", TVALUE_SIGN_ORR},
};

const UG_PAIRS UG_WORDS_MAP[] = {
    {TVALUE_VARIABLE, 0},
    {TVALUE_INITIAL, 0},
    {TVALUE_MADE, 0},
    {TVALUE_CODE_END, 0},
    {TVALUE_IF_OK, 0},
    {TVALUE_IF_NO, 0},
    // 
    {TVALUE_OUTPUT, 0},
    {TVALUE_INPUT, 0},
    {TVALUE_TARGET_FROM, 0},
    {TVALUE_TARGET_TO, 0},
    // 
    {TVALUE_WHILE, 0},
    {TVALUE_IF, 0},
    {TVALUE_IF_ELSE, 0},
    //
    {TVALUE_EXCEPTION, 0},
    // 
    {TVALUE_FUNCTION, 0},
    {TVALUE_CONTENT, 0},
    {TVALUE_WITH, 0},
    {TVALUE_CALL, 0},
    {TVALUE_RETURN, 0},
    {TVALUE_FURTHER, 0},
    {TVALUE_RESULT, 0},
    // 
    {TVALUE_NUM, 0},
    {TVALUE_STR, 0},
    {TVALUE_BOX, 0},
    {TVALUE_LOGIC, 0},
    {TVALUE_NOT, 0},
    // 
    {TVALUE_TRUE, UG_TTYPE_BOL},
    {TVALUE_FALSE, UG_TTYPE_BOL},
    {TVALUE_EMPTY, UG_TTYPE_EMP},
    // 
    {TVALUE_CALCULATOR, 0},
    {TVALUE_SIGN_ADD, UG_TTYPE_CLC},
    {TVALUE_SIGN_SUB, UG_TTYPE_CLC},
    {TVALUE_SIGN_MUL, UG_TTYPE_CLC},
    {TVALUE_SIGN_DIV, UG_TTYPE_CLC},
    {TVALUE_SIGN_POW, UG_TTYPE_CLC},
    {TVALUE_SIGN_PER, UG_TTYPE_CLC},
    {TVALUE_SIGN_EQUAL, UG_TTYPE_CLC},
    {TVALUE_SIGN_MORE, UG_TTYPE_CLC},
    {TVALUE_SIGN_LESS, UG_TTYPE_CLC},
    {TVALUE_SIGN_NOT, UG_TTYPE_CLC},
    {TVALUE_SIGN_AND, UG_TTYPE_CLC},
    {TVALUE_SIGN_ORR, UG_TTYPE_CLC},
    {TVALUE_SIGN_LNK, UG_TTYPE_CLC},
    //
    {TVALUE_OPEN, 0},
    {TVALUE_CLOSE, 0},
};

#define TTYPES_GROUP_DEFINE 7, UG_TTYPE_NAM, UG_TTYPE_KEY, UG_TTYPE_STR, UG_TTYPE_NUM, UG_TTYPE_BOL, UG_TTYPE_EMP, UG_TTYPE_WRD
#define TTYPES_GROUP_VALUES 6, UG_TTYPE_NAM, UG_TTYPE_KEY, UG_TTYPE_STR, UG_TTYPE_NUM, UG_TTYPE_BOL, UG_TTYPE_EMP
#define TTYPES_GROUP_STRING 3, UG_TTYPE_NAM, UG_TTYPE_KEY, UG_TTYPE_STR
#define TTYPES_GROUP_NUMBER 3, UG_TTYPE_NAM, UG_TTYPE_KEY, UG_TTYPE_NUM
#define TTYPES_GROUP_LOGICS 4, UG_TTYPE_NAM, UG_TTYPE_KEY, UG_TTYPE_BOL, UG_TTYPE_EMP
#define TTYPES_GROUP_KEYS 3, UG_TTYPE_NAM, UG_TTYPE_STR, UG_TTYPE_NUM

#define TTYPES_GROUP_TARGETS 2, TVALUE_TARGET_FROM, TVALUE_TARGET_TO
#define TVAUES_GROUP_CONVERT 6, TVALUE_NUM, TVALUE_STR, TVALUE_EMPTY, TVALUE_LOGIC, TVALUE_FUNCTION, TVALUE_NOT

#define TVAUE_GROUP_CALCULATION_ALL 3, TVALUE_SIGN_EQUAL, TVALUE_SIGN_LESS, TVALUE_SIGN_MORE
#define TVAUE_GROUP_CALCULATION_NUM 6, TVALUE_SIGN_ADD, TVALUE_SIGN_SUB, TVALUE_SIGN_MUL, TVALUE_SIGN_DIV, TVALUE_SIGN_POW, TVALUE_SIGN_PER
#define TVAUE_GROUP_CALCULATION_BOL 3, TVALUE_SIGN_NOT, TVALUE_SIGN_AND, TVALUE_SIGN_ORR
#define TVAUE_GROUP_CALCULATION_STR 2, TVALUE_SIGN_LNK, TVALUE_SIGN_RPT

#define TVAUE_GROUP_CALCULATION_1 2, TVALUE_SIGN_ADD, TVALUE_SIGN_SUB
#define TVAUE_GROUP_CALCULATION_2 2, TVALUE_SIGN_MUL, TVALUE_SIGN_DIV
#define TVAUE_GROUP_CALCULATION_3 11, TVALUE_SIGN_EQUAL, TVALUE_SIGN_POW, TVALUE_SIGN_PER, TVALUE_SIGN_LESS, TVALUE_SIGN_MORE, TVALUE_SIGN_EQUAL, TVALUE_SIGN_NOT, TVALUE_SIGN_AND, TVALUE_SIGN_ORR, TVALUE_SIGN_LNK, TVALUE_SIGN_RPT

#define CACHE_STRING_MAX_LENGTH 128

#define SCOPE_ALIAS_PRG "*"
#define SCOPE_ALIAS_MDL "+"
#define SCOPE_ALIAS_BOX "-"

#define CODE_FALSE -1
#define CODE_TRUE 1
#define CODE_NONE 0
#define CODE_FAIL -2
