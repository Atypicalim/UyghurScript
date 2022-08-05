// time

#include "../uyghur/uyghur.c"

void ug_time_get_zone(Bridge *bridge)
{
    Bridge_startResult(bridge);
    Bridge_pushNumber(bridge, time_get_zone());
    Bridge_return(bridge);
}

void ug_time_get_seconds(Bridge *bridge)
{
    Bridge_startResult(bridge);
    Bridge_pushNumber(bridge, time_get_seconds());
    Bridge_return(bridge);
}

void ug_time_create_seconds(Bridge *bridge)
{
    char *str = Bridge_nextString(bridge);
    Bridge_startResult(bridge);
    Bridge_pushNumber(bridge, time_create_seconds(str));
    Bridge_return(bridge);
}

void ug_time_format_seconds(Bridge *bridge)
{
    int seconds = Bridge_nextNumber(bridge);
    char *str = Bridge_nextString(bridge);
    char *time = time_format_seconds(seconds, str);
    Bridge_startResult(bridge);
    Bridge_pushString(bridge, time);
    Bridge_return(bridge);
    pct_free(time);
}

void ug_time_get_date(Bridge *bridge)
{
    Bridge_startResult(bridge);
    Bridge_pushString(bridge, time_get_date());
    Bridge_return(bridge);
}

void ug_time_get_clock(Bridge *bridge)
{
    Bridge_startResult(bridge);
    Bridge_pushNumber(bridge, time_get_clock());
    Bridge_return(bridge);
}

void ug_time_sleep_seconds(Bridge *bridge)
{
    time_sleep_seconds(Bridge_nextNumber(bridge));
    Bridge_startResult(bridge);
    Bridge_return(bridge);
}

void lib_time_register(Bridge *bridge)
{
    Bridge_startBox(bridge, "waqit");
    //
    Bridge_pushKey(bridge, "rayunElish");
    Bridge_pushFunction(bridge, ug_time_get_zone);
    Bridge_pushKey(bridge, "sikuntElish");
    Bridge_pushFunction(bridge, ug_time_get_seconds);
    Bridge_pushKey(bridge, "sikuntqaAylandurush");
    Bridge_pushFunction(bridge, ug_time_create_seconds);
    Bridge_pushKey(bridge, "sikuntniAylandurush");
    Bridge_pushFunction(bridge, ug_time_format_seconds);
    Bridge_pushKey(bridge, "chislaElish");
    Bridge_pushFunction(bridge, ug_time_get_date);
    Bridge_pushKey(bridge, "sanaqElish");
    Bridge_pushFunction(bridge, ug_time_get_clock);
    Bridge_pushKey(bridge, "programminiUxlitish");
    Bridge_pushFunction(bridge, ug_time_sleep_seconds);
    //
    Bridge_register(bridge);
}
