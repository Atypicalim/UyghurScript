// value

#ifndef H_UG_HEADER
#include "../others/header.h"
#endif

#ifndef H_UG_VALUE
#define H_UG_VALUE

Hashmap *valueCache = NULL;

typedef struct ValueNode {
    struct _Object;
    char *type;
    bool boolean;
    double number;
    String *string;
    void *object;
    void *extra;
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

 Value *Value_new(char *type, bool boolean, double number, String *string, void *object, void *extra)
{
    // create
    Value *value = malloc(sizeof(Value));
    value->type = type;
    value->boolean = boolean;
    value->number = number;
    value->string = string;
    value->object = object;
    value->extra = extra;
    // return
    Object_init(value, PCT_OBJ_VALUE);
    return value;
}

Value *Value_newEmpty(void *extra)
{
    return Value_new(RTYPE_EMPTY, NULL, 0, NULL, NULL, extra);
}

Value *Value_newContainer(Container *box, void *extra)
{
    return Value_new(RTYPE_CONTAINER, NULL, 0, NULL, box, extra);
}

Value *Value_newBoolean(bool boolean, void *extra)
{
    return Value_new(RTYPE_BOOLEAN, boolean, 0, NULL, NULL, extra);
}

Value *Value_newNumber(double number, void *extra)
{
    return Value_new(RTYPE_NUMBER, NULL, number, NULL, NULL, extra);
}

Value *Value_newString(String *string, void *extra)
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
        char *value = String_get(this->string);
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
        return String_dump(this->string);
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
        return Value_newBoolean(is_equal(String_get(this->string), TVALUE_TRUE), NULL);
    }
    else if (is_equal(this->type, RTYPE_EMPTY))
    {
        return Value_newBoolean(false, NULL);
    }
    else if (is_equal(this->type, RTYPE_BOOLEAN))
    {
        Object_retain(this);
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
        Object_retain(this);
        return this;
    }
    else if (is_equal(this->type, RTYPE_STRING))
    {
        return Value_newNumber(atof(String_get(this->string)), NULL);
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

void Value_free(Value *this)
{
    // char *tag = _get_cache_tag(this->type, this->boolean, this->number, String_get(this->string));
    // // printf("%s\n", tag);
    // if (tag != NULL)
    // {
    //     free(tag);
    // }
    
    if (is_equal(this->type, RTYPE_STRING))
    {
        Object_release(this->string);
    }
    if (is_equal(this->type, RTYPE_CONTAINER))
    {
        Object_release(this->object);
    }
    Object_free(this);
}

#endif