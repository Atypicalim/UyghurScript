// string

#include "../uyghur/uyghur.c"

void ug_string_replace(Bridge *bridge)
{
    char *origin = Bridge_receiveString(bridge);
    char *from =Bridge_receiveString(bridge);
    char *to =Bridge_receiveString(bridge);
    double direction = Bridge_receiveNumber(bridge);
    double num = Bridge_receiveNumber(bridge);
    char *result = NULL; // TODO: replace(origin, from, to, direction, num);
    Bridge_returnString(bridge, result);
}

void ug_string_replace_first(Bridge *bridge)
{
    char *origin = Bridge_receiveString(bridge);
    char *from =Bridge_receiveString(bridge);
    char *to =Bridge_receiveString(bridge);
    char *result = NULL; // TODO: replace(origin, from, to, 1, 1);
    Bridge_returnString(bridge, result);
}

void ug_string_replace_last(Bridge *bridge)
{
    char *origin = Bridge_receiveString(bridge);
    char *from =Bridge_receiveString(bridge);
    char *to =Bridge_receiveString(bridge);
    char *result = NULL; // TODO: replace(origin, from, to, -1, 1);
    Bridge_returnString(bridge, result);
}

void ug_string_replace_all(Bridge *bridge)
{
    char *origin = Bridge_receiveString(bridge);
    char *from =Bridge_receiveString(bridge);
    char *to =Bridge_receiveString(bridge);
    char *result = NULL; // TODO: replace(origin, from, to, 1, -1);
    Bridge_returnString(bridge, result);
}

void ug_string_find(Bridge *bridge)
{
    char *origin = Bridge_receiveString(bridge);
    char *find =Bridge_receiveString(bridge);
    double from = Bridge_receiveNumber(bridge);
    double to = Bridge_receiveNumber(bridge);
    double index = Bridge_receiveNumber(bridge);
    int result = 0; // TODO: find(origin, find, from, to, index);
    Bridge_returnNumber(bridge, result);
}

void ug_string_find_first(Bridge *bridge)
{
    char *origin = Bridge_receiveString(bridge);
    char *find =Bridge_receiveString(bridge);
    int result = 0; // TODO: find(origin, find, 1, -1, 1);
    Bridge_returnNumber(bridge, result);
}

void ug_string_find_last(Bridge *bridge)
{
    char *origin = Bridge_receiveString(bridge);
    char *find =Bridge_receiveString(bridge);
    int result = 0; // TODO: find(origin, find, 1, -1, -1);
    Bridge_returnNumber(bridge, result);
}

void ug_string_cut(Bridge *bridge)
{
    char *origin = Bridge_receiveString(bridge);
    double from = Bridge_receiveNumber(bridge);
    double to = Bridge_receiveNumber(bridge);
    char *result = NULL; // TODO: cut(origin, from, to);
    Bridge_returnString(bridge, result);
}

void ug_string_count(Bridge *bridge)
{
    char *origin = Bridge_receiveString(bridge);
    int result = strlen(origin);
    Bridge_returnNumber(bridge, result);
}

void ug_string_link(Bridge *bridge)
{
    char *origin = Bridge_receiveString(bridge);
    char *other = Bridge_receiveString(bridge);
    char *result = NULL; // TODO: link(origin, other);
    Bridge_returnString(bridge, result);
}

void ug_str_format(Bridge *bridge)
{
    char *f = Bridge_receiveString(bridge);
    Value *v = Bridge_receiveValue(bridge, UG_TYPE_NON);
    Value *t = Bridge_nextValue(bridge);
    tools_assert(t == NULL, "too many arguments for string format");
    String *r = NULL;
    if (v->type == UG_TYPE_NUM) r = String_format(f, v->number);
    if (v->type == UG_TYPE_STR) r = String_format(f, String_get(v->string));
    if (v->type == UG_TYPE_BOL) r = String_format(f, v->boolean);
    if (r == NULL) r = String_format(f, v->object);
    Bridge_returnString(bridge, String_get(r));
    Object_release(r);
}

void ug_str_fill(Bridge *bridge)
{
    char *f = Bridge_receiveString(bridge);
    String *r = String_format("%s", f);
    // 
    Value *v = Bridge_nextValue(bridge);
    int index = 1;
    char *target = tools_format("{%i}", index);
    while (v != NULL)
    {
        char *arg = Value_toString(v);
        int len = String_length(r);
        String_replace(r, target, arg, 0, len - 1, INT_MAX);
        pct_free(arg);
        pct_free(target);
        v = Bridge_nextValue(bridge);
        index = index + 1;
        target = tools_format("{%i}", index);
    }
    pct_free(target);
    // 
    Bridge_returnString(bridge, String_get(r));
    String_free(r);
}

void lib_string_register(Bridge *bridge)
{
    Bridge_startBox(bridge);
    //
    Bridge_bindNative(bridge, "almashturush", ug_string_replace);
    Bridge_bindNative(bridge, "birinchisiniAlmashturush", ug_string_replace_first);
    Bridge_bindNative(bridge, "axirqisiniAlmashturush", ug_string_replace_last);
    Bridge_bindNative(bridge, "hemmisiniAlmashturush", ug_string_replace_all);
    //
    Bridge_bindNative(bridge, "tepish", ug_string_find);
    Bridge_bindNative(bridge, "birinchisiniTepish", ug_string_find_first);
    Bridge_bindNative(bridge, "axirqisiniTepish", ug_string_find_last);
    // 
    Bridge_bindNative(bridge, "kisish", ug_string_cut);
    // 
    Bridge_bindNative(bridge, "sanash", ug_string_count);
    // 
    Bridge_bindNative(bridge, "ulash", ug_string_link);
    Bridge_bindNative(bridge, "formatlash", ug_str_format);
    Bridge_bindNative(bridge, "toldurush", ug_str_fill);
    //
    Bridge_register(bridge, "xet");
}
