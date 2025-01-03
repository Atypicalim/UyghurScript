// constant

// @
// common type
#define UG_TYPE_NON '?'
#define UG_TYPE_NIL '~'
#define UG_TYPE_BOL '!'
#define UG_TYPE_NUM '0'
#define UG_TYPE_STR '"'
#define UG_TYPE_WKR '&'
#define UG_TYPE_NTV '#'
#define UG_TYPE_CTR '$'
#define UG_TYPE_ATR '|'
#define UG_TYPE_OBJ '*'
#define UG_TYPE_LST 'l'
#define UG_TYPE_DCT 'd'
#define UG_TYPE_KND 'k'
#define UG_TYPE_PXY 'p'
#define UG_TYPE_SCP 's'
#define UG_TYPE_MDL 'm'
#define UG_TYPE_STF 'f'
#define UG_TYPE_TSK 't'

// char values
#define CHAR_EMPTY ' '
#define CHAR_QUESSION '?'

// sign values
#define SIGN_LINE "\n"
#define SIGN_CARRIAGE "\r"
#define SIGN_TERMINATE "\0"
#define SIGN_ESCAPE "\\"
#define SIGN_QUOTE "\""
#define SIGN_SLASH "/"
#define SIGN_LOCK "#"
#define SIGN_DOT "."
#define SIGN_AT "@"
#define SIGN_EQ "="
// 
#define SIGN_ADD "+"
#define SIGN_SUB "-"
#define SIGN_MUL "*"
#define SIGN_DIV "/"
#define SIGN_POW "^"
// 
#define SIGN_OPEN_SMALL "("
#define SIGN_CLOSE_SMALL ")"
#define SIGN_OPEN_MIDDLE "{"
#define SIGN_CLOSE_MIDDLE "}"
#define SIGN_OPEN_BIG "["
#define SIGN_CLOSE_BIG "]"

// token type
#define UG_TTYPE_NIL "NIL"
#define UG_TTYPE_BOL "BOL"
#define UG_TTYPE_NUM "NUM"
#define UG_TTYPE_STR "STR"
#define UG_TTYPE_NAM "NAM"
#define UG_TTYPE_KEY "KEY"
#define UG_TTYPE_WRD "WRD"
#define UG_TTYPE_CLC "CLC"


// ast type
#define UG_ATYPE_PRG 'P'
#define UG_ATYPE_END 'E'
#define UG_ATYPE_VAR '$'
#define UG_ATYPE_RSLT 'R'
#define UG_ATYPE_WRKR 'W'
#define UG_ATYPE_CRTR 'C'
#define UG_ATYPE_ASTR 'A'
#define UG_ATYPE_CODE '~'
#define UG_ATYPE_APPLY '&'
#define UG_ATYPE_IF 'i'
#define UG_ATYPE_IF_F '?'
#define UG_ATYPE_IF_M ':'
#define UG_ATYPE_IF_L ';'
#define UG_ATYPE_WHL 'w'
#define UG_ATYPE_SPR 's'
#define UG_ATYPE_EXC 'X'
#define UG_ATYPE_CVT 'c'
#define UG_ATYPE_CMD '%'
#define UG_ATYPE_CLC '='
#define UG_ATYPE_GNR '@'

// object type
#define PCT_OBJ_TOKEN 't'
#define PCT_OBJ_LEAF 'l'
#define PCT_OBJ_VALUE '$'

// token value of keywords
#define TVALUE_COLON ":"
#define TVALUE_COMMA ","
#define TVALUE_DOT "."
#define TVALUE_AT "@"

// calculate
#define TVALUE_EQUALING "="
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
// check
#define TVALUE_SIGN_CHK "!"
// border
#define TVALUE_OPEN "("
#define TVALUE_CLOSE ")"

# define TVALUE_COMMANDS 2, TVALUE_CMD_OUTPUT, TVALUE_CMD_INPUT

const UG_PAIRS UG_LETTERS_MAP[] = {
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
    // 
    {"ikrangha", TVALUE_COMMAND},
    {"ikrandin", TVALUE_COMMAND},
};

const UG_PAIRS UG_WORDS_MAP[] = {
    {TVALUE_VARIABLE, 0},
    {TVALUE_VALUE, 0},
    {TVALUE_MADE, 0},
    {TVALUE_CODE_END, 0},
    // 
    {TVALUE_COMMAND, 0},
    {TVALUE_CMD_OUTPUT, 0},
    {TVALUE_CMD_INPUT, 0},
    // 
    {TVALUE_WHILE, 0},
    //
    {TVALUE_SPREAD, 0},
    {TVALUE_BECOME, 0},
    //
    {TVALUE_IF, 0},
    {TVALUE_IF_ELSE, 0},
    {TVALUE_THEN, 0},
    {TVALUE_ELSE, 0},
    //
    {TVALUE_EXCEPTION, 0},
    // 
    {TVALUE_WORKER, 0},
    {TVALUE_CREATOR, 0},
    {TVALUE_ASSISTER, 0},
    {TVALUE_CONTENT, 0},
    {TVALUE_WITH, 0},
    {TVALUE_APPLY, 0},
    {TVALUE_RESULT, 0},
    {TVALUE_RETURNED, 0},
    {TVALUE_FURTHER, 0},
    {TVALUE_RECIEVED, 0},
    // 
    {TVALUE_BOL, 0},
    {TVALUE_NUM, 0},
    {TVALUE_STR, 0},
    {TVALUE_LST, 0},
    {TVALUE_DCT, 0},
    {TVALUE_OPPOSITE, 0},
    //
    {TVALUE_THIS, 0},
    {TVALUE_MODULE, 0},
    {TVALUE_GLOBAL, 0},
    // 
    {TVALUE_NIL, UG_TTYPE_NIL},
    {TVALUE_TRUE, UG_TTYPE_BOL},
    {TVALUE_FALSE, UG_TTYPE_BOL},
    // 
    {TVALUE_EQUALING, 0},
    //
    {TVALUE_SIGN_ADD, UG_TTYPE_CLC},
    {TVALUE_SIGN_SUB, UG_TTYPE_CLC},
    {TVALUE_SIGN_MUL, UG_TTYPE_CLC},
    {TVALUE_SIGN_DIV, UG_TTYPE_CLC},
    {TVALUE_SIGN_POW, UG_TTYPE_CLC},
    {TVALUE_SIGN_PER, UG_TTYPE_CLC},
    {TVALUE_SIGN_CHK, UG_TTYPE_CLC},
    {TVALUE_SIGN_EQUAL, UG_TTYPE_CLC},
    {TVALUE_SIGN_MORE, UG_TTYPE_CLC},
    {TVALUE_SIGN_LESS, UG_TTYPE_CLC},
    {TVALUE_SIGN_NOT, UG_TTYPE_CLC},
    {TVALUE_SIGN_AND, UG_TTYPE_CLC},
    {TVALUE_SIGN_ORR, UG_TTYPE_CLC},
    {TVALUE_SIGN_LNK, UG_TTYPE_CLC},
    //
    {TVALUE_COLON, 0},
    {TVALUE_COMMA, 0},
    {TVALUE_DOT, 0},
    {TVALUE_AT, 0},
    //
    {SIGN_OPEN_BIG, 0},
    {SIGN_CLOSE_BIG, 0},
    {SIGN_OPEN_MIDDLE, 0},
    {SIGN_CLOSE_MIDDLE, 0},
    {SIGN_OPEN_SMALL, 0},
    {SIGN_CLOSE_SMALL, 0},
};

#define TVAUES_GROUP_UTYPES 6, TVALUE_NIL, TVALUE_BOL, TVALUE_NUM, TVALUE_STR, TVALUE_LST, TVALUE_DCT
#define TTYPES_GROUP_VALUES 6, UG_TTYPE_NAM, UG_TTYPE_KEY, UG_TTYPE_STR, UG_TTYPE_NUM, UG_TTYPE_BOL, UG_TTYPE_NIL
#define TTYPES_GROUP_STRING 3, UG_TTYPE_NAM, UG_TTYPE_KEY, UG_TTYPE_STR
#define TTYPES_GROUP_NUMBER 3, UG_TTYPE_NAM, UG_TTYPE_KEY, UG_TTYPE_NUM
#define TTYPES_GROUP_LOGICS 4, UG_TTYPE_NAM, UG_TTYPE_KEY, UG_TTYPE_BOL, UG_TTYPE_NIL
#define TTYPES_GROUP_KEYS 3, UG_TTYPE_NAM, UG_TTYPE_STR, UG_TTYPE_NUM

#define TVAUES_GROUP_CHANGEABLE 2, UG_TTYPE_NAM, UG_TTYPE_KEY
#define TVAUES_GROUP_ITERABLE 5, UG_TTYPE_NAM, UG_TTYPE_KEY, UG_TTYPE_NUM, UG_TTYPE_STR, UG_TTYPE_WRD
#define TVAUES_GROUP_CONVERT 5, TVALUE_NIL, TVALUE_BOL, TVALUE_NUM, TVALUE_STR, TVALUE_OPPOSITE
#define TVAUES_GROUP_NICKNAME 3, TVALUE_THIS, TVALUE_MODULE, TVALUE_GLOBAL

#define TVAUE_GROUP_CALCULATION_ALL 4, TVALUE_SIGN_EQUAL, TVALUE_SIGN_LESS, TVALUE_SIGN_MORE, TVALUE_SIGN_CHK
#define TVAUE_GROUP_CALCULATION_NUM 6, TVALUE_SIGN_ADD, TVALUE_SIGN_SUB, TVALUE_SIGN_MUL, TVALUE_SIGN_DIV, TVALUE_SIGN_POW, TVALUE_SIGN_PER
#define TVAUE_GROUP_CALCULATION_BOL 3, TVALUE_SIGN_NOT, TVALUE_SIGN_AND, TVALUE_SIGN_ORR
#define TVAUE_GROUP_CALCULATION_STR 2, TVALUE_SIGN_LNK, TVALUE_SIGN_RPT

#define TVAUE_GROUP_CALCULATION_1 2, TVALUE_SIGN_ADD, TVALUE_SIGN_SUB
#define TVAUE_GROUP_CALCULATION_2 2, TVALUE_SIGN_MUL, TVALUE_SIGN_DIV
#define TVAUE_GROUP_CALCULATION_3 11, TVALUE_SIGN_CHK, TVALUE_SIGN_POW, TVALUE_SIGN_PER, TVALUE_SIGN_LESS, TVALUE_SIGN_MORE, TVALUE_SIGN_EQUAL, TVALUE_SIGN_NOT, TVALUE_SIGN_AND, TVALUE_SIGN_ORR, TVALUE_SIGN_LNK, TVALUE_SIGN_RPT

#define CACHE_STRING_MAX_LENGTH 128

#define SCOPE_ALIAS_GLB "*"
#define SCOPE_ALIAS_MDL "+"
#define SCOPE_ALIAS_SLF "-"

#define TEXT_VARIABLE "__var"
#define TEXT_FUNCTION "__fun"

#define CODE_FALSE -1
#define CODE_TRUE 1
#define CODE_NONE 0
#define CODE_FAIL -2

// languages

#define UG_TAG_LOG "LOG"
#define UG_TAG_TRACE "TRACE"
#define UG_TAG_UNKNOWN "UNKOWN"
// 
#define LANG_ERR_UYGHUR_EXCEPTION "programma xataliqi kuruldi"
#define LANG_ERR_NO_INPUT_FILE "programma hojjiti tepilmidi"
//
#define LANG_ERR_SOME_PLACE "hojjet:[%s], qur:[%d], qatar:[%d]"
#define LANG_ERR_SIGN_PLACE LANG_ERR_SOME_PLACE ", belge:[%s]"
#define LANG_ERR_TOKEN_PLACE LANG_ERR_SOME_PLACE ", sozluk:[%s]"

// alias

#define ALIAS_boolean TVALUE_BOL
#define ALIAS_number TVALUE_NUM
#define ALIAS_string TVALUE_STR
#define ALIAS_list TVALUE_LST
#define ALIAS_dict TVALUE_DCT
#define ALIAS_stuf TVALUE_STUF
#define ALIAS_task TVALUE_TASK
