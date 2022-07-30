// pure c tools

#ifndef H_PCT_PURE_C_TOOLS
#define H_PCT_PURE_C_TOOLS

#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <stdio.h>
#include <stdio.h>
#include <stdbool.h>
#include <ctype.h>
#include <stdarg.h>

// object types
#define PCT_OBJ_OBJECT "PCT_OBJ_OBJECT"
#define PCT_OBJ_STRING "PCT_OBJ_STRING"
#define PCT_OBJ_ARRAY "PCT_OBJ_ARRAY"
#define PCT_OBJ_CURSOR "PCT_OBJ_CURSOR"
#define PCT_OBJ_STACK "PCT_OBJ_STACK"
#define PCT_OBJ_QUEUE "PCT_OBJ_QUEUE"
#define PCT_OBJ_HASHMAP "PCT_OBJ_HASHMAP"
#define PCT_OBJ_FOLIAGE "PCT_OBJ_FOLIAGE"
#define PCT_OBJ_BLOCK "PCT_OBJ_BLOCK"

void *pct_mallloc(size_t size)
{
    return malloc(size);
}

void *pct_realloc(void *object, size_t size)
{
    return realloc(object, size);
}

void pct_free(void *object)
{
    free(object);
}

#include "../tools/object.h"
#include "../tools/string.h"
#include "../tools/cursor.h"
#include "../tools/hashmap.h"
#include "../tools/foliage.h"
#include "../tools/block.h"
#include "../tools/queue.h"
#include "../tools/stack.h"
#include "../tools/array.h"

#endif
