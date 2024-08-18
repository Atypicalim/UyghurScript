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

Holdable *Holdable_newModule()
{
    return Holdable_new(UG_TYPE_MDL, NULL);
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
    return this->type == UG_TYPE_MDL;
}

bool Holdable_isScope(Holdable *this)
{
    return this->type == UG_TYPE_SCP;
}

bool Holdable_isBox(Holdable *this)
{
    return this->type == UG_TYPE_BOX;
}

// 

char *Holdable_toString(Holdable *this)
{
    char *name = get_value_name(this->type, "holdable");
    return tools_format("<%s p:%p>", name, this);
}

#endif
