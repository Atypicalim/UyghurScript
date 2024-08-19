// holdable

#ifndef H_UG_HEADER
#include "../others/header.h"
#endif

#ifndef H_UG_BOX
#define H_UG_BOX

Holdable *Holdable_new(char tp, void *extra)
{
    tools_assert(is_type_holdable(tp), "invalid holdable type for new");
    Holdable *holdable = _value_newValueBySize(false, tp, sizeof(Holdable));
    holdable->map = Hashmap_new(true);
    // log_debug("new-%s: %p %p", get_value_name(tp, "holdable"), holdable, holdable->map);
    Machine_tryLinkForGC(holdable->map);
    holdable->type = tp;
    holdable->extra = extra;
    return holdable;
}

Holdable *Holdable_newModule(char *path)
{
    return Holdable_new(UG_TYPE_MDL, path);
}

Holdable *Holdable_newScope()
{
    return Holdable_new(UG_TYPE_SCP, NULL);
}

Holdable *Holdable_newBox(void *extra)
{
    return Holdable_new(UG_TYPE_BOX, extra);
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

bool Holdable_isBox(Holdable *this)
{
    return this != NULL && this->type == UG_TYPE_BOX;
}

// 

char *Holdable_toString(Holdable *this)
{
    char *desc = "?";
    if (Holdable_isBox(this)) {
        Token *token = this->extra;
        desc = token != NULL ? token->value : "?";
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
