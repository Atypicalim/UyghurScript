// container

#ifndef H_UG_HEADER
#include "../others/header.h"
#endif

#ifndef H_UG_BOX
#define H_UG_BOX

typedef struct _Container {
    struct _Object;
    Hashmap *map;
    char *type;
} Container;

Container *Container_new(char *tp)
{
    bool isBox = is_equal(tp, CONTAINER_TYPE_BOX);
    bool isScope = is_equal(tp, CONTAINER_TYPE_SCOPE);
    bool isModule = is_equal(tp, CONTAINER_TYPE_MODULE);
    tools_assert(isBox || isScope || isModule, "invalid container type for new");
    Container *container = malloc(sizeof(Container));
    Object_init(container, PCT_OBJ_CONTAINER);
    container->map = Hashmap_new();
    container->type = tp;
    return container;
}

Container *Container_newBox()
{
    return Container_new(CONTAINER_TYPE_BOX);
}

Container *Container_newScope()
{
    return Container_new(CONTAINER_TYPE_SCOPE);
}

Container *Container_newModule()
{
    return Container_new(CONTAINER_TYPE_MODULE);
}

void *Container_set(Container *this, char *key, void *value)
{
    return Hashmap_set(this->map, key, value);
}

void *Container_get(Container *this, char *key)
{
    return Hashmap_get(this->map, key);
}

void Container_del(Container *this, char *key)
{
    Hashmap_del(this->map, key);
}

char *Container_toString(Container *this)
{
    return tools_format("[Container => type:%s]", this->type);
}

void Container_print(Container *this)
{
    printf(Container_toString(this));
    Hashmap_print(this->map);
}

bool Container_isScope(Container *this)
{
    return is_equal(this->type, CONTAINER_TYPE_SCOPE);
}

bool Container_isBox(Container *this)
{
    return is_equal(this->type, CONTAINER_TYPE_BOX);
}

bool Container_isModule(Container *this)
{
    return is_equal(this->type, CONTAINER_TYPE_MODULE);
}

void Container_free(Container *this)
{
    Object_release(this->map);
    Object_free(this);
}

#endif
