// time

#include "../uyghur/uyghur.c"

void ug_time_get_zone(Bridge *bridge)
{
    Bridge_returnNumber(bridge, time_get_zone());
}

void ug_time_get_seconds(Bridge *bridge)
{
    Bridge_returnNumber(bridge, time_get_seconds());
}

void ug_time_create_seconds(Bridge *bridge)
{
    char *str = Bridge_receiveString(bridge);
    Bridge_returnNumber(bridge, time_create_seconds(str));
}

void ug_time_format_seconds(Bridge *bridge)
{
    int seconds = Bridge_receiveNumber(bridge);
    char *str = Bridge_receiveString(bridge);
    char *time = time_format_seconds(seconds, str);
    Bridge_returnString(bridge, time);
    pct_free(time);
}

void ug_time_get_date(Bridge *bridge)
{
    Bridge_returnString(bridge, time_get_date());
}

void ug_time_get_clock(Bridge *bridge)
{
    Bridge_returnNumber(bridge, time_get_clock());
}

void ug_time_sleep_seconds(Bridge *bridge)
{
    time_sleep_seconds(Bridge_receiveNumber(bridge));
    Bridge_returnEmpty(bridge);
}

void lib_time_register(Bridge *bridge)
{
    Bridge_startBox(bridge, "waqit");
    //
    Bridge_bindNative(bridge, "rayunElish", ug_time_get_zone);
    Bridge_bindNative(bridge, "sikuntElish", ug_time_get_seconds);
    Bridge_bindNative(bridge, "sikuntqaAylandurush", ug_time_create_seconds);
    Bridge_bindNative(bridge, "sikuntniAylandurush", ug_time_format_seconds);
    Bridge_bindNative(bridge, "chislaElish", ug_time_get_date);
    Bridge_bindNative(bridge, "sanaqElish", ug_time_get_clock);
    Bridge_bindNative(bridge, "programminiUxlitish", ug_time_sleep_seconds);
    //
    Bridge_register(bridge);
}
