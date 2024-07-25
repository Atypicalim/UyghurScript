// time

#include "../uyghur.c"

void native_time_get_zone(Bridge *bridge)
{
    Bridge_returnNumber(bridge, time_get_zone());
}

void native_time_get_seconds(Bridge *bridge)
{
    Bridge_returnNumber(bridge, time_get_seconds());
}

void native_time_convert_to_seconds(Bridge *bridge)
{
    char *str = Bridge_receiveString(bridge);
    Bridge_returnNumber(bridge, time_convert_to_seconds(str));
}

void native_time_convert_from_seconds(Bridge *bridge)
{
    int seconds = Bridge_receiveNumber(bridge);
    char *str = Bridge_receiveString(bridge);
    char *time = time_convert_from_seconds(seconds, str);
    Bridge_returnString(bridge, time);
    pct_free(time);
}

void native_time_get_time(Bridge *bridge)
{
    Bridge_returnString(bridge, time_get_time());
}

void native_time_get_clock(Bridge *bridge)
{
    Bridge_returnNumber(bridge, time_get_clock());
}

void native_time_sleep_seconds(Bridge *bridge)
{
    time_sleep_seconds(Bridge_receiveNumber(bridge));
    Bridge_returnEmpty(bridge);
}

void lib_time_register(Bridge *bridge)
{
    Bridge_startBox(bridge);
    //
    BRIDGE_BIND_NATIVE(time_get_zone);
    BRIDGE_BIND_NATIVE(time_get_seconds);
    BRIDGE_BIND_NATIVE(time_convert_to_seconds);
    BRIDGE_BIND_NATIVE(time_convert_from_seconds);
    BRIDGE_BIND_NATIVE(time_get_time);
    BRIDGE_BIND_NATIVE(time_get_clock);
    BRIDGE_BIND_NATIVE(time_sleep_seconds);
    //
    Bridge_register(bridge, ALIAS_TIME);
}
