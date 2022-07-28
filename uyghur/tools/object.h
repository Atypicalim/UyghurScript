
#ifndef H_PCT_UG_OBJECT
#define H_PCT_UG_OBJECT

typedef struct _Object {
    void *objType;
} Object;

void Object_init(void *_this, void *_objType)
{
    Object *this = _this;
    this->objType = _objType;
}

Object *Object_new()
{
    Object *object = (Object *)malloc(sizeof(Object));
    Object_init(object, PCT_OBJ_OBJECT);
    return object;
}

void Object_print(void *_this)
{
    Object *this = _this;
    printf("<object,type:%s>\n", this->objType);
}

void Object_free(void *_this)
{
    Object *this = _this;
    free(this);
}

#endif
