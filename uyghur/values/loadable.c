// loadable

#ifndef H_UG_HEADER
#include "../others/header.h"
#endif

#ifndef H_UG_LOADABLE
#define H_UG_LOADABLE

Loadable *Loadable_new(char tp, Token *token)
{
    tools_assert(is_type_loadable(tp), "invalid loadable type for new");
    Loadable *loadable = Machine_newCacheableValue(tp, false);
    loadable->token = token;
    loadable->extra = NULL;
    return loadable;
}

Loadable *Loadable_newStuf(void *data, CString name, CString path, CPointer releaser)
{
    char *_name = helper_translate_something(name);
    Token *token = Token_new(UG_TTYPE_NAM, _name);
    Loadable *loadable = Loadable_new(UG_TYPE_STF, token);
    loadable->obj = data;
    loadable->extra = path;
    loadable->linka = releaser;
    return loadable;
}

bool Loadable_isStuf(Loadable *this)
{
    return this != NULL && this->type == UG_TYPE_STF;
}

char *Loadable_toString(Loadable *this)
{
    return helper_value_to_string(this, "loadable", (char *)this->extra);
}

void Loadable_print(Loadable *this)
{
    printf("%s\n", Loadable_toString(this));
}

#endif
