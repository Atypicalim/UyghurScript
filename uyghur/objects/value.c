// value

#ifndef H_UG_HEADER
#include "../others/header.h"
#endif

#ifndef H_UG_VALUE
#define H_UG_VALUE

char *_get_cache_tag(char type, bool boolean, double number, char *string)
{
    if (type == UG_TYPE_NIL) return tools_format("<R-VALUE:%c>", type);
    if (type == UG_TYPE_BOL) return tools_format("<R-VALUE:%c:%s>", type, b2s(boolean));
    if (type == UG_TYPE_NUM && round(number) == number)
    {
        return tools_format("<R-VALUE:%c:%f>", type, number);
    }
    if (type == UG_TYPE_STR && strlen(string) <= CACHE_STRING_MAX_LENGTH)
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
    if (Value_EMPTY == NULL) Value_EMPTY = Value_new(UG_TYPE_NIL, NULL, 0, NULL, NULL, NULL);
    return Value_EMPTY;
}

Value *Value_newBoolean(bool boolean, void *extra)
{
    if (Value_TRUE == NULL) Value_TRUE = Value_new(UG_TYPE_BOL, true, 0, NULL, NULL, NULL);
    if (Value_FALSE == NULL) Value_FALSE = Value_new(UG_TYPE_BOL, false, 0, NULL, NULL, NULL);
    return boolean ? Value_TRUE : Value_FALSE;
}

Value *Value_newNumber(double number, void *extra)
{
    return Value_new(UG_TYPE_NUM, NULL, number, NULL, NULL, extra);
}

Value *Value_newString(String *string, void *extra)
{
    // Object_retain(string);
    return Value_new(UG_TYPE_STR, NULL, 0, string, NULL, extra);
}

Value *Value_newContainer(Container *container, void *extra)
{
    // Object_retain(container);
    return Value_new(UG_TYPE_CNT, NULL, 0, NULL, container, extra);
}

Value *Value_newWorker(void *function, void *extra)
{
    return Value_new(UG_TYPE_WKR ,NULL, 0, NULL, function, extra);
}

Value *Value_newCreator(void *function, void *extra)
{
    return Value_new(UG_TYPE_CTR ,NULL, 0, NULL, function, extra);
}

Value *Value_newNative(void *function, void *extra)
{
    return Value_new(UG_TYPE_NTV ,NULL, 0, NULL, function, extra);
}

bool Value_isEmpty(Value *this)
{
    return this != NULL && this->type == UG_TYPE_NIL;
}

bool Value_isBoolean(Value *this)
{
    return this != NULL && this->type == UG_TYPE_BOL;
}

bool Value_isNumber(Value *this)
{
    return this != NULL && this->type == UG_TYPE_NUM;
}

bool Value_isInt(Value *this)
{
    return Value_isNumber(this) && tools_number_is_integer(this->number);
}

bool Value_isFlt(Value *this)
{
    return Value_isNumber(this) && !tools_number_is_integer(this->number);
}

bool Value_isString(Value *this)
{
    return this != NULL && this->type == UG_TYPE_STR;
}

bool Value_isContainer(Value *this)
{
    return this != NULL && this->type == UG_TYPE_CNT;
}

bool Value_isWorker(Value *this)
{
    return this != NULL && this->type == UG_TYPE_WKR;
}

bool Value_isCreator(Value *this)
{
    return this != NULL && this->type == UG_TYPE_CTR;
}

bool Value_isNative(Value *this)
{
    return this != NULL && this->type == UG_TYPE_NTV;
}

bool Value_isRunnable(Value *this)
{
    return this != NULL && (this->type == UG_TYPE_WKR || this->type == UG_TYPE_CTR || this->type == UG_TYPE_NTV);
}

void Value_print(Value *this)
{
    if (this == NULL) {
        printf("<NULL>\n");
    }
    else if (this->type == UG_TYPE_NIL)
    {
        printf("<Empty>\n", TVALUE_EMPTY);
    }
    else if (this->type == UG_TYPE_BOL)
    {
        bool value = this->boolean;
        printf("<Boolean => v:%s>\n", value ? TVALUE_TRUE : TVALUE_FALSE);
    }
    else if (this->type == UG_TYPE_NUM)
    {
        double value = this->number;
        printf("<Number => v:%f>\n", value);
    }
    else if (this->type == UG_TYPE_STR)
    {
        char *value = String_get(this->string);
        printf("<String => v:%s>\n", value);
    }
    else if (this->type == UG_TYPE_CNT)
    {
        printf("<Container => p:%p>\n", this->object);
    }
    else if (this->type == UG_TYPE_NTV)
    {
        printf("<Native => p:%p>\n", this->object);
    }
    else
    {
        printf("<Value => t:%c>\n", this->type);
    }
}

// TODO return Value
char *Value_toString(Value *this)
{
    if (this == NULL || this->type == UG_TYPE_NIL) {
        return tools_format("%s", TVALUE_EMPTY);
    } if (this->type == UG_TYPE_BOL) {
        return tools_boolean_to_string(this->boolean);
    } if (this->type == UG_TYPE_NUM) {
        return tools_number_to_string(this->number);
    } if (this->type == UG_TYPE_STR) {
        return String_dump(this->string);
    } if (this->type == UG_TYPE_CNT) {
        return Container_toString(this->object);
    } if (this->type == UG_TYPE_WKR) {
        return tools_format("<Worker p:%p>",  this->object);
    } if (this->type == UG_TYPE_CTR) {
        return tools_format("<Creator p:%p>",  this->object);
    } if (this->type == UG_TYPE_NTV) {
        return tools_format("<Native p:%p>",  this->object);
    } else {
        return tools_format("<Object p:%p t:%c>", this->object, this->type);
    } 
}

Value *Value_toBoolean(Value *this)
{
    if (this == NULL || this->type == UG_TYPE_NIL) {
        return Value_newBoolean(false, NULL);
    } else if (this->type == UG_TYPE_NUM) {
        return Value_newBoolean(this->number > 0, NULL);
    } else if (this->type == UG_TYPE_STR) {
        return Value_newBoolean(is_eq_string(String_get(this->string), TVALUE_TRUE), NULL);
    } else if (this->type == UG_TYPE_BOL) {
        Object_retain(this);
        return this;
    } else {
        return Value_newBoolean(true, NULL);
    }
}

Value *Value_toNumber(Value *this)
{

    if (this->type == UG_TYPE_NUM)
    {
        Object_retain(this);
        return this;
    }
    else if (this->type == UG_TYPE_STR)
    {
        return Value_newNumber(atof(String_get(this->string)), NULL);
    }
    else if (this->type == UG_TYPE_NIL)
    {
        return Value_newNumber(0, NULL);
    }
    else if (this->type == UG_TYPE_BOL)
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
    if (this->type == UG_TYPE_NUM) {
        if (this->number > other->number) return CODE_TRUE;
        if (this->number < other->number) return CODE_FALSE;
    } else if (this->type == UG_TYPE_STR) {
        return String_compare(this->string, other->string);
    } else if (this->type == UG_TYPE_BOL) {
        if (this->boolean && !other->boolean) return CODE_TRUE;
        if (!this->boolean && other->boolean) return CODE_FALSE;
    }
    return CODE_NONE;
}

bool Value_isTrue(Value *this)
{
    if (this == NULL || this->type == UG_TYPE_NIL) {
        return false;
    } else if (this->type == UG_TYPE_BOL) {
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
    if (this->type == UG_TYPE_STR) {
        Object_release(this->string);
    } else if (this->type == UG_TYPE_CNT) {
        Object_release(this->object);
    }
    if (this != Value_EMPTY && this != Value_TRUE && this != Value_FALSE) {
        Object_free(this);
    }
}

#endif
