// control

#include "raylib.h"
#include "../uyghur.c"

void native_control_show_button(Bridge *bridge)
{
    Rectangle rectangle = rectangle_from_bridge(bridge);
    USTRING text = Bridge_receiveString(bridge);
    bool r = GuiButton(rectangle, text);
    Bridge_returnBoolean(bridge, r);
}

void native_control_show_label(Bridge *bridge)
{
    Rectangle rectangle = rectangle_from_bridge(bridge);
    USTRING text = Bridge_receiveString(bridge);
    GuiLabel(rectangle, text);
    Bridge_returnEmpty(bridge);
}
