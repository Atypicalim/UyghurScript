// listable

#ifndef H_UG_HEADER
#include "../others/header.h"
#endif

#ifndef H_UG_LIST
#define H_UG_LIST

Listable *Listable_new(char tp, Token *token)
{
    Listable *listable = Machine_newCacheableValue(tp, false);
    tools_assert(is_type_listable(tp), "invalid listable type for new");
    listable->token = token;
    listable->extra = NULL;
    return listable;
}

Listable *Listable_newLst(Token *token)
{
    return Listable_new(UG_TYPE_LST, token);
}

bool Listable_isLst(Listable *this)
{
    return this != NULL && this->type == UG_TYPE_LST;
}

// 

bool Listable_push(Listable *this, Value *item) {
    return Array_push(this->arr, item);
}

Value *Listable_getIndex(Listable *this, int index) {
    return Array_get(this->arr, index);
}

bool Listable_setIndex(Listable *this, int index, Value *item) {
    return Array_set(this->arr, index, item);
}

int Listable_getCount(Listable *this) {
    return this->arr->length;
}

// 

char *Listable_toString(Listable *this)
{
    return helper_value_to_string(this, "listable", (char *)this->extra);
}

void Listable_print(Listable *this)
{
    printf("%s\n", Listable_toString(this));
}


#endif
