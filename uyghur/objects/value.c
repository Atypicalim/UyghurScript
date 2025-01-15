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

Value *_value_newValue(bool freeze, char typ) {
    Machine *machine = __uyghur->machine;
    if (freeze || machine->freezing) {
        return Machine_newNormalValue(freeze, typ);
    } else {
        Value *value = Machine_newCacheableValue(typ, true);
        return value;
    }
}

Value *Value_newEmpty(void *extra)
{
    if (Value_EMPTY == NULL) {
        Value_EMPTY = _value_newValue(true, UG_TYPE_NIL);
    }
    return Value_EMPTY;
}

Value *Value_getTemporary() {
    if (Value_TEMPORARY == NULL) {
        Value_TEMPORARY = _value_newValue(true, UG_TYPE_NIL);
    }
    return Value_TEMPORARY;
}

Value *Value_newBoolean(bool boolean, void *extra)
{
    if (Value_TRUE == NULL) {
        Value_TRUE = _value_newValue(true, UG_TYPE_BOL);
        Value_TRUE->boolean = true;
    }
    if (Value_FALSE == NULL) {
        Value_FALSE = _value_newValue(true, UG_TYPE_BOL);
        Value_FALSE->boolean = false;
    }
    return boolean ? Value_TRUE : Value_FALSE;
}

Value *Value_newNumber(double number, void *extra)
{
    Value *value = _value_newValue(false, UG_TYPE_NUM);
    value->number = number;
    value->extra = extra;
    return value;
}

Value *Value_newString(CString string, void *extra)
{
    Value *value = _value_newValue(false, UG_TYPE_STR);
    value->string = strdup(string);
    value->extra = extra;
    return value;
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

bool Value_isListable(Value *this)
{
    return this != NULL && is_type_listable(this->type);
}

bool Value_isDictable(Value *this)
{
    return this != NULL && is_type_dictable(this->type);
}

bool Value_isHoldable(Value *this)
{
    return this != NULL && is_type_holdable(this->type);
}

bool Value_isObjective(Value *this)
{
    return this != NULL && is_type_objective(this->type);
}

bool Value_isRunnable(Value *this)
{
    return this != NULL && is_type_runnable(this->type);
}

bool Value_isLoadable(Value *this)
{
    return this != NULL && is_type_loadable(this->type);
}

bool Value_isWaitable(Value *this)
{
    return this != NULL && is_type_waitable(this->type);
}

bool Value_isSimple(Value *this)
{
    return this != NULL && is_type_simple(this->type);
}

bool Value_isComplex(Value *this)
{
    return this != NULL && is_type_complex(this->type);
}

void Value_print(Value *this)
{
    if (this == NULL) {
        printf("<V:NULL>\n");
    }
    else if (this->type == UG_TYPE_NIL)
    {
        printf("<V:Empty p:%p>\n", LETTER_NIL, this);
    }
    else if (this->type == UG_TYPE_BOL)
    {
        bool value = this->boolean;
        printf("<V:Boolean => v:%s p:%p>\n", value ? LETTER_TRUE : LETTER_FALSE, this);
    }
    else if (this->type == UG_TYPE_NUM)
    {
        double value = this->number;
        printf("<V:Number => v:%f p:%p>\n", value, this);
    }
    else if (this->type == UG_TYPE_STR)
    {
        char *value = this->string;
        printf("<V:String => v:%s p:%p>\n", value, this);
    }
    else if (is_type_listable(this->type))
    {
        Listable_print(this);
    }
    else if (is_type_dictable(this->type))
    {
        Dictable_print(this);
    }
    else if (is_type_holdable(this->type))
    {
        Holdable_print(this);
    }
    else if (is_type_objective(this->type))
    {
        Objective_print(this);
    }
    else if (is_type_runnable(this->type))
    {
        Runnable_print(this);
    }
    else if (is_type_loadable(this->type))
    {
        Loadable_print(this);
    }
    else if (is_type_waitable(this->type))
    {
        Waitable_print(this);
    }
    else
    {
        printf("<Value => t:%c p:%p>\n", this->type, this);
    }
}

// TODO return Value
char *Value_toString(Value *this)
{
    if (this == NULL || this->type == UG_TYPE_NIL) {
        return tools_format("%s", LETTER_NIL);
    } if (this->type == UG_TYPE_BOL) {
        return tools_boolean_to_string(this->boolean);
    } if (this->type == UG_TYPE_NUM) {
        return tools_number_to_string(this->number);
    } if (this->type == UG_TYPE_STR) {
        return strdup(this->string);
    } if (is_type_listable(this->type)) {
        return Listable_toString(this);
    } if (is_type_dictable(this->type)) {
        return Dictable_toString(this);
    } if (is_type_holdable(this->type)) {
        return Holdable_toString(this);
    } if (is_type_objective(this->type)) {
        return Objective_toString(this);
    } if (is_type_runnable(this->type)) {
        return Runnable_toString(this);
    } if (is_type_loadable(this->type)) {
        return Loadable_toString(this);
    } if (is_type_waitable(this->type)) {
        return Waitable_toString(this);
    } else {
        return tools_format("<Object %p p:%p t:%c>", this, this->obj, this->type);
    } 
}

Value *Value_toBoolean(Value *this)
{
    if (this == NULL || this->type == UG_TYPE_NIL) {
        return Value_newBoolean(false, NULL);
    } else if (this->type == UG_TYPE_NUM) {
        return Value_newBoolean(this->number > 0, NULL);
    } else if (this->type == UG_TYPE_STR) {
        return Value_newBoolean(is_eq_string(this->string, LETTER_TRUE), NULL);
    } else if (this->type == UG_TYPE_BOL) {
        Machine_retainObj(this);
        return this;
    } else {
        return Value_newBoolean(true, NULL);
    }
}

Value *Value_toNumber(Value *this)
{

    if (this->type == UG_TYPE_NUM)
    {
        Machine_retainObj(this);
        return this;
    }
    else if (this->type == UG_TYPE_STR)
    {
        return Value_newNumber(tools_string_to_number(this->string), NULL);
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
        return strcmp(this->string, other->string);
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

void Value_free(Value *this)
{
    // char *tag = _get_cache_tag(this->type, this->boolean, this->number, this->string);
    // // printf("%s\n", tag);
    // if (tag != NULL)
    // {
    //     free(tag);
    // }
    if (this->type == UG_TYPE_STR) {
        pct_free(this->string);
    } else if (is_type_listable(this->type)) {
        Machine_releaseObj(this->map);
    } else if (is_type_dictable(this->type)) {
        Machine_releaseObj(this->map);
    } else if (is_type_holdable(this->type)) {
        Machine_releaseObj(this->map);
    } else if (is_type_objective(this->type)) {
        Machine_releaseObj(this->map);
    }
    if (this != Value_EMPTY && this != Value_TRUE && this != Value_FALSE) {
        Machine_freeObj(this);
    }
}

//

#endif
