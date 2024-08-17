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
#include "utf-8.c"

// #define STDSTRING_IMPLEMENTATION
// #include "stdstring.h"

// 

bool isTest = false;
#define MAX_STACK_SIZE 1000
#define MAX_TRACE_SIZE 5

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
typedef struct Parser Parser;
typedef struct Executer Executer;
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

Token *Token_EMPTY;
Token *Token_VARIABLE;
Token *Token_FUNCTION;

// value

typedef struct _Value {
    struct _Object;
    char type;
    void *extra;
    union { 
        bool boolean;
        double number;
        String *string;
        Object *obj;
        Hashmap *map;
    };
} Value;

bool Value_isBoolean(Value*);
bool Value_isNumber(Value*);
bool Value_isInt(Value*);
bool Value_isFlt(Value*);
bool Value_isString(Value*);
char *Value_toString(Value*);

Value *Value_EMPTY;
Value *Value_TRUE;
Value *Value_FALSE;

// container

typedef struct _Runnable {
    struct _Value;
} Runnable;

Runnable *Runnable_new(char, void*);
char *Runnable_toString(Runnable *);
void Runnable_print(Runnable *);

// container

typedef struct _Container {
    struct _Value;
} Container;

Container *Container_new(char, void*);
char *Container_toString(Container *);
void Container_print(Container *);

// 

#include "utils.c"

#include "../objects/token.c"
#include "../objects/leaf.c"

typedef void (*WORKER)(Leaf *);
typedef void (*CONTAINER)(Container *);
typedef void (*NATIVE)(Bridge *);

#include "../objects/value.c"
#include "../objects/runnable.c"
#include "../objects/container.c"

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

////////////////////////////////////////////////////////////////////////////

#include "helpers.c"

void *INVALID_PTR = NULL;
Container *INVALID_CTN = NULL;
Value *INVALID_VAL = NULL;

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

struct Machine {
    Uyghur *uyghur;
    Stack* stack;
    Object* first;
    bool sweeping;
    bool freezing;
    bool collects;
    Stack *calls;
    Container *globals;
    Container *rootModule;
    Container *currContainer;
    int numObjects;
    int maxObjects;
};

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

#endif
