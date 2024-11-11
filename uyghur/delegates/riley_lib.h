// riley

#include "../include.h"


RGFW_window* _rileyWindow = NULL;
bool _rileyHasCursor = true;

#define CHECK_WRILEY_WINDOW() \
    if (_rileyWindow == NULL) { \
        printf("riley window is not created yet!\n"); \
        return; \
    }

#define STAGE_IS_FULLSCREEN RGFW_window_isFullscreen(_rileyWindow)
#define STAGE_IS_HIDDEN RGFW_window_isHidden(_rileyWindow)
#define STAGE_IS_MINIMIZED RGFW_window_isMinimized(_rileyWindow)
#define STAGE_IS_MAXIMIZED RGFW_window_isMaximized(_rileyWindow)

#define STAGE_SET_MINIMIZE RGFW_window_minimize(_rileyWindow)
#define STAGE_SET_MAXIMIZE RGFW_window_maximize(_rileyWindow)
#define STAGE_SET_NORMALIZE RGFW_window_restore(_rileyWindow)

void delegate_stage_set_fps(int fps) {

}

int delegate_stage_get_fps() {
    return 60;
}

int delegate_draw_debug(int x, int y) {
    RSGL_drawText("60", RSGL_CIRCLE(x, y, 24), RSGL_RGB(255, 0, 0));
}

void stage_toggle_fullscreen() {
    CHECK_WRILEY_WINDOW();
    _rileyWindow->_winArgs ^= RGFW_FULLSCREEN;
}

void stage_set_title(char *title) {
    RGFW_window_setName(_rileyWindow, title);
}

void stage_set_icon(char *path) {
    u8 icon[4 * 3 * 3] = {
        0xFF, 0x00, 0x00, 0xFF,
        0xFF, 0x00, 0x00, 0xFF,
        0xFF, 0x00, 0x00, 0xFF,
        0xFF, 0x00, 0x00, 0x00,
        0xFF, 0xFF, 0x00, 0xFF,
        0xFF, 0xFF, 0x00, 0xFF,
        0xFF, 0x00, 0x00, 0xFF,
        0xFF, 0x00, 0x00, 0xFF,
        0xFF, 0x00, 0x00, 0xFF
    };
	RGFW_window_setIcon(_rileyWindow, icon, RGFW_AREA(3, 3), 4);
}

void stage_set_position(int x, int y) {
    RGFW_window_move(_rileyWindow, RGFW_POINT(x, y));
}

void stage_get_position(int *x, int *y) {
    CHECK_WRILEY_WINDOW();
    RGFW_rect rect = _rileyWindow->r;
    *x = rect.x;
    *y = rect.y;
}


void stage_set_size(int w, int h) {
    RGFW_window_resize(_rileyWindow, RGFW_AREA(w, h));
}

void stage_get_size(int *w, int *h) {
    CHECK_WRILEY_WINDOW();
    RGFW_rect rect = _rileyWindow->r;
    *w = rect.w;
    *h = rect.h;
}

void delegate_set_min_size(int w, int h) {
    RGFW_window_setMinSize(_rileyWindow, RGFW_AREA(w, h));
}

void delegate_show_cursor(bool b) {
    RGFW_window_showMouse(_rileyWindow, b);
    _rileyHasCursor = b;
}

bool delegate_have_cursor() {
    return _rileyHasCursor;
}

void delegate_set_clipboard(CString text) {
    RGFW_writeClipboard(text, strlen(text));
}

CString *delegate_get_clipboard() {
    int size;
    return RGFW_readClipboard(&size);
}

void delegate_set_mouse_cursor(int typ) {
    RGFW_window_setMouseStandard(_rileyWindow, typ);
}

void delegate_set_mouse_location(int x, int y) {
    RGFW_window_moveMouse(_rileyWindow, RGFW_POINT(x, y));
}

void delegate_get_mouse_location(int *x, int *y) {
    RGFW_point point = RGFW_window_getMousePoint(_rileyWindow);
    *x = point.x;
    *y = point.y;
}

int delegate_was_mouse_pressed(int key) {
    return RGFW_wasMousePressed(_rileyWindow, key);
}

int delegate_is_mouse_pressed(int key) {
    return RGFW_isMousePressed(_rileyWindow, key);
}

int delegate_was_key_pressed(int key) {
    return RGFW_wasPressed(_rileyWindow, key);
}

int delegate_is_key_pressed(int key) {
    return RGFW_isPressed(_rileyWindow, key);
}

void delegate_stage_save_screenshot(CString path) {
}

void delegate_stage_run_program() {
    _rileyWindow = RGFW_createWindow("name", (RSGL_rect){500, 500, 500, 500}, RGFW_CENTER);
	RSGL_init(RSGL_AREA(_rileyWindow->r.w, _rileyWindow->r.h), RGFW_getProcAddress);
    RSGL_setFont(RSGL_loadFont("../resources/ukij.ttf"));
}

void delegate_stage_update_program() {
    //
    CHECK_WRILEY_WINDOW();
    bool closeable = RGFW_window_shouldClose(_rileyWindow);
    Value *value = Value_newBoolean(closeable, NULL);
    helper_set_proxy_value(ALIAS_stage, ALIAS_stage_is_closeable, value);
    // 
    RSGL_checkEvent(_rileyWindow);
    RSGL_clear(RSGL_RGB(50, 50, 50));
    RGFW_window_swapBuffers(_rileyWindow);
    RGFW_window_checkFPS(_rileyWindow, 0);
    //
    if (closeable && _rileyWindow != NULL) {
        RSGL_free();
        RGFW_window_close(_rileyWindow);
        _rileyWindow = NULL;
    }
}
