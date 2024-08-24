// dictable

#ifndef H_UG_HEADER
#include "../others/header.h"
#endif

#ifndef H_UG_DICT
#define H_UG_DICT

Dictable *_Dictable_new(char tp, void *extra)
{
    Dictable *dictable = _value_newValueBySize(false, tp, sizeof(Dictable));
    dictable->map = Hashmap_new(IS_GC_COUNTING);
    #if IS_GC_LINK_DICTABLE_MAP
    Machine_tryLinkForGC(dictable->map);
    #endif
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

void Dictable_delLocation(Dictable *this, char *key)
{
    Hashmap_del(this->map, key);
}

void *Dictable_getLocation(Value *this, char *key)
{
    return Hashmap_get(this->map, key);
}


bool _hashmap_set_check_dictable(Value *old, Value *new) {
    if (!old || old->type == UG_TYPE_NIL) return true;
    if (!new || new->type == UG_TYPE_NIL) return true;
    if (old->fixed && old->type != new->type) {
        Runtime_error(LANG_ERR_EXECUTER_VARIABLE_INVALID_TYPE);
        return false;
    }
    new->fixed = old->fixed;
    return true;
}

void Dictable_setLocation(Value *this, char *key, void *value)
{
    if (value == NULL) return Dictable_delLocation(this, key);
    // Hashmap_set(this->map, key, value);
    Hashmap_setByCheck(this->map, key, value, _hashmap_set_check_dictable);
}

int Dictable_getCount(Dictable *this) {
    int count = 0;
    Hashkey *ptr;
    for (int i = 0; i < this->map->size; ++i) {
        ptr = this->map->bucket[i];
        while (ptr != NULL) {
            count++;
            ptr = ptr->next;
        }
    }
    return count;
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
