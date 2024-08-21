// dictable

#ifndef H_UG_HEADER
#include "../others/header.h"
#endif

#ifndef H_UG_DICT
#define H_UG_DICT

Dictable *_Dictable_new(char tp, void *extra)
{
    Dictable *dictable = _value_newValueBySize(false, tp, sizeof(Dictable));
    dictable->map = Hashmap_new(true);
    Machine_tryLinkForGC(dictable->map);
    // log_debug("new-%s: %p %p", get_value_name(tp, "dictable"), dictable, dictable->obj);
    dictable->type = tp;
    dictable->extra = extra;
    return dictable;
}

Dictable *Dictable_new(char tp, void *extra)
{
    tools_assert(is_type_dictable(tp), "invalid dictable type for new");
    return _Dictable_new(tp, extra);
}

Dictable *Dictable_newDct(void *extra)
{
    return Dictable_new(UG_TYPE_DCT, extra);
}

bool Dictable_isDct(Dictable *this)
{
    return this != NULL && this->type == UG_TYPE_DCT;
}

// 

char *Dictable_toString(Dictable *this)
{
    char *desc = "?";
    if (Dictable_isDct(this)) {
        Token *token = this->extra;
        desc = token != NULL ? token->value : "?";
    }
    char *name = get_value_name(this->type, "dictable");
    return tools_format("<%s %p %s>", name, this, desc);
}

void Dictable_print(Dictable *this)
{
    printf("%s\n", Dictable_toString(this));
    // printf("[V:%s -> %p %p]\n", get_value_name(this->type, "Dictable"), this, this->map);
    // _hashmap_print_with_callback(this->map, "|", _container_key_print_callback);
    // printf("[Dictable]\n");
}


#endif
