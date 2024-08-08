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
#define GC_USE_COUNTING false
#define GC_USE_SWEEPING true

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
Object* Machine_createObjTryGC(char, size_t);
void Machine_retainObj(Object*);
void Machine_releaseObj(Object*);
void Machine_freeObj(Object*);

// value

typedef struct _Value {
    struct _Object;
    char type;
    bool boolean;
    char character;
    double number;
    String *string;
    void *object;
    void *extra;
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

typedef struct _Container {
    struct _Object;
    Hashmap *map;
    char type;
} Container;

Container *Container_new(char);

// 

#include "utils.c"

#include "../objects/token.c"
#include "../objects/leaf.c"
#include "../objects/container.c"
#include "../objects/value.c"

////////////////////////////////////////////////////////////////////////////

typedef struct _Uyghur Uyghur;

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

typedef struct Parser Parser;
typedef struct Executer Executer;
typedef struct Machine Machine;
typedef struct Debug Debug;
typedef struct Bridge Bridge;

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

struct Bridge
{
    Uyghur *uyghur;
    Stack *stack;
    Cursor *cursor;
    int type;
    char *last;
};
void Bridge_pushValue(Bridge *, Value *);
Value *Bridge_popValue(Bridge *);
void Bridge_startArgument(Bridge *);
void *Bridge_send(Bridge *);
void Bridge_startResult(Bridge *);
void *Bridge_return(Bridge *);
void Bridge_call(Bridge *, char *);
void Bridge_run(Bridge *, Value *);

typedef void (*NATIVE)(Bridge *);

// push alias -> func to bridge
#define BRIDGE_BIND_NATIVE(name) \
    Bridge_bindNative(bridge, ALIAS_ ## name, native_ ## name);

////////////////////////////////////////////////////////////////////////////

struct Machine {
    Uyghur *uyghur;
    Stack* stack;
    Object* first;
    bool freezing;
    bool collecting;
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
