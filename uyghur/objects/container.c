// container

#ifndef H_UG_HEADER
#include "../others/header.h"
#endif

#ifndef H_UG_BOX
#define H_UG_BOX

Container *Container_new(char tp)
{
    bool isBox = tp == UG_CTYPE_BOX;
    bool isScope = tp == UG_CTYPE_SCP;
    bool isModule = tp == UG_CTYPE_MDL;
    tools_assert(isBox || isScope || isModule, "invalid container type for new");
    Container *container = malloc(sizeof(Container));
    Object_init(container, PCT_OBJ_CNTNR);
    container->map = Hashmap_new(true);
    container->type = tp;
    return container;
}

void Container_free(Container *this)
{
    Object_release(this->map);
    Object_free(this);
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

// 

void Container_delLocation(Container *this, char *key)
{
    Hashmap_del(this->map, key);
}

// 

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
    Hashmap_foreachHashkey(this->map, __Container_copyTo, other->map);
}

// 

char *Container_toString(Container *this)
{
    char *name = "Container";
    if (Container_isScope(this)) name = "Scope";
    if (Container_isBox(this)) name = "Box";
    if (Container_isModule(this)) name = "Module";
    return tools_format("<%s p:%p>", name, this);
}

#endif
