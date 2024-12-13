// mouse

#include "./externals.h"
#include "../delegates/header.h"

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

int __mouse_get_code_from_number(double num) {
    if (num <= 0) return 0;
    return num;
}

int __mouse_get_code_from_string(CString str) {
    if (strlen(str) < 1) return 0;
    char first = str[0];
    if (first == 'l' || first == 'L') return UG_MOUSE_BUTTON_CODE_LEFT;
    if (first == 'm' || first == 'M') return UG_MOUSE_BUTTON_CODE_MIDDLE;
    if (first == 'r' || first == 'R') return UG_MOUSE_BUTTON_CODE_RIGHT;
    return 0;
}

int __mouse_get_code_from_value(Value *value) {
    if (Value_isNumber(value)) {
        return __mouse_get_code_from_number(value->number);
    } else if (Value_isString(value)) {
        return __mouse_get_code_from_string(value->string);
    } else {
        return 0;
    }
}

int __mouse_get_delegate_button(int keyCode) {
    int _keyCode = delegate_board_code_swap(keyCode, true);
    int keyState = delegate_get_board_state(_keyCode);
    return keyState;
}

int __mouse_get_key_state(Value *keyCodeOrName) {
    int keyCode = __mouse_get_code_from_value(keyCodeOrName);
    int currState = __mouse_get_delegate_button(keyCode);
    return currState;
}

int __mouse_get_key_action(Value *keyCodeOrName) {
    int keyCode = __mouse_get_code_from_value(keyCodeOrName);
    int currState = __mouse_get_delegate_button(keyCode);
    return externals_get_button_action(true, keyCode, currState);
}

void native_mouse_get_key_action(Bridge *bridge)
{
    Value *keyCodeOrName = Bridge_receiveValue(bridge, UG_TYPE_NON);
    int action = __mouse_get_key_action(keyCodeOrName);
    Bridge_returnNumber(bridge, action);
}

void native_mouse_get_key_state(Bridge *bridge)
{
    Value *keyCodeOrName = Bridge_receiveValue(bridge, UG_TYPE_NON);
    int state = __mouse_get_key_state(keyCodeOrName);
    Bridge_returnNumber(bridge, state);
}

void native_mouse_is_pressed(Bridge *bridge)
{
    Value *keyCodeOrName = Bridge_receiveValue(bridge, UG_TYPE_NON);
    int action = __mouse_get_key_action(keyCodeOrName);
    bool pressed = action == UG_BUTTON_ACTION_PRESS;
    Bridge_returnBoolean(bridge, pressed);
}

void native_mouse_is_pressing(Bridge *bridge)
{
    Value *keyCodeOrName = Bridge_receiveValue(bridge, UG_TYPE_NON);
    int state = __mouse_get_key_state(keyCodeOrName);
    bool down = state == UG_BUTTON_STATE_DOWN;
    Bridge_returnBoolean(bridge, down);
}

void native_mouse_is_released(Bridge *bridge)
{
    Value *keyCodeOrName = Bridge_receiveValue(bridge, UG_TYPE_NON);
    int action = __mouse_get_key_action(keyCodeOrName);
    bool released = action == UG_BUTTON_ACTION_RELEASE;
    Bridge_returnBoolean(bridge, released);
}
