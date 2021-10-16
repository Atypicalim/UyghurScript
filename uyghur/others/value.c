// value

#ifndef H_VALUE
#define H_VALUE

#include "header.h"

typedef struct ValueNode {
    char *type;
    void *data;
    void *extra;
} Value;

Value *Value_new(char *type, void *data, void *extra)
{
    Value *value = malloc(sizeof(Value));
    value->type = type;
    value->data = data;
    value->extra = extra;
    return value;
}

void Value_print(Value *this)
{
    if (is_equal(this->type, RTYPE_STRING))
    {
        char *value = this->data;
        printf("[RV => t:%s v:%s]\n", this->type, value);
    }
    else if (is_equal(this->type, RTYPE_NUMBER))
    {
        double *value = this->data;
        printf("[RV => t:%s v:%f]\n", this->type, *value);
    }
    else
    {
        char *value = this->data;
        printf("[RV => t:%s]\n", this->type);
    }
}

char *Value_string(Value *this)
{
    if (is_equal(this->type, RTYPE_STRING))
    {
        char *value = this->data;
        return value;
    }
    if (is_equal(this->type, RTYPE_NUMBER))
    {
        double *value = this->data;
        return tools_format("%f", *value);
    }
    else
    {
        char *value = this->data;
        return tools_format("[RV => t:%s]\n", this->type);
    } 
}

void Value_free(Value *this)
{
    free(this);
}

#endif
