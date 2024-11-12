// stage

#include "../delegates/header.h"
#include "../uyghur.c"

// callback

void stage_on_show() {
}

void stage_on_hide() {
}

void stage_on_focus()
{
}

void stage_on_resize() {
}

void stage_on_drop() {
}

// api

void native_stage_set_fps(Bridge *bridge)
{
    int fps = Bridge_receiveNumber(bridge);
    delegate_stage_set_fps(fps);
    Bridge_returnEmpty(bridge);
}

void native_stage_get_fps(Bridge *bridge)
{
    int fps = delegate_stage_get_fps();
    Bridge_returnNumber(bridge, fps);
}

void native_stage_draw_fps(Bridge *bridge)
{
    int x = Bridge_receiveNumber(bridge);
    int y = Bridge_receiveNumber(bridge);
    delegate_draw_debug(x, y);
    Bridge_returnEmpty(bridge);
}

void native_stage_show_window(Bridge *bridge)
{
    int w = Bridge_receiveNumber(bridge);
    int h = Bridge_receiveNumber(bridge);
    CString title = Bridge_receiveString(bridge);
    int mode = Bridge_receiveNumber(bridge);
    delegate_stage_run_program(w, h, title, mode);
    raudio_InitAudioDevice();
    Bridge_returnEmpty(bridge);
}

void native_stage_update_window(Bridge *bridge)
{
    bool isClose = delegate_stage_update_program();
    if (isClose) {
        raudio_CloseAudioDevice();
    }
    Bridge_returnEmpty(bridge);
}

void native_stage_is_fullscreen(Bridge *bridge)
{
    bool ok = STAGE_IS_FULLSCREEN;
    Bridge_returnBoolean(bridge, ok);
}

void native_stage_is_hidden(Bridge *bridge)
{
    bool ok = STAGE_IS_HIDDEN;
    Bridge_returnBoolean(bridge, ok);
}

void native_stage_is_minimized(Bridge *bridge)
{
    bool ok = STAGE_IS_MINIMIZED;
    Bridge_returnBoolean(bridge, ok);
}

void native_stage_is_maximized(Bridge *bridge)
{
    bool ok = STAGE_IS_MAXIMIZED;
    Bridge_returnBoolean(bridge, ok);
}

void native_stage_toggle_fullscreen(Bridge *bridge)
{
    stage_toggle_fullscreen();
    Bridge_returnEmpty(bridge);
}

void native_stage_set_minimize(Bridge *bridge)
{
    STAGE_SET_MINIMIZE;
    Bridge_returnEmpty(bridge);
}

void native_stage_set_maximize(Bridge *bridge)
{
    STAGE_SET_MAXIMIZE;
    Bridge_returnEmpty(bridge);
}

void native_stage_set_normalize(Bridge *bridge)
{
    STAGE_SET_NORMALIZE;
    Bridge_returnEmpty(bridge);
}

void native_stage_set_title(Bridge *bridge)
{
    CString title = Bridge_receiveString(bridge);
    stage_set_title(title);
    Bridge_returnEmpty(bridge);
}

void native_stage_set_icon(Bridge *bridge)
{
    CString path = Bridge_receiveString(bridge);
    stage_set_icon(path);
    Bridge_returnEmpty(bridge);
}

void native_stage_set_position(Bridge *bridge)
{
    int x = Bridge_receiveNumber(bridge);
    int y = Bridge_receiveNumber(bridge);
    stage_set_position(x, y);
    Bridge_returnEmpty(bridge);
}

void native_stage_get_position(Bridge *bridge)
{
    int x = 0;
    int y = 0;
    stage_get_position(&x, &y);
    Bridge_returnNumbers(bridge, 2, x, y);
}

void native_stage_set_size(Bridge *bridge)
{
    int w = Bridge_receiveNumber(bridge);
    int h = Bridge_receiveNumber(bridge);
    stage_set_size(w, h); 
    Bridge_returnEmpty(bridge);
}

void native_stage_get_size(Bridge *bridge)
{
    int w = 0;
    int h = 0;
    stage_get_size(&w, &h);
    Bridge_returnNumbers(bridge, 2, w, h);
}

void native_stage_set_min_size(Bridge *bridge)
{
    int w = Bridge_receiveNumber(bridge);
    int h = Bridge_receiveNumber(bridge);
    delegate_set_min_size(w, h);
    Bridge_returnEmpty(bridge);
}

void native_stage_set_clipboard(Bridge *bridge)
{
    CString c = Bridge_receiveString(bridge);
    delegate_set_clipboard(c);
    Bridge_returnEmpty(bridge);
}

void native_stage_get_clipboard(Bridge *bridge)
{
    CString r = delegate_get_clipboard();
    Bridge_returnString(bridge, r);
}

void native_stage_save_screenshot(Bridge *bridge)
{
    CString path = Bridge_receiveString(bridge);
    delegate_stage_save_screenshot(path);
    Bridge_returnEmpty(bridge);
}
