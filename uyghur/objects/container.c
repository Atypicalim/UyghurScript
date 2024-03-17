// container

#ifndef H_UG_HEADER
#include "../others/header.h"
#endif

#ifndef H_UG_BOX
#define H_UG_BOX

typedef struct _Container {
    struct _Object;
    Hashmap *map;
    char type;
} Container;

Container *Container_new(char tp)
{
    bool isBox = tp == UG_CTYPE_BOX;
    bool isScope = tp == UG_CTYPE_SCP;
    bool isModule = tp == UG_CTYPE_MDL;
    tools_assert(isBox || isScope || isModule, "invalid container type for new");
    Container *container = malloc(sizeof(Container));
    Object_init(container, PCT_OBJ_CNTNR);
    container->map = Hashmap_new();
    container->type = tp;
    return container;
}

Container *Container_newBox()
{
    return Container_new(UG_CTYPE_BOX);
}

Container *Container_newScope()
{
    return Container_new(UG_CTYPE_SCP);
}

Container *Container_newModule()
{
    return Container_new(UG_CTYPE_MDL);
}

void *Container_del(Container *this, char *key)
{
    void *deleted = Hashmap_del(this->map, key);
    if (deleted != NULL) Object_release(deleted);
    return deleted;
}

void *Container_get(Container *this, char *key)
{
    return Hashmap_get(this->map, key);
}

void *Container_set(Container *this, char *key, void *value)
{
    if (value == NULL) return Container_del(this, key);
    Object_retain(value);
    void *replaced = Hashmap_set(this->map, key, value);
    if (replaced != NULL) Object_release(replaced);
    return replaced;
}

char *Container_toString(Container *this)
{
    return tools_format("[Container => type:%c]", this->type);
}

void Container_print(Container *this)
{
    printf(Container_toString(this));
}

bool Container_isScope(Container *this)
{
    return this->type == UG_CTYPE_SCP;
}

bool Container_isBox(Container *this)
{
    return this->type == UG_CTYPE_BOX;
}

bool Container_isModule(Container *this)
{
    return this->type == UG_CTYPE_MDL;
}

void Container_free(Container *this)
{
    Object_release(this->map);
    Object_free(this);
}

#endif
