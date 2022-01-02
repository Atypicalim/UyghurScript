// string

#include "../uyghur.c"

void ug_string_replace(Bridge *bridge)
{
    char *origin = Bridge_nexString(bridge);
    char *from =Bridge_nexString(bridge);
    char *to =Bridge_nexString(bridge);
    double direction = Bridge_nexNumber(bridge);
    double num = Bridge_nexNumber(bridge);
    char *result = str_replace(origin, from, to, direction, num);
    Bridge_startResult(bridge);
    Bridge_pushString(bridge, result);
    Bridge_return(bridge);
}

void ug_string_replace_first(Bridge *bridge)
{
    char *origin = Bridge_nexString(bridge);
    char *from =Bridge_nexString(bridge);
    char *to =Bridge_nexString(bridge);
    char *result = str_replace(origin, from, to, 1, 1);
    Bridge_startResult(bridge);
    Bridge_pushString(bridge, result);
    Bridge_return(bridge);
}

void ug_string_replace_last(Bridge *bridge)
{
    char *origin = Bridge_nexString(bridge);
    char *from =Bridge_nexString(bridge);
    char *to =Bridge_nexString(bridge);
    char *result = str_replace(origin, from, to, -1, 1);
    Bridge_startResult(bridge);
    Bridge_pushString(bridge, result);
    Bridge_return(bridge);
}

void ug_string_replace_all(Bridge *bridge)
{
    char *origin = Bridge_nexString(bridge);
    char *from =Bridge_nexString(bridge);
    char *to =Bridge_nexString(bridge);
    char *result = str_replace(origin, from, to, 1, -1);
    Bridge_startResult(bridge);
    Bridge_pushString(bridge, result);
    Bridge_return(bridge);
}

void ug_string_find(Bridge *bridge)
{
    char *origin = Bridge_nexString(bridge);
    char *find =Bridge_nexString(bridge);
    double from = Bridge_nexNumber(bridge);
    double to = Bridge_nexNumber(bridge);
    double index = Bridge_nexNumber(bridge);
    int result = str_find(origin, find, from, to, index);
    Bridge_startResult(bridge);
    Bridge_pushNumber(bridge, result);
    Bridge_return(bridge);
}

void ug_string_find_first(Bridge *bridge)
{
    char *origin = Bridge_nexString(bridge);
    char *find =Bridge_nexString(bridge);
    int result = str_find(origin, find, 1, -1, 1);
    Bridge_startResult(bridge);
    Bridge_pushNumber(bridge, result);
    Bridge_return(bridge);
}

void ug_string_find_last(Bridge *bridge)
{
    char *origin = Bridge_nexString(bridge);
    char *find =Bridge_nexString(bridge);
    int result = str_find(origin, find, 1, -1, -1);
    Bridge_startResult(bridge);
    Bridge_pushNumber(bridge, result);
    Bridge_return(bridge);
}

void ug_string_cut(Bridge *bridge)
{
    char *origin = Bridge_nexString(bridge);
    double from = Bridge_nexNumber(bridge);
    double to = Bridge_nexNumber(bridge);
    char *result = str_cut(origin, from, to);
    Bridge_startResult(bridge);
    Bridge_pushString(bridge, result);
    Bridge_return(bridge);
}

void ug_string_count(Bridge *bridge)
{
    char *origin = Bridge_nexString(bridge);
    int result = str_count(origin);
    Bridge_startResult(bridge);
    Bridge_pushNumber(bridge, result);
    Bridge_return(bridge);
}

void ug_string_link(Bridge *bridge)
{
    char *origin = Bridge_nexString(bridge);
    char *other = Bridge_nexString(bridge);
    char *result = str_link(origin, other);
    Bridge_startResult(bridge);
    Bridge_pushString(bridge, result);
    Bridge_return(bridge);
}

void ug_str_format(Bridge *bridge)
{
    char *f = Bridge_nexString(bridge);
    Value *v = Bridge_nextValue(bridge);
    char *r = Value_toString(v);
    if (v->type == RTYPE_NUMBER) r = str_format(f, v->number);
    if (v->type == RTYPE_STRING) r = str_format(f, v->string);
    if (v->type == RTYPE_BOOLEAN) r = str_format(f, v->boolean);
    Bridge_startResult(bridge);
    Bridge_pushString(bridge, r);
    Bridge_return(bridge);
}

void lib_string_register(Bridge *bridge)
{
    Bridge_startBox(bridge, "xet");
    //
    Bridge_pushKey(bridge, "almashturush");
    Bridge_pushFunction(bridge, ug_string_replace);
    Bridge_pushKey(bridge, "birinchisiniAlmashturush");
    Bridge_pushFunction(bridge, ug_string_replace_first);
    Bridge_pushKey(bridge, "axirqisiniAlmashturush");
    Bridge_pushFunction(bridge, ug_string_replace_last);
    Bridge_pushKey(bridge, "hemmisiniAlmashturush");
    Bridge_pushFunction(bridge, ug_string_replace_all);
    //
    Bridge_pushKey(bridge, "tepish");
    Bridge_pushFunction(bridge, ug_string_find);
    Bridge_pushKey(bridge, "birinchisiniTepish");
    Bridge_pushFunction(bridge, ug_string_find_first);
    Bridge_pushKey(bridge, "axirqisiniTepish");
    Bridge_pushFunction(bridge, ug_string_find_last);
    // 
    Bridge_pushKey(bridge, "kisish");
    Bridge_pushFunction(bridge, ug_string_cut);
    // 
    Bridge_pushKey(bridge, "sanash");
    Bridge_pushFunction(bridge, ug_string_count);
    // 
    Bridge_pushKey(bridge, "ulash");
    Bridge_pushFunction(bridge, ug_string_link);
    Bridge_pushKey(bridge, "formatlash");
    Bridge_pushFunction(bridge, ug_str_format);
    //
    Bridge_register(bridge);
}
