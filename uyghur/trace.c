// trace

#include "others/header.h"

void Trace_reset(Trace *this)
{
    Object_release(this->array);
}

Trace *Trace_new(Uyghur *uyghur)
{
    Trace *trace = malloc(sizeof(Trace));
    trace->uyghur = uyghur;
    trace->array = Array_new();
    return trace;
}

void Trace_push(Trace *this, Token *element)
{
    Token *last = Array_getLast(this->array);
    if (last != NULL && last == element) {
        return;
    }
    Array_push(this->array, element);
    if (this->array->length > MAX_TRACE_SIZE) {
        Array_delFirst(this->array);
    }
}

void Trace_pop(Trace *this, Token *element)
{
    Array_pop(this->array);
}

void Trace_write(Trace *this)
{
    // Array_print(this->array);
    for (int i = this->array->length - 1; i >= 0; i--) {
        Token *t = Array_get(this->array, i);
        char *s = format_some_place(t);
        printf("[%s] %s %s\n", LANG_LOG, s, t->value);
    }
}
