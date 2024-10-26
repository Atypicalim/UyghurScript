// header

#ifndef H_UG_HEADER
#define H_UG_HEADER

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <ctype.h>
#include <stdarg.h>
#include <time.h>
#include <unistd.h>
#include <sys/stat.h>
#include <limits.h>
#include <setjmp.h>

#include "utf8.h"
#include "libs/utf8iter/utf-8.h"
#include "libs/utf8iter/utf-8.c"

// #define STDSTRING_IMPLEMENTATION
// #include "stdstring.h"

// 

#define IS_DEVELOP false
bool isTest = false;
#define MAX_STACK_SIZE 1000
#define MAX_TRACE_SIZE 5

#define IS_GC_SWEEPING true
#define IS_GC_COUNTING !IS_GC_SWEEPING
#define IS_GC_LINK_LISTABLE_ARR true
#define IS_GC_LINK_DICTABLE_MAP true
#define IS_GC_CACHEABLE true

#define IS_RETAIN_VALUES !IS_GC_SWEEPING

#define MACHINE_CACHE_SIZE_BASE 1000
#define MACHINE_CACHE_SIZE_ARR MACHINE_CACHE_SIZE_BASE * 1
#define MACHINE_CACHE_SIZE_MAP MACHINE_CACHE_SIZE_BASE * 5
#define MACHINE_CACHE_SIZE_VAL MACHINE_CACHE_SIZE_BASE * 10
#define MACHINE_OBJECTS_EXTRA_COUNT 10000

// 
typedef char* CString;
typedef void* CPointer;

typedef char* UG_NAMES;
typedef const char* UCHAR;
typedef const char* USTRING;
typedef struct {
    const char* key;
    const char* val;
} UG_PAIRS;

// 

#define H_PCT_OBJECT_CALLBACKS
void Object_initByType(char, void *);
void Object_freeByType(char, void *);
void Object_printByType(char, void *);
#include "../../../pure-c-tools/tools.h"

// 

USTRING helper_translate_letter(char *);
USTRING helper_translate_alias(char *);
USTRING helper_translate_something(char *);
CString helper_value_to_string(CPointer, CString);
CString helper_value_to_string_ext(CPointer, CString, CString);
// 

#include "../../build/languages.h"
#include "../../build/letters.h"
#include "../../build/aliases.h"
#include "../../build/configs.h"
#include "constant.h"
#include "tools.c"

// object

void Machine_tryLinkForGC(Object*);
Object* Machine_createObjByCurrentFreezeFlag(char, size_t);
Object* Machine_createObjByCustomFreezeFlag(char, size_t, bool);
Object* Machine_createObjNotFreeze(char, size_t);
Object* Machine_createObjAndFreeze(char, size_t);
void Machine_retainObj(Object*);
void Machine_releaseObj(Object*);
void Machine_freeObj(Object*);

// uyghur

typedef struct _Uyghur Uyghur;
typedef struct Machine Machine;
typedef struct Debug Debug;
typedef struct _Bridge Bridge;

// token

typedef struct _Token {
    struct _Object;
    const char *file;
    int line;
    int column;
    char *type;
    char *value;
    void *extra;
    void *next;
    void *last;
} Token;

Token *Token_EMPTY = NULL;
Token *Token_VARIABLE = NULL;
Token *Token_FUNCTION = NULL;
Token *Token_TEMPORARY = NULL;

// leaf

typedef struct _Leaf {
    struct _Object;
    char type; // ast type
    struct _Leaf *parent; // parent leaf
    Stack *tokens; // tokens of this ast leaf (params of expression, statement or func)
    Queue *leafs; // programs of this ast leaf (sub code block of statement or func)
} Leaf;

// value

typedef struct _Value {
    struct _Object;
    char type;
    Token *token;
    void *linka;
    void *extra;
    bool fixed;
    union { 
        bool boolean;
        double number;
        String *string;
        Object *obj;
        Array *arr;
        Hashmap *map;
    };
} Value;

bool Value_isBoolean(Value*);
bool Value_isNumber(Value*);
bool Value_isInt(Value*);
bool Value_isFlt(Value*);
bool Value_isString(Value*);
char *Value_toString(Value*);

Value *Value_EMPTY = NULL;
Value *Value_TRUE = NULL;
Value *Value_FALSE = NULL;
Value *Value_TEMPORARY = NULL;

void *INVALID_PTR = NULL;
Value *INVALID_CTN = NULL;
Value *INVALID_VAL = NULL;

#define VEmpty Value*
#define VBolean Value*
#define VNumber Value*
#define VString Value*
#define VFunction Value*
#define VList Value*
#define VDict Value*

// listable

typedef struct _Listable {
    struct _Value;
} Listable;

Listable *Listable_new(char, Token*);
char *Listable_toString(Listable *);
void Listable_print(Listable *);

// dictable

typedef struct _Dictable {
    struct _Value;
} Dictable;

Dictable *Dictable_new(char, Token*);
char *Dictable_toString(Dictable *);
void Dictable_print(Dictable *);

// runnable

typedef struct _Runnable {
    struct _Value;
} Runnable;

Runnable *Runnable_new(char, Token*);
char *Runnable_toString(Runnable *);
void Runnable_print(Runnable *);

// objective

typedef struct _Objective {
    struct _Value;
} Objective;

Objective *Objective_new(char, Token*, void*);
char *Objective_toString(Objective *);
void Objective_print(Objective *);

// holdable

typedef struct _Holdable {
    struct _Value;
} Holdable;

Holdable *Holdable_new(char, void*);
char *Holdable_toString(Holdable *);
void Holdable_print(Holdable *);

// loadable

typedef struct _Loadable {
    struct _Value;
} Loadable;

Loadable *Loadable_new(char, Token*);
char *Loadable_toString(Loadable *);
void Loadable_print(Loadable *);

typedef void (*LOADABLE_RELEASE_FUNC)(void *);

// 

void Runtime_error(char *);
void Runtime_assert(bool, char *);

// 

#include "utils.c"

#include "../objects/token.c"
#include "../objects/leaf.c"

typedef void (*WORKER)(Leaf *);
typedef void (*NATIVE)(Bridge *);

Value *Machine_newNormalValue(bool, char);
Value *Machine_newCacheableValue(char, bool);

////////////////////////////////////////////////////////////////////////////

typedef struct _Tokenizer {
    Uyghur *uyghur;
    int line;
    int column;
    int position;
    const char *path;
    const char *code;
    Token *head;
    Token *tail;
    utf8_iter *iterStatic;
    utf8_iter *iterDynamic;
} Tokenizer;

typedef struct _Parser
{
    Uyghur *uyghur;
    Token *tokens;
    Token *token;
    Leaf *tree;
    Leaf *leaf;
} Parser;

typedef struct _Executer {
    Uyghur *uyghur;
    Machine *machine;
    Debug *debug;
    Bridge *bridge;
    Leaf *tree;
    Leaf *leaf;
    Holdable *globalScope;
    Stack *callStack;
    bool isReturn;
    bool isCatch;
    char *errorMsg;
} Executer;

typedef struct _Uyghur {
    bool running;
    Hashmap *lettersMap;
    Hashmap *wordsMap;
    USTRING language;
    Tokenizer *tokenizer;
    Parser *parser;
    Executer *executer;
    Machine *machine;
    Debug *debug;
    Bridge *bridge;
} Uyghur;

Uyghur *__uyghur = NULL;
Value *Uyghur_runModule(Uyghur *, char *);
Value *Uyghur_runProgram(Uyghur *, char *);

struct Machine {
    Uyghur *uyghur;
    Object* first;
    bool sweeping;
    bool freezing;
    Stack* holders;
    Stack *calls;
    Holdable *globals;
    Holdable *rootModule;
    Holdable *currHoldable;
    Holdable *kindLgc;
    Holdable *kindNum;
    Holdable *kindStr;
    Holdable *kindList;
    Holdable *kindDict;
    int numObjects;
    int maxObjects;
    Object *cacheMap;
    Object *cacheArr;
    Object *cacheVal;
    int valueSize;
};

////////////////////////////////////////////////////////////////////////////

#define BRIDGE_STACK_TP_BOX 1
#define BRIDGE_STACK_TP_FUN 2
#define BRIDGE_STACK_TP_ARG 3
#define BRIDGE_STACK_TP_RES 4

typedef struct _Bridge {
    Uyghur *uyghur;
    Stack *stack;
    int type;
    char *last;
} Bridge;

void Bridge_pushValue(Bridge *, Value *);
Value *Bridge_popValue(Bridge *);
void Bridge_startArgument(Bridge *);
void *Bridge_send(Bridge *);
void Bridge_startResult(Bridge *);
void *Bridge_return(Bridge *);
void Bridge_call(Bridge *, char *);
void Bridge_run(Bridge *, Value *);

// push alias -> func to bridge
#define BRIDGE_BIND_NATIVE(name) \
    Bridge_bindNative(bridge, ALIAS_ ## name, native_ ## name);

////////////////////////////////////////////////////////////////////////////

struct Debug
{
    Uyghur *uyghur;
    Array *trace;
};

void Debug_pushTrace(Debug *, Token *);
void Debug_popTrace(Debug *, Token *);
void Debug_writeTrace(Debug *);
void Debug_error(Uyghur *);
void Debug_assert(Uyghur *);

////////////////////////////////////////////////////////////////////////////

#include "../objects/value.c"
#include "../objects/runnable.c"
#include "../objects/listable.c"
#include "../objects/dictable.c"
#include "../objects/holdable.c"
#include "../objects/objective.c"
#include "../objects/loadable.c"

#include "helpers.c"

#endif
