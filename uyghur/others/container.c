// container

#ifndef H_BOX
#define H_BOX

#include "header.h"

typedef struct _Container {
    Hashmap *map;
    bool isBox;
    bool isScope;
} Container;

Container *Container_new()
{
    Container *container = malloc(sizeof(Container));
    container->map = Hashmap_new();
    container->isBox = false;
    container->isScope = false;
    return container;
}

Container *Container_newBox()
{
    Container *container = Container_new();
    container->isBox = true;
    return container;
}

Container *Container_newScope()
{
    Container *container = Container_new();
    container->isScope = true;
    return container;
}

void Container_set(Container *this, char *key, void *value)
{
    Hashmap_set(this->map, key, value);
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
    return tools_format("[Container => t:%d, box:%s, scope:%s]", this, b2s(this->isBox), b2s(this->isScope));
}

void Container_print(Container *this)
{
    printf(Container_toString(this));
}


void Container_free(Container *this)
{
    Hashmap_free(this->map);
    free(this);
}

#endif
