// holdable

#ifndef H_UG_HEADER
#include "../others/header.h"
#endif

#ifndef H_UG_HOLDABLE
#define H_UG_HOLDABLE

Holdable *Holdable_new(char tp, void *extra)
{
    tools_assert(is_type_holdable(tp), "invalid holdable type for new");
    return _Dictable_new(tp, extra);
}

Holdable *Holdable_newModule(char *path)
{
    return Holdable_new(UG_TYPE_MDL, path);
}

Holdable *Holdable_newScope()
{
    return Holdable_new(UG_TYPE_SCP, NULL);
}

Holdable *Holdable_newProxy(void *extra)
{
    return Holdable_new(UG_TYPE_PXY, extra);
}

//

bool Holdable_isModule(Holdable *this)
{
    return this != NULL && this->type == UG_TYPE_MDL;
}

bool Holdable_isScope(Holdable *this)
{
    return this != NULL && this->type == UG_TYPE_SCP;
}

bool Holdable_isProxy(Holdable *this)
{
    return this != NULL && this->type == UG_TYPE_PXY;
}

// 

bool Holdable_isProxyOf(Holdable *this, Value *other) {
    if (!Holdable_isProxy(this)) return false;
    Machine *machine = __uyghur->machine;
    if (this == machine->proxyLogic && Value_isBoolean(other)) return true;
    if (this == machine->proxyNumber && Value_isNumber(other)) return true;
    if (this == machine->proxyString && Value_isString(other)) return true;
    if (this == machine->proxyList && Value_isListable(other)) return true;
    if (this == machine->proxyDict && Value_isDictable(other)) return true;
    return false;
}

// 

char *Holdable_toString(Holdable *this)
{
    char *desc = "?";
    if (Holdable_isProxy(this)) {
        desc = helper_translate_something(this->extra);
    } else if (Holdable_isModule(this)) {
        desc = this->extra;
    }
    char *name = get_value_name(this->type, "holdable");
    return tools_format("<%s %p %s>", name, this, desc);
}

void Holdable_print(Holdable *this)
{
    printf("%s\n", Holdable_toString(this));
    // printf("[V:%s -> %p %p]\n", get_value_name(this->type, "Holdable"), this, this->map);
    // _hashmap_print_with_callback(this->map, "|", _container_key_print_callback);
    // printf("[Holdable]\n");
}


#endif
