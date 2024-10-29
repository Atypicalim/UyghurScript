// string

#include "../uyghur.c"

void native_string_replace(Bridge *bridge)
{
    USTRING origin = Bridge_receiveString(bridge);
    USTRING from =Bridge_receiveString(bridge);
    USTRING to =Bridge_receiveString(bridge);
    double direction = Bridge_receiveNumber(bridge);
    double num = Bridge_receiveNumber(bridge);
    USTRING result = NULL; // TODO: replace(origin, from, to, direction, num);
    Bridge_returnString(bridge, result);
}

void native_string_replace_first(Bridge *bridge)
{
    USTRING origin = Bridge_receiveString(bridge);
    USTRING from =Bridge_receiveString(bridge);
    USTRING to =Bridge_receiveString(bridge);
    USTRING result = NULL; // TODO: replace(origin, from, to, 1, 1);
    Bridge_returnString(bridge, result);
}

void native_string_replace_last(Bridge *bridge)
{
    USTRING origin = Bridge_receiveString(bridge);
    USTRING from =Bridge_receiveString(bridge);
    USTRING to =Bridge_receiveString(bridge);
    USTRING result = NULL; // TODO: replace(origin, from, to, -1, 1);
    Bridge_returnString(bridge, result);
}

void native_string_replace_all(Bridge *bridge)
{
    USTRING origin = Bridge_receiveString(bridge);
    USTRING from =Bridge_receiveString(bridge);
    USTRING to =Bridge_receiveString(bridge);
    String *tmp = String_set(TEMPORARY_String, origin);
    String *new = String_replace(tmp, from, to, 0, strlen(origin), INT_MAX);
    USTRING result = String_get(new);
    Bridge_returnString(bridge, result);
}

void native_string_find(Bridge *bridge)
{
    USTRING origin = Bridge_receiveString(bridge);
    USTRING find =Bridge_receiveString(bridge);
    double from = Bridge_receiveNumber(bridge);
    double to = Bridge_receiveNumber(bridge);
    double index = Bridge_receiveNumber(bridge);
    String *tmp = String_set(TEMPORARY_String, origin);
    int *found = String_findAll(tmp, find);
    int result = -1;
    int count = 0;
    for (size_t i = from; i < to; i++) {
        if (found[i] >= 0) {
            count++;
        }
        if (count == index) {
            result = found[i];
            break;
        }
    }
    Bridge_returnNumber(bridge, result);
}

void native_string_find_first(Bridge *bridge)
{
    USTRING origin = Bridge_receiveString(bridge);
    USTRING find =Bridge_receiveString(bridge);
    String *tmp = String_set(TEMPORARY_String, origin);
    int result = String_findNext(tmp, 0, find);
    Bridge_returnNumber(bridge, result);
}

void native_string_find_last(Bridge *bridge)
{
    USTRING origin = Bridge_receiveString(bridge);
    USTRING find =Bridge_receiveString(bridge);
    String *tmp = String_set(TEMPORARY_String, origin);
    int result = String_findLast(tmp, strlen(origin), find);
    Bridge_returnNumber(bridge, result);
}

void native_string_cut(Bridge *bridge)
{
    USTRING origin = Bridge_receiveString(bridge);
    double from = Bridge_receiveNumber(bridge);
    double to = Bridge_receiveNumber(bridge);
    String *tmp = String_set(TEMPORARY_String, origin);
    String *new = String_subString(tmp, from, to);
    USTRING result = String_get(new);
    Bridge_returnString(bridge, result);
    Object_release(new);
}

void native_string_count(Bridge *bridge)
{
    USTRING origin = Bridge_receiveString(bridge);
    int result = strlen(origin);
    Bridge_returnNumber(bridge, result);
}

void native_string_link(Bridge *bridge)
{
    USTRING origin = Bridge_receiveString(bridge);
    USTRING other = Bridge_receiveString(bridge);
    String *tmp = String_set(TEMPORARY_String, origin);
    String_appendStr(tmp, other);
    USTRING result = String_get(tmp);
    Bridge_returnString(bridge, result);
}

void native_string_format(Bridge *bridge)
{
    USTRING f = Bridge_receiveString(bridge);
    Value *v = Bridge_receiveValue(bridge, UG_TYPE_NON);
    Value *t = Bridge_nextValue(bridge);
    tools_assert(t == NULL, "too many arguments for string format");
    CString r = NULL;
    if (v->type == UG_TYPE_NUM) r = tools_format(f, v->number);
    if (v->type == UG_TYPE_STR) r = tools_format(f, v->string);
    if (v->type == UG_TYPE_BOL) r = tools_format(f, v->boolean);
    if (r == NULL) r = tools_format(f, v->obj);
    Bridge_returnString(bridge, r);
    pct_free(r);
}

void native_string_fill(Bridge *bridge)
{
    USTRING f = Bridge_receiveString(bridge);
    String *tmp = String_set(TEMPORARY_String, f);
    // 
    Value *v = Bridge_nextValue(bridge);
    int index = 1;
    USTRING target = tools_format("{%i}", index);
    while (v != NULL)
    {
        USTRING arg = Value_toString(v);
        int len = String_length(tmp);
        String_replace(tmp, target, arg, 0, len - 1, INT_MAX);
        pct_free(arg);
        pct_free(target);
        v = Bridge_nextValue(bridge);
        index = index + 1;
        target = tools_format("{%i}", index);
    }
    pct_free(target);
    // 
    USTRING result = String_get(tmp);
    Bridge_returnString(bridge, result);
}
