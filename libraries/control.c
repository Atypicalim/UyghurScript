// control

#include "raylib.h"
#include "../uyghur/uyghur.c"

void ug_control_show_button(Bridge *bridge)
{
    Rectangle rectangle = rectangle_from_bridge(bridge);
    char *text = Bridge_nextString(bridge);
    bool r = GuiButton(rectangle, text);
    Bridge_startResult(bridge);
    Bridge_pushBoolean(bridge, r);
    Bridge_return(bridge);
}

// other

void lib_raylib_control_register(Bridge *bridge)
{
    //
    Bridge_startBox(bridge, "zapchas");
    //
    Bridge_pushKey(bridge, "kunupkaKorsitish");
    Bridge_pushFunction(bridge, ug_control_show_button);
    //
    Bridge_register(bridge);
}
