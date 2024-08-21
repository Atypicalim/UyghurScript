// listable

#ifndef H_UG_HEADER
#include "../others/header.h"
#endif

#ifndef H_UG_LIST
#define H_UG_LIST

Listable *Listable_new(char tp, void *extra)
{
    tools_assert(is_type_listable(tp), "invalid listable type for new");
    Listable *listable = _value_newValueBySize(false, tp, sizeof(Listable));
    listable->arr = Array_new();
    Machine_tryLinkForGC(listable->arr);
    // log_debug("new-%s: %p %p", get_value_name(tp, "listable"), listable, listable->arr);
    listable->type = tp;
    listable->extra = extra;
    return listable;
}

Listable *Listable_newLst(void *extra)
{
    return Listable_new(UG_TYPE_LST, extra);
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

// 

char *Listable_toString(Listable *this)
{
    char *desc = "?";
    if (Listable_isLst(this)) {
        Token *token = this->extra;
        desc = token != NULL ? token->value : "?";
    }
    char *name = get_value_name(this->type, "listable");
    return tools_format("<%s %p %s>", name, this, desc);
}

void Listable_print(Listable *this)
{
    printf("%s\n", Listable_toString(this));
    // printf("[V:%s -> %p %p]\n", get_value_name(this->type, "Listable"), this, this->map);
    // _hashmap_print_with_callback(this->map, "|", _container_key_print_callback);
    // printf("[Listable]\n");
}


#endif
