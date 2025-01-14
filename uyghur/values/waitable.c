// waitable

#ifndef H_UG_HEADER
#include "../others/header.h"
#endif

#ifndef H_UG_WAITABLE
#define H_UG_WAITABLE

Waitable *Waitable_new(char tp, Token *token)
{
    tools_assert(is_type_waitable(tp), "invalid waitable type for new");
    Waitable *waitable = Machine_newCacheableValue(tp, false);
    waitable->token = token;
    waitable->extra = NULL;
    return waitable;
}

Waitable *Waitable_newTask(Value *func, CString name, CString path, CPointer releaser)
{
    char *_name = helper_translate_something(name);
    Token *token = Token_new(UG_TTYPE_NAM, _name);
    Waitable *waitable = Waitable_new(UG_TYPE_TSK, token);
    waitable->obj = func;
    waitable->extra = path;
    waitable->linka = releaser;
    return waitable;
}

bool Waitable_isTask(Waitable *this)
{
    return this != NULL && this->type == UG_TYPE_TSK;
}

char *Waitable_toString(Waitable *this)
{
    return helper_value_to_string(this, "waitable", (char *)this->extra);
}

void Waitable_print(Waitable *this)
{
    printf("%s\n", Waitable_toString(this));
}

#endif
