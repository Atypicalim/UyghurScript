// stage

#include "./externals.h"
#include "../delegates/header.h"

// fps

#define __E_FPS_CHECK_SIZE 30
#define __E_FPS_PRINT_RATE 10
double _eCheckClock = 0.0f;
double _eCountClock = 0.0f;
double _eCountDelay = 0.5f;
int _eFpsNum = 0;
double _etargetDelay = 1.0f / 15.0f;
double _eFpsDelays[__E_FPS_CHECK_SIZE] = {0.0f};

int externals_check_fps() {
    double current = time_clock();
    double delay = !_eCheckClock ? 0 : current - _eCheckClock;
    _eCheckClock = current;
    //
    double total = 0;
    for (int i = 0; i < __E_FPS_CHECK_SIZE -1; i++) {
        double t = _eFpsDelays[i+1];
        _eFpsDelays[i] = t;
        total = total + t;
    }
    _eFpsDelays[__E_FPS_CHECK_SIZE - 1] = delay;
    total = total + delay;
    // 
    if (total != 0 && current - _eCountClock >= _eCountDelay) {
        double average = 1.0f * total / __E_FPS_CHECK_SIZE;
        _eCountClock = current;
        _eFpsNum = replot_math_round(1 / average);
    }
    // 
    // float extra = (float)MAX(0, MIN(1, _etargetDelay - delay));
    // system_sleep(extra * 1000);
    //
    return _eFpsNum;
}

int externals_get_fps() {
    return _eFpsNum;
}

void externals_set_fps(int fps) {
    fps = MAX(1, MIN(60, fps));
    _etargetDelay = 1.0f / fps;
}

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
    externals_get_fps(fps);
    Bridge_returnEmpty(bridge);
}

void native_stage_get_fps(Bridge *bridge)
{
    int fps = externals_get_fps();
    Bridge_returnNumber(bridge, fps);
}

void native_stage_show(Bridge *bridge)
{
    int w = Bridge_receiveNumber(bridge);
    int h = Bridge_receiveNumber(bridge);
    CString title = Bridge_receiveString(bridge);
    int mode = Bridge_receiveNumber(bridge);
    if (w <= 50) w = 500;
    if (h <= 50) h = 500;
    mode = delegate_stage_mode_swap(mode);
    delegate_stage_run_program(w, h, title, mode);
    replay_InitAudioDevice();
    externals_stage_start(w, h);
    Bridge_returnEmpty(bridge);
}

CString __saveStagePath = NULL;

void native_stage_update(Bridge *bridge)
{
    // print fps
    int fps = externals_check_fps();
    CString _fps = tools_format("fps:%d", fps);
    __eReplot->plot = !USTAGE_USE_SOFT ? deletage_paint_plot : NULL;
    Replot_printText(__eReplot, RPOINT(__eReplot->w / 2, 10), 2, _fps);
    __eReplot->plot = NULL;
    pct_free(_fps);
    // soft render
    if (USTAGE_USE_SOFT) {
        UGPixels pixels = __eReplot->buffer;
        int w = __eReplot->w;
        int h = __eReplot->h;
        unsigned int size;
        delegate_soft_render(pixels, w, h, 4);
    }
    // save screenshot
    if (__saveStagePath != NULL) {
        Replot_write(__eReplot, __saveStagePath);
        __saveStagePath = NULL;
    }
    // update stage
    bool isClose = delegate_stage_update_program();
    Value *value = Value_newBoolean(isClose, NULL);
    helper_set_proxy_value(ALIAS_stage, ALIAS_is_closeable, value);
    externals_stage_update();
    Replot_clear(__eReplot, RCOLOR(11, 11, 11, 255));
    if (isClose) {
        // Replot_write(__eReplot, "replot_stage.png");
        replay_CloseAudioDevice();
        externals_stage_end();
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
    file_remove(path);
    __saveStagePath = path;
    if (!USTAGE_USE_SOFT) {
        log_error("screenshot not supported yet!");
    }
    Bridge_returnEmpty(bridge);
}
