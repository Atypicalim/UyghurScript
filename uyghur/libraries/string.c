// string

#include "../uyghur.c"

void ug_string_replace(Bridge *bridge)
{
    char *origin = Bridge_popString(bridge);
    char *from =Bridge_popString(bridge);
    char *to =Bridge_popString(bridge);
    double direction = Bridge_popNumber(bridge);
    double num = Bridge_popNumber(bridge);
    char *result = str_replace(origin, from, to, direction, num);
    Bridge_startResult(bridge);
    Bridge_pushString(bridge, result);
    Bridge_return(bridge);
}

void ug_string_replace_first(Bridge *bridge)
{
    char *origin = Bridge_popString(bridge);
    char *from =Bridge_popString(bridge);
    char *to =Bridge_popString(bridge);
    char *result = str_replace(origin, from, to, 1, 1);
    Bridge_startResult(bridge);
    Bridge_pushString(bridge, result);
    Bridge_return(bridge);
}

void ug_string_replace_last(Bridge *bridge)
{
    char *origin = Bridge_popString(bridge);
    char *from =Bridge_popString(bridge);
    char *to =Bridge_popString(bridge);
    char *result = str_replace(origin, from, to, -1, 1);
    Bridge_startResult(bridge);
    Bridge_pushString(bridge, result);
    Bridge_return(bridge);
}

void ug_string_replace_all(Bridge *bridge)
{
    char *origin = Bridge_popString(bridge);
    char *from =Bridge_popString(bridge);
    char *to =Bridge_popString(bridge);
    char *result = str_replace(origin, from, to, 1, -1);
    Bridge_startResult(bridge);
    Bridge_pushString(bridge, result);
    Bridge_return(bridge);
}

void ug_string_find(Bridge *bridge)
{
    char *origin = Bridge_popString(bridge);
    char *find =Bridge_popString(bridge);
    double from = Bridge_popNumber(bridge);
    double to = Bridge_popNumber(bridge);
    double index = Bridge_popNumber(bridge);
    int result = str_find(origin, find, from, to, index);
    Bridge_startResult(bridge);
    Bridge_pushNumber(bridge, result);
    Bridge_return(bridge);
}

void ug_string_find_first(Bridge *bridge)
{
    char *origin = Bridge_popString(bridge);
    char *find =Bridge_popString(bridge);
    int result = str_find(origin, find, 1, -1, 1);
    Bridge_startResult(bridge);
    Bridge_pushNumber(bridge, result);
    Bridge_return(bridge);
}

void ug_string_find_last(Bridge *bridge)
{
    char *origin = Bridge_popString(bridge);
    char *find =Bridge_popString(bridge);
    int result = str_find(origin, find, 1, -1, -1);
    Bridge_startResult(bridge);
    Bridge_pushNumber(bridge, result);
    Bridge_return(bridge);
}

void ug_string_cut(Bridge *bridge)
{
    char *origin = Bridge_popString(bridge);
    double from = Bridge_popNumber(bridge);
    double to = Bridge_popNumber(bridge);
    char *result = str_cut(origin, from, to);
    Bridge_startResult(bridge);
    Bridge_pushString(bridge, result);
    Bridge_return(bridge);
}

void ug_string_count(Bridge *bridge)
{
    char *origin = Bridge_popString(bridge);
    int result = str_count(origin);
    Bridge_startResult(bridge);
    Bridge_pushNumber(bridge, result);
    Bridge_return(bridge);
}

void ug_string_link(Bridge *bridge)
{
    char *origin = Bridge_popString(bridge);
    char *other = Bridge_popString(bridge);
    char *result = str_link(origin, other);
    Bridge_startResult(bridge);
    Bridge_pushString(bridge, result);
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
    //
    Bridge_register(bridge);
}
