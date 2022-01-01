// value

#ifndef H_VALUE
#define H_VALUE

#include "header.h"

Hashmap *valueCache = NULL;

typedef struct ValueNode {
    char *type;
    bool boolean;
    double number;
    char *string;
    void *object;
    void *extra;
    int reference;
} Value;

char *_get_cache_tag(char *type, bool boolean, double number, char *string)
{
    if (is_equal(type, RTYPE_EMPTY)) return tools_format("<R-VALUE:%s>", type);
    if (is_equal(type, RTYPE_BOOLEAN)) return tools_format("<R-VALUE:%s:%s>", type, b2s(boolean));
    if (is_equal(type, RTYPE_NUMBER) && round(number) == number)
    {
        return tools_format("<R-VALUE:%s:%f>", type, number);
    }
    if (is_equal(type, RTYPE_STRING) && strlen(string) <= CACHE_STRING_MAX_LENGTH)
    {
        return tools_format("<R-VALUE:%s:%s>", type, string);
    }
    return NULL;
}

 Value *Value_new(char *type, bool boolean, double number, char *string, void *object, void *extra)
{
    // cache
    if (valueCache == NULL) valueCache = Hashmap_new(NULL);
    char *tag = _get_cache_tag(type, boolean, number, string);
    bool canCache = tag != NULL;
    int hashValue = 0;
    // get
    if (canCache) {
        hashValue =hashValue = hash(tag);
        Value *v = Hashmap_getWithHash(valueCache, tag, hashValue);
        if (v != NULL)
        {
            free(tag);
            // if (string != NULL)
            // {
            //     free(string);
            //     string = NULL;
            // }
            // if (object != NULL)
            // {
            //     free(object);
            //     object = NULL;
            // }
            return v;
        }
    }
    // create
    Value *value = malloc(sizeof(Value));
    value->type = type;
    value->boolean = boolean;
    value->number = number;
    value->string = string;
    value->object = object;
    value->extra = extra;
    value->reference = 0;
    // save
    if (canCache)
    {
        Hashmap_setWithHash(valueCache, tag, value, hashValue);
    }
    // return
    return value;
}

Value *Value_newEmpty(void *extra)
{
    return Value_new(RTYPE_EMPTY, NULL, 0, NULL, NULL, extra);
}

Value *Value_newBox(Container *box, void *extra)
{
    return Value_new(RTYPE_BOX, NULL, 0, NULL, box, extra);
}

Value *Value_newBoolean(bool boolean, void *extra)
{
    return Value_new(RTYPE_BOOLEAN, boolean, 0, NULL, NULL, extra);
}

Value *Value_newNumber(double number, void *extra)
{
    return Value_new(RTYPE_NUMBER, NULL, number, NULL, NULL, extra);
}

Value *Value_newString(char *string, void *extra)
{
    return Value_new(RTYPE_STRING, NULL, 0, string, NULL, extra);
}

Value *Value_newFunction(void *function, void *extra)
{
    return Value_new(RTYPE_FUNCTION ,NULL, 0, NULL, function, extra);
}

Value *Value_newCFunction(void *function, void *extra)
{
    return Value_new(RTYPE_CFUNCTION ,NULL, 0, NULL, function, extra);
}

Value *Value_newObject(void *object, void *extra)
{
    return Value_new(RTYPE_UNKNOWN ,NULL, 0, NULL, object, extra);
}


void Value_print(Value *this)
{
    if (is_equal(this->type, RTYPE_EMPTY))
    {
        printf("[RV => t:%s v:%s]\n", this->type, TVALUE_EMPTY);
    }
    else if (is_equal(this->type, RTYPE_BOOLEAN))
    {
        bool value = this->boolean;
        printf("[RV => t:%s v:%s]\n", this->type, value ? TVALUE_TRUE : TVALUE_FALSE);
    }
    else if (is_equal(this->type, RTYPE_NUMBER))
    {
        double value = this->number;
        printf("[RV => t:%s v:%f]\n", this->type, value);
    }
    else if (is_equal(this->type, RTYPE_STRING))
    {
        char *value = this->string;
        printf("[RV => t:%s v:%s]\n", this->type, value);
    }
    else
    {
        printf("[RV => t:%s]\n", this->type);
    }
}

// TODO return Value
char *Value_toString(Value *this)
{
    if (is_equal(this->type, RTYPE_EMPTY))
    {
        return tools_format("%s", TVALUE_EMPTY);
    }
    if (is_equal(this->type, RTYPE_BOOLEAN))
    {
        double value = this->boolean;
        return tools_format("%s", value ? TVALUE_TRUE : TVALUE_FALSE);
    }
    if (is_equal(this->type, RTYPE_NUMBER))
    {
        double value = this->number;
        return tools_format("%f", value);
    }
    if (is_equal(this->type, RTYPE_STRING))
    {
        char *value = this->string;
        return value;
    }
    else
    {
        void *value = this->object;
        return tools_format("[RV => t:%s p:%d]\n", this->type, value != NULL ? value : 0);
    } 
}

Value *Value_toBoolean(Value *this)
{
    if (is_equal(this->type, RTYPE_NUMBER))
    {
        return Value_newBoolean(this->number > 0, NULL);
    }
    else if (is_equal(this->type, RTYPE_STRING))
    {
        return Value_newBoolean(is_equal(this->string, TVALUE_TRUE), NULL);
    }
    else if (is_equal(this->type, RTYPE_EMPTY))
    {
        return Value_newBoolean(false, NULL);
    }
    else if (is_equal(this->type, RTYPE_BOOLEAN))
    {
        return this;
    }
    else
    {
        return Value_newBoolean(false, NULL);
    }
}

Value *Value_toNumber(Value *this)
{

    if (is_equal(this->type, RTYPE_NUMBER))
    {
        return this;
    }
    else if (is_equal(this->type, RTYPE_STRING))
    {
        return Value_newNumber(atof(this->string), NULL);
    }
    else if (is_equal(this->type, RTYPE_EMPTY))
    {
        return Value_newNumber(0, NULL);
    }
    else if (is_equal(this->type, RTYPE_BOOLEAN))
    {
        return Value_newNumber(this->boolean ? 1 : 0, NULL);
    }
    else
    {
        return Value_newNumber(0, NULL);
    }
}

void Value_increaseReference(Value *this)
{
    this->reference++;  
}

void Value_decreaseReference(Value *this)
{
    if (this->reference == 0) return;
    this->reference--;
}

void Value_free(Value *this)
{
    Value_decreaseReference(this);
    char *tag = _get_cache_tag(this->type, this->boolean, this->number, this->string);
    if (tag != NULL)
    {
        free(tag);
    }
    else if (this->reference == 0)
    {
        if (is_equal(this->type, RTYPE_STRING))
        {
            free(this->string);
        }
        if (is_equal(this->type, RTYPE_BOX))
        {
            Container_free(this->object);
        }
        free(this);
    }
    // TODO: ug free pointers
}

#endif
