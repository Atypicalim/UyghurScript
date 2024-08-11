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
    runnable->map = Hashmap_new(true);
    Machine_tryLinkForGC(runnable->map);
    runnable->type = tp;
    runnable->extra = extra;
    return runnable;
}

void Runnable_free(Runnable *this)
{
#if GC_USE_COUNTING
    Machine_releaseObj(this->map);
    Machine_freeObj(this);
#elif GC_USE_SWEEPING
    //
#endif
}

Runnable *Runnable_newWorker(WORKER worker, void *extra)
{
    Runnable *runnable = Runnable_new(UG_TYPE_WKR, extra);
    runnable->object = worker;
    return runnable;
}

Runnable *Runnable_newNative(NATIVE native, void *extra)
{
    Runnable *runnable = Runnable_new(UG_TYPE_NTV, extra);
    runnable->object = native;
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
    char *name = "Runnable";
    if (Runnable_isWorker(this)) name = "Worker";
    if (Runnable_isNative(this)) name = "Native";
    return tools_format("<%s p:%p>", name, this);
}

#endif
