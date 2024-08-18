// runnable

#ifndef H_UG_HEADER
#include "../others/header.h"
#endif

#ifndef H_UG_RUNNABLE
#define H_UG_RUNNABLE

Runnable *Runnable_new(char tp, void *extra)
{
    tools_assert(is_type_runnable(tp), "invalid runnable type for new");
    Runnable *runnable = _value_newValueBySize(false, tp, sizeof(Runnable));
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
    char *name = get_value_name(this->type, "runnable");
    return tools_format("<%s p:%p>", name, this);
}

#endif
