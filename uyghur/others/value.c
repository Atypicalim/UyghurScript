// value

#ifndef H_VALUE
#define H_VALUE

#include "header.h"

typedef struct ValueNode {
    char *type;
    bool boolean;
    float number;
    char *string;
    void *object;
    void *extra;
} Value;

 Value *Value_new(char *type, bool boolean, float number, char *string, void *object, void *extra)
{
    Value *value = malloc(sizeof(Value));
    value->type = type;
    value->boolean = boolean;
    value->number = number;
    value->string = string;
    value->object = object;
    value->extra = extra;
    return value;
}

Value *Value_newEmpty(void *extra)
{
    return Value_new(RTYPE_EMPTY, NULL, 0, NULL, NULL, extra);
}

Value *Value_newBoolean(bool boolean, void *extra)
{
    return Value_new(RTYPE_BOOLEAN, boolean, 0, NULL, NULL, extra);
}

Value *Value_newNumber(float number, void *extra)
{
    return Value_new(RTYPE_NUMBER, NULL, number, NULL, NULL, extra);
}

Value *Value_newString(char *string, void *extra)
{
    return Value_new(RTYPE_STRING, NULL, 0, string, NULL, extra);
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
        char *value = this->object;
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
        char *value = this->object;
        return tools_format("[RV => t:%s]\n", this->type);
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

void Value_free(Value *this)
{
    free(this);
}

#endif
