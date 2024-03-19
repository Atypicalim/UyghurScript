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
    container->map = Hashmap_new();
    container->array = Array_new();
    container->type = tp;
    return container;
}

void Container_free(Container *this)
{
    Object_release(this->map);
    Object_release(this->array);
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

void *Container_getByStringLocation(Container *this, char *key)
{
    return Container_get(this, key);
}

void *Container_getByTypedLocation(Container *this, char tp, char *key)
{
    return Container_get(this, key);
}

void *Container_setByStringLocation(Container *this, char *key, void *value)
{
    return Container_set(this, key, value);
}

void *Container_setByTypedLocation(Container *this, char tp, char *key, void *value)
{
    return Container_set(this, key, value);
}


void *Container_getByValueKey(Container *this, Value *key)
{
    if (Value_isInt(key)) {

    } else if (Value_isString(key)) {
        Container_get(this, key->string->data);
    } else {
        tools_error(LANG_ERR_UYGHUR_EXCEPTION);
    }
}

void *Container_setByValueKey(Container *this, Value *key, Value *value)
{
    if (Value_isInt(key)) {

    } else if (Value_isString(key)) {
        Container_set(this, key->string->data, value);
    } else {
        tools_error(LANG_ERR_UYGHUR_EXCEPTION);
    }
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

char *Container_toString(Container *this)
{
    return tools_format("[Container => p:%d t:%c]", this, this->type);
}

#endif
