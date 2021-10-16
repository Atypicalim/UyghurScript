// value

#ifndef H_VALUE
#define H_VALUE

#include "header.h"

typedef struct ValueNode {
    char *type;
    float number;
    char *string;
    void *object;
    void *extra;
} Value;

 Value *Value_new(char *type, float number, char *string, void *object, void *extra)
{
    Value *value = malloc(sizeof(Value));
    value->type = type;
    value->number = number;
    value->string = string;
    value->object = object;
    value->extra = extra;
    return value;
}

Value *Value_newString(char *string, void *extra)
{
    return Value_new(RTYPE_STRING, 0, string, NULL, extra);
}

Value *Value_newNumber(float number, void *extra)
{
    return Value_new(RTYPE_NUMBER, number, "", NULL, extra);
}

Value *Value_newObject(void *object, void *extra)
{
    return Value_new(RTYPE_UNKNOWN, 0, "", object, extra);
}


void Value_print(Value *this)
{
    if (is_equal(this->type, RTYPE_STRING))
    {
        char *value = this->string;
        printf("[RV => t:%s v:%s]\n", this->type, value);
    }
    else if (is_equal(this->type, RTYPE_NUMBER))
    {
        double value = this->number;
        printf("[RV => t:%s v:%f]\n", this->type, value);
    }
    else
    {
        char *value = this->object;
        printf("[RV => t:%s]\n", this->type);
    }
}

char *Value_string(Value *this)
{
    if (is_equal(this->type, RTYPE_STRING))
    {
        char *value = this->string;
        return value;
    }
    if (is_equal(this->type, RTYPE_NUMBER))
    {
        double value = this->number;
        return tools_format("%f", value);
    }
    else
    {
        char *value = this->object;
        return tools_format("[RV => t:%s]\n", this->type);
    } 
}

void Value_free(Value *this)
{
    free(this);
}

#endif
