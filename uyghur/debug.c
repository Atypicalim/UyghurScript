// debug

#include "others/header.h"

void Debug_reset(Debug *this)
{
    Object_release(this->trace);
}

Debug *Debug_new(Uyghur *uyghur)
{
    Debug *debug = malloc(sizeof(Debug));
    debug->uyghur = uyghur;
    debug->trace = Array_new();
    return debug;
}

void Debug_pushTrace(Debug *this, Token *element)
{
    Token *last = Array_getLast(this->trace);
    if (last != NULL && last == element) {
        return;
    }
    Array_push(this->trace, element);
    if (this->trace->length > MAX_TRACE_SIZE) {
        Array_delFirst(this->trace);
    }
}

void Debug_popTrace(Debug *this, Token *element)
{
    Array_pop(this->trace);
}

void Debug_writeTrace(Debug *this)
{
    for (int i = this->trace->length - 1; i >= 0; i--) {
        Token *t = Array_get(this->trace, i);
        char *s = format_some_place(t);
        printf("[%s] %s %s\n", UG_TAG_TRACE, s, t->value);
    }
}
