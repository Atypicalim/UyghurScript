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


// char values
#define CHAR_EMPTY ' '
#define CHAR_QUESSION '?'

// sign symbols 
#define SIGN_EQUAL "="
#define SIGN_COLON ":"
#define SIGN_COMMA ","
#define SIGN_DOT "."
#define SIGN_AT "@"
#define SIGN_LOCK "#"
#define SIGN_QUEST "?"
#define SIGN_EXCLAM "!"

// sign special
#define SIGN_LINE "\n"
#define SIGN_CARRIAGE "\r"
#define SIGN_TERMINATE "\0"
#define SIGN_ESCAPE "\\"
#define SIGN_QUOTE "\""
#define SIGN_SLASH "/"

// sign calculations
#define SIGN_ADD "+"
#define SIGN_SUB "-"
#define SIGN_MUL "*"
#define SIGN_DIV "/"
#define SIGN_POW "^"
#define SIGN_PER "%"

// sign bracket
#define SIGN_OPEN_SMALL "("
#define SIGN_CLOSE_SMALL ")"
#define SIGN_OPEN_MIDDLE "{"
#define SIGN_CLOSE_MIDDLE "}"
#define SIGN_OPEN_BIG "["
#define SIGN_CLOSE_BIG "]"

// compares
#define SIGN_LESS "<"
#define SIGN_MORE ">"
// bool
#define SIGN_NOT "~" // duplicate
#define SIGN_AND "&"
#define SIGN_ORR "|"
// string
#define SIGN_LNK SIGN_ADD
#define SIGN_RPT SIGN_MUL
// check
#define SIGN_CHK SIGN_EXCLAM
// border
#define SIGN_OPEN "("
#define SIGN_CLOSE ")"

# define TVALUE_COMMANDS 2, TVALUE_CMD_OUTPUT, TVALUE_CMD_INPUT

const UG_PAIRS UG_WORDS_MAP[] = {
    {LETTER_VARIABLE, 0},
    {LETTER_VALUE, 0},
    {LETTER_MADE, 0},
    {LETTER_END, 0},
    // 
    {LETTER_COMMAND, 0},
    {LETTER_CMD_OUTPUT, 0},
    {LETTER_CMD_INPUT, 0},
    // 
    {LETTER_WHILE, 0},
    //
    {LETTER_SPREAD, 0},
    {LETTER_BECOME, 0},
    //
    {LETTER_IF, 0},
    {LETTER_ELIF, 0},
    {LETTER_THEN, 0},
    {LETTER_ELSE, 0},
    //
    {LETTER_EXCEPTION, 0},
    // 
    {LETTER_WORKER, 0},
    {LETTER_CREATOR, 0},
    {LETTER_ASSISTER, 0},
    {LETTER_CONTENT, 0},
    {LETTER_WITH, 0},
    {LETTER_APPLY, 0},
    {LETTER_RESULT, 0},
    {LETTER_RETURNED, 0},
    {LETTER_FURTHER, 0},
    {LETTER_RECIEVED, 0},
    // 
    {LETTER_BOL, 0},
    {LETTER_NUM, 0},
    {LETTER_STR, 0},
    {LETTER_LST, 0},
    {LETTER_DCT, 0},
    {LETTER_OPPOSITE, 0},
    //
    {LETTER_THIS, 0},
    {LETTER_MODULE, 0},
    {LETTER_GLOBAL, 0},
    // 
    {LETTER_NIL, UG_TTYPE_NIL},
    {LETTER_TRUE, UG_TTYPE_BOL},
    {LETTER_FALSE, UG_TTYPE_BOL},
    // 
    {SIGN_EQUAL, 0},
    //
    {SIGN_ADD, UG_TTYPE_CLC},
    {SIGN_SUB, UG_TTYPE_CLC},
    {SIGN_MUL, UG_TTYPE_CLC},
    {SIGN_DIV, UG_TTYPE_CLC},
    {SIGN_POW, UG_TTYPE_CLC},
    {SIGN_PER, UG_TTYPE_CLC},
    {SIGN_CHK, UG_TTYPE_CLC},
    {SIGN_QUEST, UG_TTYPE_CLC},
    {SIGN_MORE, UG_TTYPE_CLC},
    {SIGN_LESS, UG_TTYPE_CLC},
    {SIGN_NOT, UG_TTYPE_CLC},
    {SIGN_AND, UG_TTYPE_CLC},
    {SIGN_ORR, UG_TTYPE_CLC},
    {SIGN_LNK, UG_TTYPE_CLC},
    //
    {SIGN_COLON, 0},
    {SIGN_COMMA, 0},
    {SIGN_DOT, 0},
    {SIGN_AT, 0},
    //
    {SIGN_OPEN_BIG, 0},
    {SIGN_CLOSE_BIG, 0},
    {SIGN_OPEN_MIDDLE, 0},
    {SIGN_CLOSE_MIDDLE, 0},
    {SIGN_OPEN_SMALL, 0},
    {SIGN_CLOSE_SMALL, 0},
};

#define TVAUES_GROUP_UTYPES 6, LETTER_NIL, LETTER_BOL, LETTER_NUM, LETTER_STR, LETTER_LST, LETTER_DCT
#define TTYPES_GROUP_VALUES 6, UG_TTYPE_NAM, UG_TTYPE_KEY, UG_TTYPE_STR, UG_TTYPE_NUM, UG_TTYPE_BOL, UG_TTYPE_NIL
#define TTYPES_GROUP_STRING 3, UG_TTYPE_NAM, UG_TTYPE_KEY, UG_TTYPE_STR
#define TTYPES_GROUP_NUMBER 3, UG_TTYPE_NAM, UG_TTYPE_KEY, UG_TTYPE_NUM
#define TTYPES_GROUP_LOGICS 4, UG_TTYPE_NAM, UG_TTYPE_KEY, UG_TTYPE_BOL, UG_TTYPE_NIL
#define TTYPES_GROUP_KEYS 3, UG_TTYPE_NAM, UG_TTYPE_STR, UG_TTYPE_NUM

#define TVAUES_GROUP_CHANGEABLE 2, UG_TTYPE_NAM, UG_TTYPE_KEY
#define TVAUES_GROUP_ITERABLE 5, UG_TTYPE_NAM, UG_TTYPE_KEY, UG_TTYPE_NUM, UG_TTYPE_STR, UG_TTYPE_WRD
#define TVAUES_GROUP_CONVERT 5, LETTER_NIL, LETTER_BOL, LETTER_NUM, LETTER_STR, LETTER_OPPOSITE
#define TVAUES_GROUP_NICKNAME 3, LETTER_THIS, LETTER_MODULE, LETTER_GLOBAL

#define TVAUE_GROUP_CALCULATION_ALL 4, SIGN_QUEST, SIGN_LESS, SIGN_MORE, SIGN_CHK
#define TVAUE_GROUP_CALCULATION_NUM 6, SIGN_ADD, SIGN_SUB, SIGN_MUL, SIGN_DIV, SIGN_POW, SIGN_PER
#define TVAUE_GROUP_CALCULATION_BOL 3, SIGN_NOT, SIGN_AND, SIGN_ORR
#define TVAUE_GROUP_CALCULATION_STR 2, SIGN_LNK, SIGN_RPT

#define TVAUE_GROUP_CALCULATION_1 2, SIGN_ADD, SIGN_SUB
#define TVAUE_GROUP_CALCULATION_2 2, SIGN_MUL, SIGN_DIV
#define TVAUE_GROUP_CALCULATION_3 11, SIGN_CHK, SIGN_POW, SIGN_PER, SIGN_LESS, SIGN_MORE, SIGN_QUEST, SIGN_NOT, SIGN_AND, SIGN_ORR, SIGN_LNK, SIGN_RPT

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

#define ALIAS_boolean LETTER_BOL
#define ALIAS_number LETTER_NUM
#define ALIAS_string LETTER_STR
#define ALIAS_list LETTER_LST
#define ALIAS_dict LETTER_DCT
#define ALIAS_stuf LETTER_STUF
#define ALIAS_task LETTER_TASK
