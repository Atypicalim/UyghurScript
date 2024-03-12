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

// 

bool isTest = false;
#define MAX_STACK_SIZE 1000
#define MAX_TRACE_SIZE 5

typedef char* UG_NAMES;
typedef struct {
    const char* key;
    const char* val;
} UG_PAIRS;

// 

#include "language.h"
#include "constant.h"
#include "tools.c"

// 

#define H_PCT_OBJECT_CALLBACKS
void Object_initByType(char, void *);
void Object_freeByType(char, void *);
void Object_printByType(char, void *);
#include "../tools/header.h"

// 

#include "utils.c"

#include "../objects/token.c"
#include "../objects/leaf.c"
#include "../objects/container.c"
#include "../objects/value.c"

#include "helpers.c"

// 

typedef struct Tokenizer Tokenizer;
typedef struct Parser Parser;
typedef struct Executer Executer;
typedef struct Debug Debug;
typedef struct Bridge Bridge;

// 

typedef struct Uyghur {
    bool running;
    Tokenizer *tokenizer;
    Parser *parser;
    Executer *executer;
    Debug *debug;
    Bridge *bridge;
} Uyghur;

// 

#define BRIDGE_STACK_TP_BOX 1
#define BRIDGE_STACK_TP_FUN 2
#define BRIDGE_STACK_TP_ARG 3
#define BRIDGE_STACK_TP_RES 4

struct Bridge
{
    Uyghur *uyghur;
    Stack *stack;
    Cursor *cursor;
    char *name;
    int type;
    char *last;
};
void Bridge_pushValue(Bridge *, Value *);
Value *Bridge_popValue(Bridge *);
void Bridge_startArgument(Bridge *);
void *Bridge_send(Bridge *);
void Bridge_startResult(Bridge *);
void *Bridge_return(Bridge *);
void Bridge_call(Bridge *);
void Bridge_run(Bridge *, Value *);

typedef void (*NATIVE)(Bridge *);

// 

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
