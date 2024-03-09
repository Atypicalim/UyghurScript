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
    Value *v = Bridge_receiveValue(bridge, UG_CHR_NON);
    String *r = NULL;
    if (v->type == UG_RTYPE_NUM) r = String_format(f, v->number);
    if (v->type == UG_RTYPE_STR) r = String_format(f, v->string);
    if (v->type == UG_RTYPE_BOL) r = String_format(f, v->boolean);
    if (r == NULL) r = String_format("");
    Bridge_returnString(bridge, String_get(r));
    Object_release(r);
}

void lib_string_register(Bridge *bridge)
{
    Bridge_startBox(bridge, "xet");
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
    //
    Bridge_register(bridge);
}
