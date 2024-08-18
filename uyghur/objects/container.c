// container

#ifndef H_UG_HEADER
#include "../others/header.h"
#endif

#ifndef H_UG_BOX
#define H_UG_BOX

Container *Container_new(char tp, void *extra)
{
    tools_assert(is_type_container(tp), "invalid container type for new");
    Container *container = _value_newValueBySize(false, tp, sizeof(Container));
    container->map = Hashmap_new(true);
    // log_debug("new-%s: %p %p", get_value_name(tp, "container"), container, container->map);
    Machine_tryLinkForGC(container->map);
    container->type = tp;
    container->extra = extra;
    return container;
}

Container *Container_newModule()
{
    return Container_new(UG_TYPE_MDL, NULL);
}

Container *Container_newScope()
{
    return Container_new(UG_TYPE_SCP, NULL);
}

Container *Container_newBox(void *extra)
{
    return Container_new(UG_TYPE_BOX, extra);
}

// 

void Container_delLocation(Container *this, char *key)
{
    Hashmap_del(this->map, key);
}

void *Container_getLocation(Container *this, char *key)
{
    return Hashmap_get(this->map, key);
}

void Container_setLocation(Container *this, char *key, void *value)
{
    if (value == NULL) return Container_delLocation(this, key);
    Hashmap_set(this->map, key, value);
}

//

bool Container_isModule(Container *this)
{
    return this->type == UG_TYPE_MDL;
}

bool Container_isScope(Container *this)
{
    return this->type == UG_TYPE_SCP;
}

bool Container_isBox(Container *this)
{
    return this->type == UG_TYPE_BOX;
}

// 

void __Container_copyTo(Hashkey *hashkey, Hashmap *other)
{
    String *key = hashkey->key;
    Value *val = hashkey->value;
    char *_key = String_get(key);
    Hashmap_set(other, _key, val);
}

void Container_copyTo(Container *this, Container *other)
{
    Hashmap_foreachItem(this->map, __Container_copyTo, other->map);
}

// 

char *Container_toString(Container *this)
{
    char *name = get_value_name(this->type, "container");
    return tools_format("<%s p:%p>", name, this);
}

#endif
