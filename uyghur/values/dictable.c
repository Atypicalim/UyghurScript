// dictable

#ifndef H_UG_HEADER
#include "../others/header.h"
#endif

#ifndef H_UG_DICT
#define H_UG_DICT

Dictable *_Dictable_new(char tp, Token *token, void *extra)
{
    Dictable *dictable = Machine_newCacheableValue(tp, false);
    dictable->token = token;
    dictable->extra = extra;
    return dictable;
}

Dictable *Dictable_new(char tp, Token *token)
{
    tools_assert(is_type_dictable(tp), "invalid dictable type for new");
    return _Dictable_new(tp, token, NULL);
}

Dictable *Dictable_newDct(Token *token)
{
    return Dictable_new(UG_TYPE_DCT, token);
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

Value *Dictable_getLocation(Dictable *this, char *key)
{
    return Hashmap_get(this->map, key);
}


bool _hashmap_set_check_dictable(void *_old, void *_new) {
    Value *old = (Value *)_old;
    Value *new = (Value *)_new;
    if (!old || old->type == UG_TYPE_NIL) return true;
    if (!new || new->type == UG_TYPE_NIL) return true;
    if (old->fixed && old->type != new->type) {
        Runtime_error(LANG_ERR_EXECUTER_VARIABLE_INVALID_TYPE);
        return false;
    }
    new->fixed = old->fixed;
    return true;
}

void Dictable_setLocation(Value *this, char *key, Value *value)
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
    return helper_value_to_string(this, "dictable", (char *)this->extra);
}

void Dictable_print(Dictable *this)
{
    printf("%s\n", Dictable_toString(this));
}


#endif
