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

#include "hashmap.h"
#include "block.h"
#include "stack.h"
#include "queue.h"

#include "language.h"
#include "constant.h"
#include "tools.c"
#include "helpers.c"

typedef struct Tokenizer Tokenizer;
typedef struct Parser Parser;

typedef struct Uyghur {
    bool running;
    Queue *TTYPES_GROUP_DEFINE;
    Queue *TTYPES_GROUP_STRING;
    Queue *TTYPES_GROUP_NUMBER;
    Queue *TTYPES_GROUP_LOGICS;
    Tokenizer *tokenizer;
    Parser *parser;
} Uyghur;

#endif
