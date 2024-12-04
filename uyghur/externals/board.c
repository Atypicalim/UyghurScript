// mouse

#include "./externals.h"
#include "../delegates/header.h"

int __board_get_code_from_number(double num) {
    if (num <= 0) return 0;
    return num;
}

int __board_get_code_from_string(CString str) {
    if (strlen(str) < 1) return 0;
    char first = str[0];
    if (isdigit(first)) return (int)first;
    if (isalpha(first)) return (~0x20) & first;
    return 0;
}

int __board_get_code_from_value(Value *value) {
    if (Value_isNumber(value)) {
        return __board_get_code_from_number(value->number);
    } else if (Value_isString(value)) {
        return __board_get_code_from_string(value->string);
    } else {
        return 0;
    }
}

int __board_get_key_state(int keyCode) {
    keyCode = delegate_board_code_swap(keyCode, true);
    int state = delegate_get_board_state(keyCode);
    return state;
}

int __board_get_key_action(Value *keyCodeOrName) {
    int keyCode = __board_get_code_from_value(keyCodeOrName);
    int currState = __board_get_key_state(keyCode);
    return externals_get_button_action(false, keyCode, currState);
}

void native_board_get_key_action(Bridge *bridge)
{
    Value *keyCodeOrName = Bridge_receiveValue(bridge, UG_TYPE_NON);
    int action = __board_get_key_action(keyCodeOrName);
    Bridge_returnNumber(bridge, action);
}

void native_board_get_key_state(Bridge *bridge)
{
    Value *keyCodeOrName = Bridge_receiveValue(bridge, UG_TYPE_NON);
    int keyCode = __board_get_code_from_value(keyCodeOrName);
    int state = __board_get_key_state(keyCode);
    Bridge_returnNumber(bridge, state);
}

void native_board_is_pressed(Bridge *bridge)
{
    Value *keyCodeOrName = Bridge_receiveValue(bridge, UG_TYPE_NON);
    int action = __board_get_key_action(keyCodeOrName);
    bool pressed = action == UG_BUTTON_ACTION_PRESS;
    Bridge_returnBoolean(bridge, pressed);
}

void native_board_is_released(Bridge *bridge)
{
    Value *keyCodeOrName = Bridge_receiveValue(bridge, UG_TYPE_NON);
    int action = __board_get_key_action(keyCodeOrName);
    bool released = action == UG_BUTTON_ACTION_RELEASE;
    Bridge_returnBoolean(bridge, released);
}
