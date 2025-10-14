// string

#include "../uyghur.c"

void native_string_replace(Bridge *bridge)
{
    CString origin = Bridge_receiveString(bridge);
    CString fromText = Bridge_receiveString(bridge);
    CString toText = Bridge_receiveString(bridge);
    double fromIndex = Bridge_receiveNumber(bridge);
    double toIndex = Bridge_receiveNumber(bridge);
    double replaceCount = Bridge_receiveNumber(bridge);
    //
    String *tmp = String_set(TEMPORARY_String, strdup(origin));
    String_replace(tmp, fromText, toText, fromIndex, toIndex, replaceCount);
    //
    CString result = String_get(tmp);
    Bridge_returnString(bridge, result);
}

void native_string_replace_first(Bridge *bridge)
{
    CString origin = Bridge_receiveString(bridge);
    CString fromText = Bridge_receiveString(bridge);
    CString toText = Bridge_receiveString(bridge);
    //
    String *tmp = String_set(TEMPORARY_String, strdup(origin));
    int index = String_findNext(tmp, 0, fromText);
    String_replace(tmp, fromText, toText, 0, strlen(origin), 1);
    //
    CString result = String_get(tmp);
    Bridge_returnString(bridge, result);
}

void native_string_replace_last(Bridge *bridge)
{
    CString origin = Bridge_receiveString(bridge);
    CString fromText = Bridge_receiveString(bridge);
    CString toText = Bridge_receiveString(bridge);
    //
    String *tmp = String_set(TEMPORARY_String, strdup(origin));
    int index = String_findLast(tmp, -1, fromText);
    String_replace(tmp, fromText, toText, index, strlen(origin), 1);
    //
    CString result = String_get(tmp);
    Bridge_returnString(bridge, result);

}

void native_string_replace_all(Bridge *bridge)
{
    CString origin = Bridge_receiveString(bridge);
    CString fromText = Bridge_receiveString(bridge);
    CString toText = Bridge_receiveString(bridge);
    //
    String *tmp = String_set(TEMPORARY_String, strdup(origin));
    String_replace(tmp, fromText, toText, 0, strlen(origin), INT_MAX);
    //
    CString result = String_get(tmp);
    Bridge_returnString(bridge, result);
}

void native_string_find(Bridge *bridge)
{
    CString origin = Bridge_receiveString(bridge);
    CString find =Bridge_receiveString(bridge);
    double fromIndex = Bridge_receiveNumber(bridge);
    double toIndex = Bridge_receiveNumber(bridge);
    double index = Bridge_receiveNumber(bridge);
    String *tmp = String_set(TEMPORARY_String, origin);
    int *found = String_findAll(tmp, find);
    int result = -1;
    int count = 0;
    for (size_t i = fromIndex; i < toIndex; i++) {
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
    CString origin = Bridge_receiveString(bridge);
    CString find =Bridge_receiveString(bridge);
    String *tmp = String_set(TEMPORARY_String, origin);
    int result = String_findNext(tmp, 0, find);
    Bridge_returnNumber(bridge, result);
}

void native_string_find_last(Bridge *bridge)
{
    CString origin = Bridge_receiveString(bridge);
    CString find =Bridge_receiveString(bridge);
    String *tmp = String_set(TEMPORARY_String, origin);
    int result = String_findLast(tmp, -1, find);
    Bridge_returnNumber(bridge, result);
}

void native_string_cut(Bridge *bridge)
{
    CString origin = Bridge_receiveString(bridge);
    double fromIndex = Bridge_receiveNumber(bridge);
    double toIndex = Bridge_receiveNumber(bridge);
    String *tmp = String_set(TEMPORARY_String, origin);
    String *new = String_subString(tmp, fromIndex, toIndex);
    CString result = String_get(new);
    Bridge_returnString(bridge, result);
    Object_release(new);
}

void native_string_count(Bridge *bridge)
{
    CString origin = Bridge_receiveString(bridge);
    String *tmp = String_set(TEMPORARY_String, origin);
    int result = String_length(tmp);
    Bridge_returnNumber(bridge, result);
}

void native_string_link(Bridge *bridge)
{
    CString origin = Bridge_receiveString(bridge);
    CString other = Bridge_receiveString(bridge);
    String *tmp = String_set(TEMPORARY_String, origin);
    String_appendStr(tmp, other);
    CString result = String_get(tmp);
    Bridge_returnString(bridge, result);
}

void native_string_format(Bridge *bridge)
{
    CString format = Bridge_receiveString(bridge);
    Value *value = Bridge_receiveValue(bridge, UG_TYPE_NON);
    Value *t = Bridge_nextValue(bridge);
    tools_assert(t == NULL, "too many arguments for string format");
    CString r = NULL;
    if (value->type == UG_TYPE_NUM) r = tools_format(format, value->number);
    if (value->type == UG_TYPE_STR) r = tools_format(format, value->string);
    if (value->type == UG_TYPE_BOL) r = tools_format(format, value->boolean);
    if (r == NULL) r = tools_format(format, '?');
    Bridge_returnString(bridge, r);
    pct_free(r);
}

void native_string_fill(Bridge *bridge)
{
    CString format = Bridge_receiveString(bridge);
    String *tmp = String_set(TEMPORARY_String, format);
    // 
    Value *value = Bridge_receiveValue(bridge, UG_TYPE_NON);
    int index = 1;
    CString target = tools_format("{%i}", index);
    while (value != NULL)
    {
        CString arg = Value_toString(value);
        int len = String_length(tmp);
        String_replace(tmp, target, arg, 0, len - 1, INT_MAX);
        pct_free(arg);
        pct_free(target);
        value = Bridge_nextValue(bridge);
        index = index + 1;
        target = tools_format("{%i}", index);
    }
    pct_free(target);
    // 
    CString result = String_get(tmp);
    Bridge_returnString(bridge, result);
}
