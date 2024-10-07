// runnable

#ifndef H_UG_HEADER
#include "../others/header.h"
#endif

#ifndef H_UG_RUNNABLE
#define H_UG_RUNNABLE

Runnable *Runnable_new(char tp, void *extra)
{
    tools_assert(is_type_runnable(tp), "invalid runnable type for new");
    Runnable *runnable = Machine_newNormalValue(false, tp);
    runnable->type = tp;
    runnable->extra = extra;
    return runnable;
}

Runnable *Runnable_newWorker(WORKER worker, void *extra)
{
    Runnable *runnable = Runnable_new(UG_TYPE_WKR, extra);
    runnable->obj = worker;
    return runnable;
}

Runnable *Runnable_newNative(NATIVE native, void *extra)
{
    Runnable *runnable = Runnable_new(UG_TYPE_NTV, extra);
    runnable->obj = native;
    return runnable;
}


bool Runnable_isWorker(Runnable *this)
{
    return this != NULL && this->type == UG_TYPE_WKR;
}

bool Runnable_isNative(Runnable *this)
{
    return this != NULL && this->type == UG_TYPE_NTV;
}

char *Runnable_toString(Runnable *this)
{
    char *desc = "?";
    if (Runnable_isWorker(this)) {
        Token *token = this->extra;
        desc = token != NULL ? token->value : "?";
    } else if (Runnable_isNative(this)) {
        desc = this->extra;
    }
    char *name = get_value_name(this->type, "runnable");
    return tools_format("<%s %p %s>", name, this, desc);
}

void Runnable_print(Runnable *this)
{
    printf("%s\n", Runnable_toString(this));
    // printf("[V:%s -> %p %p]\n", get_value_name(this->type, "Runnable"), this, this->obj);
}

#endif
