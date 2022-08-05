
#ifndef H_PCT_UG_OBJECT
#define H_PCT_UG_OBJECT

typedef struct _Object {
    void *objType;
    int referenceCount;
} Object;

void Object_init(void *_this, void *_objType)
{
    Object *this = _this;
    this->objType = _objType;
    this->referenceCount = 1;
    #ifdef H_PCT_OBJECT_CALLBACKS
    Object_initByType(this->objType, this);
    #endif
}

Object *Object_new()
{
    Object *object = (Object *)pct_mallloc(sizeof(Object));
    Object_init(object, PCT_OBJ_OBJECT);
    return object;
}


void Object_free(void *_this)
{
    Object *this = _this;
    pct_free(this);
}

void Object_retain(void *_this)
{
    if (_this == NULL) tools_error("null pointer to object retain");
    Object *this = _this;
    this->referenceCount++;
}

void Object_release(void *_this)
{
    if (_this == NULL) tools_error("null pointer to object release");
    Object *this = _this;
    this->referenceCount--;
    if (this->referenceCount <= 0) {
        #ifdef H_PCT_OBJECT_CALLBACKS
        Object_freeByType(this->objType, this);
        #elif
        Object_free(this);
        #endif
    }
}

void Object_print(void *_this)
{
    if (_this == NULL) tools_error("null pointer to object print");
    Object *this = _this;
    #ifdef H_PCT_OBJECT_CALLBACKS
    Object_printByType(this->objType, this);
    #elif
    printf("<object,type:%s,address:%d>\n", this->objType, this);
    #endif
}

#endif
