// header

#ifndef H_HEADER
#define H_HEADER

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
#include <string.h>
#include <direct.h>
#include <stdlib.h>
#include <math.h>
#include <ctype.h>
#include <stdarg.h>
#include <time.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>

#include "../tools/hashmap.h"
#include "../tools/block.h"
#include "../tools/cursor.h"
#include "../tools/stack.h"
#include "../tools/queue.h"

#include "language.h"
#include "constant.h"
#include "tools.c"

#include "token.c"
#include "leaf.c"
#include "container.c"
#include "value.c"

#include "helpers.c"

typedef struct Tokenizer Tokenizer;
typedef struct Parser Parser;
typedef struct Executer Executer;
typedef struct Bridge Bridge;

typedef struct Uyghur {
    bool running;
    Tokenizer *tokenizer;
    Parser *parser;
    Executer *executer;
    Bridge *bridge;
} Uyghur;

Value *Uyghur_execute(Uyghur *, char *, bool);

#define BRIDGE_STACK_TP_BOX 1
#define BRIDGE_STACK_TP_FUN 2
#define BRIDGE_STACK_TP_ARG 3
#define BRIDGE_STACK_TP_RES 4

struct Bridge
{
    Uyghur *uyghur;
    Stack *stack;
    char *name;
    int type;
    char *last;
};
void Bridge_pushValue(Bridge *, Value *);
Value *Bridge_popValue(Bridge *);
void Bridge_startArgument(Bridge *);
void *Bridge_send(Bridge *);
void *Bridge_return(Bridge *);

#endif
