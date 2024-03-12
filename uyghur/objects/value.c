// value

#ifndef H_UG_HEADER
#include "../others/header.h"
#endif

#ifndef H_UG_VALUE
#define H_UG_VALUE

Hashmap *valueCache = NULL;

typedef struct ValueNode {
    struct _Object;
    char type;
    bool boolean;
    char character;
    double number;
    String *string;
    void *object;
    void *extra;
} Value;

Value *Value_EMPTY;
Value *Value_TRUE;
Value *Value_FALSE;

char *_get_cache_tag(char type, bool boolean, double number, char *string)
{
    if (type == UG_RTYPE_NIL) return tools_format("<R-VALUE:%c>", type);
    if (type == UG_RTYPE_BOL) return tools_format("<R-VALUE:%c:%s>", type, b2s(boolean));
    if (type == UG_RTYPE_NUM && round(number) == number)
    {
        return tools_format("<R-VALUE:%c:%f>", type, number);
    }
    if (type == UG_RTYPE_STR && strlen(string) <= CACHE_STRING_MAX_LENGTH)
    {
        return tools_format("<R-VALUE:%c:%s>", type, string);
    }
    return NULL;
}

 Value *Value_new(char type, bool boolean, double number, String *string, void *object, void *extra)
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
    if (Value_EMPTY == NULL) Value_EMPTY = Value_new(UG_RTYPE_NIL, NULL, 0, NULL, NULL, NULL);
    return Value_EMPTY;
}

Value *Value_newBoolean(bool boolean, void *extra)
{
    if (Value_TRUE == NULL) Value_TRUE = Value_new(UG_RTYPE_BOL, true, 0, NULL, NULL, NULL);
    if (Value_FALSE == NULL) Value_FALSE = Value_new(UG_RTYPE_BOL, false, 0, NULL, NULL, NULL);
    return boolean ? Value_TRUE : Value_FALSE;
}

Value *Value_newNumber(double number, void *extra)
{
    return Value_new(UG_RTYPE_NUM, NULL, number, NULL, NULL, extra);
}

Value *Value_newString(String *string, void *extra)
{
    return Value_new(UG_RTYPE_STR, NULL, 0, string, NULL, extra);
}

Value *Value_newContainer(Container *box, void *extra)
{
    return Value_new(UG_RTYPE_CNT, NULL, 0, NULL, box, extra);
}

Value *Value_newFunction(void *function, void *extra)
{
    return Value_new(UG_RTYPE_FUN ,NULL, 0, NULL, function, extra);
}

Value *Value_newNative(void *function, void *extra)
{
    return Value_new(UG_RTYPE_NTV ,NULL, 0, NULL, function, extra);
}

bool Value_isNumber(Value *this)
{
    return this != NULL && this->type == UG_RTYPE_NUM;
}

bool Value_isInt(Value *this)
{
    double intpart;
    return Value_isNumber(this) && modf(this->number, &intpart) == 0.0;
}

bool Value_isFlt(Value *this)
{
    double intpart;
    return Value_isNumber(this) && modf(this->number, &intpart) != 0.0;
}

bool Value_isFunc(Value *this)
{
    return this != NULL && this->type == UG_RTYPE_FUN;
}

bool Value_isNative(Value *this)
{
    return this != NULL && this->type == UG_RTYPE_NTV;
}

bool Value_isRunnable(Value *this)
{
    return this != NULL && (this->type == UG_RTYPE_FUN || this->type == UG_RTYPE_NTV);
}

void Value_print(Value *this)
{
    if (this->type == UG_RTYPE_NIL)
    {
        printf("[RV => t:%c v:%s]\n", this->type, TVALUE_EMPTY);
    }
    else if (this->type == UG_RTYPE_BOL)
    {
        bool value = this->boolean;
        printf("[RV => t:%c v:%s]\n", this->type, value ? TVALUE_TRUE : TVALUE_FALSE);
    }
    else if (this->type == UG_RTYPE_NUM)
    {
        double value = this->number;
        printf("[RV => t:%c v:%f]\n", this->type, value);
    }
    else if (this->type == UG_RTYPE_STR)
    {
        char *value = String_get(this->string);
        printf("[RV => t:%c v:%s]\n", this->type, value);
    }
    else
    {
        printf("[RV => t:%c]\n", this->type);
    }
}

// TODO return Value
char *Value_toString(Value *this)
{
    if (this == NULL || this->type == UG_RTYPE_NIL) {
        return tools_format("%s", TVALUE_EMPTY);
    } if (this->type == UG_RTYPE_BOL) {
        double value = this->boolean;
        return tools_format("%s", value ? TVALUE_TRUE : TVALUE_FALSE);
    } if (this->type == UG_RTYPE_NUM) {
        double value = this->number;
        return Value_isInt(this) ? tools_format("%i", (int)value) : tools_format("%.15g", (double)value);
    } if (this->type == UG_RTYPE_STR) {
        return String_dump(this->string);
    } if (this->type == UG_RTYPE_FUN) {
        return tools_format("<Function p:%d>\n",  this->object);
    } if (this->type == UG_RTYPE_NTV) {
        return tools_format("<Native p:%d>\n",  this->object);
    } else {
        void *value = this->object;
        return tools_format("<Object t:%c p:%d>\n", this->type, value != NULL ? value : 0);
    } 
}

Value *Value_toBoolean(Value *this)
{
    if (this == NULL || this->type == UG_RTYPE_NIL) {
        return Value_newBoolean(false, NULL);
    } else if (this->type == UG_RTYPE_NUM) {
        return Value_newBoolean(this->number > 0, NULL);
    } else if (this->type == UG_RTYPE_STR) {
        return Value_newBoolean(is_equal(String_get(this->string), TVALUE_TRUE), NULL);
    } else if (this->type == UG_RTYPE_BOL) {
        Object_retain(this);
        return this;
    } else {
        return Value_newBoolean(true, NULL);
    }
}

Value *Value_toNumber(Value *this)
{

    if (this->type == UG_RTYPE_NUM)
    {
        Object_retain(this);
        return this;
    }
    else if (this->type == UG_RTYPE_STR)
    {
        return Value_newNumber(atof(String_get(this->string)), NULL);
    }
    else if (this->type == UG_RTYPE_NIL)
    {
        return Value_newNumber(0, NULL);
    }
    else if (this->type == UG_RTYPE_BOL)
    {
        return Value_newNumber(this->boolean ? 1 : 0, NULL);
    }
    else
    {
        return Value_newNumber(0, NULL);
    }
}

int Value_compareTo(Value *this, Value *other)
{
    if (!other || this->type != other->type) {
         return CODE_FAIL;
    }
    if (this->type == UG_RTYPE_NUM) {
        if (this->number > other->number) return CODE_TRUE;
        if (this->number < other->number) return CODE_FALSE;
    } else if (this->type == UG_RTYPE_STR) {
        return String_compare(this->string, other->string);
    } else if (this->type == UG_RTYPE_BOL) {
        if (this->boolean && !other->boolean) return CODE_TRUE;
        if (!this->boolean && other->boolean) return CODE_FALSE;
    }
    return CODE_NONE;
}

bool Value_isTrue(Value *this)
{
    if (this == NULL || this->type == UG_RTYPE_NIL) {
        return false;
    } else if (this->type == UG_RTYPE_BOL) {
        return this->boolean;
    } else {
        return true;
    }
}

void Value_release(Value *this)
{
    Object_release(this);
}

void Value_free(Value *this)
{
    // char *tag = _get_cache_tag(this->type, this->boolean, this->number, String_get(this->string));
    // // printf("%s\n", tag);
    // if (tag != NULL)
    // {
    //     free(tag);
    // }
    if (this->type == UG_RTYPE_STR) {
        Object_release(this->string);
    } else if (this->type == UG_RTYPE_CNT) {
        Object_release(this->object);
    }
    if (this != Value_EMPTY && this != Value_TRUE && this != Value_FALSE) {
        Object_free(this);
    }
}

#endif
