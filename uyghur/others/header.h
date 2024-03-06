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

#include "language.h"
#include "constant.h"
#include "tools.c"

bool isTest = false;
#define MAX_STACK_SIZE 1000
#define MAX_TRACE_SIZE 5

#define H_PCT_OBJECT_CALLBACKS
void Object_initByType(char *, void *);
void Object_freeByType(char *, void *);
void Object_printByType(char *, void *);
#include "../tools/header.h"

#include "utils.c"

#include "../objects/token.c"
#include "../objects/leaf.c"
#include "../objects/container.c"
#include "../objects/value.c"

#include "helpers.c"

typedef struct Tokenizer Tokenizer;
typedef struct Parser Parser;
typedef struct Executer Executer;
typedef struct Trace Trace;
typedef struct Bridge Bridge;

typedef struct Uyghur {
    bool running;
    Tokenizer *tokenizer;
    Parser *parser;
    Executer *executer;
    Trace *trace;
    Bridge *bridge;
} Uyghur;

Value *Uyghur_runPath(Uyghur *, char *);

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
Value *Bridge_nextValue(Bridge *);
void Bridge_startArgument(Bridge *);
void *Bridge_send(Bridge *);
void *Bridge_return(Bridge *);
void Bridge_bind(Bridge *, char *, void *, char);
void Bridge_run(Bridge *, Value *);

struct Trace
{
    Uyghur *uyghur;
    Array *array;
};
void Trace_push(Trace *, Token *);
void Trace_pop(Trace *, Token *);
void Trace_write(Trace *);

#endif
