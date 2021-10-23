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

#include "../tools/hashmap.h"
#include "../tools/block.h"
#include "../tools/stack.h"
#include "../tools/queue.h"

#include "language.h"
#include "constant.h"
#include "tools.c"

#include "token.c"
#include "leaf.c"
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

#endif
