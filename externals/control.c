// control

#include "raylib.h"
#include "../uyghur/uyghur.c"

void ug_control_show_button(Bridge *bridge)
{
    Rectangle rectangle = rectangle_from_bridge(bridge);
    char *text = Bridge_receiveString(bridge);
    bool r = GuiButton(rectangle, text);
    Bridge_returnBoolean(bridge, r);
}

void ug_control_show_label(Bridge *bridge)
{
    Rectangle rectangle = rectangle_from_bridge(bridge);
    char *text = Bridge_receiveString(bridge);
    GuiLabel(rectangle, text);
    Bridge_returnEmpty(bridge);
}

// other

void lib_raylib_control_register(Bridge *bridge)
{
    //
    Bridge_startBox(bridge);
    //
    Bridge_bindNative(bridge, "kunupkaKorsitish", ug_control_show_button);
    Bridge_bindNative(bridge, "xetKorsitish", ug_control_show_label);
    //
    Bridge_register(bridge, "zapchas");
}
