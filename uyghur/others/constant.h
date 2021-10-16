// constant

// token types
#define TTYPE_NAME "TTYPE_NAME"
#define TTYPE_WORD "TTYPE_WORD"
#define TTYPE_STRING "TTYPE_STRING"
#define TTYPE_NUMBER "TTYPE_NUMBER"
#define TTYPE_BOOL "TTYPE_BOOL"
#define TTYPE_EMPTY "TTYPE_EMPTY"

// token value of keywords
//
#define TVALUE_STR "xet"
#define TVALUE_NUM "san"
#define TVALUE_LOGIKA "logika"
#define TVALUE_EMPTY "quruq"
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
// 
#define TVALUE_SOMEVALUE "qimmet"
#define TVALUE_SOMETYPE "tipi"
//
#define TVALUE_TRUE "rast"
#define TVALUE_FALSE "yalghan"
//
#define TVALUE_AND "xemde"
#define TVALUE_OR "yaki"
#define TVALUE_NOT "ekische"
//
#define TVALUE_ADD "qushulghan"
#define TVALUE_SUB "elinghan"
#define TVALUE_MUL "kupeytilgen"
#define TVALUE_DIV "bulungen"
#define TVALUE_LESS "kichik"
#define TVALUE_MORE "chong"
// 
#define TVALUE_CONCAT "ulanghan"
#define TVALUE_EQUAL "teng"
//
#define TVALUE_TARGET_FROM "ikrandin" // TODO target+action -> box+func
#define TVALUE_TARGET_TO "ikrangha" // TODO screen, file, other tream
//
#define TVALUE_VARIABLE "mixtar"
#define TVALUE_VALUE "qimmiti"
#define TVALUE_MADE "bolsun"

#define TVALUE_FUNC "fonkisiye"
#define TVALUE_CONTENT "mezmuni"
#define TVALUE_WITH "bilen"
#define TVALUE_CALL "ishlitilsun"
#define TVALUE_RETURN "qayturulsun"
#define TVALUE_FURTHER "we"
#define TVALUE_RESULT "netije"

// ast type
#define ASTTYPE_PROGRAM "ASTTYPE_PROGRAM"
#define ASTTYPE_END "ASTTYPE_END"
#define ASTTYPE_VARIABLE "ASTTYPE_VARIABLE"
#define ASTTYPE_ASSIGN "ASTTYPE_ASSIGN"
#define ASTTYPE_TRANSFORM "ASTTYPE_TRANSFORM"
#define ASTTYPE_RESULT "ASTTYPE_RESULT"
#define ASTTYPE_FUNC "ASTTYPE_FUNC"
#define ASTTYPE_CALL "ASTTYPE_CALL"
#define ASTTYPE_IF "ASTTYPE_IF"
#define ASTTYPE_IF_FIRST "ASTTYPE_IF_FIRST"
#define ASTTYPE_IF_MIDDLE "ASTTYPE_IF_MIDDLE"
#define ASTTYPE_IF_LAST "ASTTYPE_IF_LAST"
#define ASTTYPE_WHILE "ASTTYPE_WHILE"
#define ASTTYPE_EXPRESSION_SINGLE "ASTTYPE_EXPRESSION_SINGLE"
#define ASTTYPE_EXPRESSION_DOUBLE "ASTTYPE_EXPRESSION_DOUBLE" // TODO complex
#define ASTTYPE_OPERATE "ASTTYPE_OPERATE"

#define TTYPES_GROUP_DEFINE 5, TTYPE_NAME, TTYPE_STRING, TTYPE_NUMBER, TTYPE_BOOL, TTYPE_EMPTY
#define TTYPES_GROUP_STRING 2, TTYPE_NAME, TTYPE_STRING
#define TTYPES_GROUP_NUMBER 2, TTYPE_NAME, TTYPE_NUMBER
#define TTYPES_GROUP_LOGICS 3, TTYPE_NAME, TTYPE_BOOL, TTYPE_EMPTY

#define TTYPES_GROUP_TARGETS 2, TVALUE_TARGET_FROM, TVALUE_TARGET_TO

#define TVALUE_GROUP_EXP_STRING 2, TVALUE_EQUAL, TVALUE_CONCAT
#define TVALUE_GROUP_EXP_NUMBER 7, TVALUE_EQUAL, TVALUE_ADD, TVALUE_SUB, TVALUE_MUL, TVALUE_DIV, TVALUE_LESS, TVALUE_MORE
#define TVALUE_GROUP_EXP_BOOL 4, TVALUE_EQUAL, TVALUE_AND, TVALUE_OR, TVALUE_NOT
#define TVALUE_GROUP_EXP_ALL 11, TVALUE_EQUAL, TVALUE_CONCAT, TVALUE_ADD, TVALUE_SUB, TVALUE_MUL, TVALUE_DIV, TVALUE_LESS, TVALUE_MORE, TVALUE_AND, TVALUE_OR, TVALUE_NOT

#define TVAUE_GROUP_DO_TRANSFROM 4, TVALUE_STR, TVALUE_NUM, TVALUE_LOGIKA, TVALUE_FUNC

