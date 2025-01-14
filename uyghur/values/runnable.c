// runnable

#ifndef H_UG_HEADER
#include "../others/header.h"
#endif

#ifndef H_UG_RUNNABLE
#define H_UG_RUNNABLE

Runnable *Runnable_new(char tp, Token *token)
{
    tools_assert(is_type_runnable(tp), "invalid runnable type for new");
    Runnable *runnable = Machine_newCacheableValue(tp, false);
    runnable->token = token;
    runnable->extra = NULL;
    return runnable;
}

Runnable *Runnable_newWorker(WORKER worker, Token *token, void *environment)
{
    Runnable *runnable = Runnable_new(UG_TYPE_WKR, token);
    runnable->obj = worker;
    runnable->linka = environment;
    return runnable;
}

Runnable *Runnable_newNative(NATIVE native, Token *token)
{
    Runnable *runnable = Runnable_new(UG_TYPE_NTV, token);
    runnable->obj = native;
    runnable->linka = NULL;
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
    return helper_value_to_string(this, "runnable", (char *)this->extra);
}

void Runnable_print(Runnable *this)
{
    printf("%s\n", Runnable_toString(this));
}

#endif
