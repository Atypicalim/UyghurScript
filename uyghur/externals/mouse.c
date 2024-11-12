// mouse

#include "../delegates/header.h"
#include "../uyghur.c"

bool __mouseIsVisible = true;

void native_mouse_set_visible(Bridge *bridge)
{
    bool visible = Bridge_receiveBoolean(bridge);
    delegate_set_mouse_visible(visible);
    __mouseIsVisible = visible;
    Bridge_returnEmpty(bridge);
}

void native_mouse_is_visible(Bridge *bridge)
{
    bool visible = __mouseIsVisible;
    Bridge_returnBoolean(bridge, visible);
}

void native_mouse_set_cursor(Bridge *bridge)
{
    int type = Bridge_receiveNumber(bridge);
    type = delegate_mouse_cursor_swap(type, true);
    delegate_set_mouse_cursor(type);
    Bridge_returnEmpty(bridge);
}

void native_mouse_get_position(Bridge *bridge)
{
    int x = 0; 
    int y = 0;
    delegate_get_mouse_location(&x, &y);
    Bridge_returnNumbers(bridge, 2, x, y);
}

void native_mouse_get_wheel(Bridge *bridge)
{
    int movement = delegate_get_mouse_wheel();
    Bridge_returnNumber(bridge, movement);
}

void native_mouse_get_key_action(Bridge *bridge)
{
    int keyCode = Bridge_receiveNumber(bridge);
    keyCode = delegate_mouse_code_swap(keyCode, true);
    int action = delegate_get_mouse_action(keyCode);
    Bridge_returnNumber(bridge, action);
}

void native_mouse_get_key_state(Bridge *bridge)
{
    int keyCode = Bridge_receiveNumber(bridge);
    keyCode = delegate_mouse_code_swap(keyCode, true);
    int action = delegate_get_mouse_state(keyCode);
    Bridge_returnNumber(bridge, action);
}

void native_mouse_is_pressed(Bridge *bridge)
{
    int keyCode = Bridge_receiveNumber(bridge);
    keyCode = delegate_mouse_code_swap(keyCode, true);
    int action = delegate_get_mouse_action(keyCode);
    bool pressed = action == UG_BUTTON_ACTION_PRESS;
    Bridge_returnBoolean(bridge, pressed);
}

void native_mouse_is_released(Bridge *bridge)
{
    int keyCode = Bridge_receiveNumber(bridge);
    keyCode = delegate_mouse_code_swap(keyCode, true);
    int action = delegate_get_mouse_action(keyCode);
    bool released = action == UG_BUTTON_ACTION_RELEASE;
    Bridge_returnBoolean(bridge, released);
}
