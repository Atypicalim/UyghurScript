// tigr

#ifndef UG_DELEGATE_TIGER_H
#define UG_DELEGATE_TIGER_H
#include "./delegates.h"

//////////////////////////////////////////////////////////////

int delegate_stage_mode_swap(int mode) {
    int _mode = mode <= 0 ? TIGR_AUTO : TIGR_AUTO | TIGR_FULLSCREEN;
    return _mode;
}

int delegate_mouse_code_swap(int code, bool fromU) {
    _UG_CODE_SWAP_RETURN(UG_MOUSE_BUTTON_CODE_LEFT, 1);
    _UG_CODE_SWAP_RETURN(UG_MOUSE_BUTTON_CODE_MIDDLE, 2);
    _UG_CODE_SWAP_RETURN(UG_MOUSE_BUTTON_CODE_RIGHT, 4);
    return code;
}

int delegate_mouse_cursor_swap(int type, bool fromU) {
    return type;
}

int delegate_board_code_swap(int code, bool fromU) {
    _UG_CODE_SWAP_RETURN(UG_BOARD_KEY_NULL, 0);
    //
    _UG_CODE_SWAP_RETURN(UG_BOARD_KEY_SPACE, TK_SPACE);
    _UG_CODE_SWAP_RETURN(UG_BOARD_KEY_APOSTROPHE, TK_TICK);
    _UG_CODE_SWAP_RETURN(UG_BOARD_KEY_COMMA, TK_COMMA);
    _UG_CODE_SWAP_RETURN(UG_BOARD_KEY_MINUS, TK_MINUS);
    _UG_CODE_SWAP_RETURN(UG_BOARD_KEY_PERIOD, TK_DOT);
    _UG_CODE_SWAP_RETURN(UG_BOARD_KEY_SLASH, TK_SLASH);
    //
    _UG_CODE_SWAP_RETURN(UG_BOARD_KEY_0, '0');
    _UG_CODE_SWAP_RETURN(UG_BOARD_KEY_1, '1');
    _UG_CODE_SWAP_RETURN(UG_BOARD_KEY_2, '2');
    _UG_CODE_SWAP_RETURN(UG_BOARD_KEY_3, '3');
    _UG_CODE_SWAP_RETURN(UG_BOARD_KEY_4, '4');
    _UG_CODE_SWAP_RETURN(UG_BOARD_KEY_5, '5');
    _UG_CODE_SWAP_RETURN(UG_BOARD_KEY_6, '6');
    _UG_CODE_SWAP_RETURN(UG_BOARD_KEY_7, '7');
    _UG_CODE_SWAP_RETURN(UG_BOARD_KEY_8, '8');
    _UG_CODE_SWAP_RETURN(UG_BOARD_KEY_9, '9');
    //
    _UG_CODE_SWAP_RETURN(UG_BOARD_KEY_SEMICOLON, TK_SEMICOLON);
    _UG_CODE_SWAP_RETURN(UG_BOARD_KEY_EQUAL, TK_EQUALS);
    //
    _UG_CODE_SWAP_RETURN(UG_BOARD_KEY_A, 'A');
    _UG_CODE_SWAP_RETURN(UG_BOARD_KEY_B, 'B');
    _UG_CODE_SWAP_RETURN(UG_BOARD_KEY_C, 'C');
    _UG_CODE_SWAP_RETURN(UG_BOARD_KEY_D, 'D');
    _UG_CODE_SWAP_RETURN(UG_BOARD_KEY_E, 'E');
    _UG_CODE_SWAP_RETURN(UG_BOARD_KEY_F, 'F');
    _UG_CODE_SWAP_RETURN(UG_BOARD_KEY_G, 'G');
    _UG_CODE_SWAP_RETURN(UG_BOARD_KEY_H, 'H');
    _UG_CODE_SWAP_RETURN(UG_BOARD_KEY_I, 'I');
    _UG_CODE_SWAP_RETURN(UG_BOARD_KEY_J, 'J');
    _UG_CODE_SWAP_RETURN(UG_BOARD_KEY_K, 'K');
    _UG_CODE_SWAP_RETURN(UG_BOARD_KEY_L, 'L');
    _UG_CODE_SWAP_RETURN(UG_BOARD_KEY_M, 'M');
    _UG_CODE_SWAP_RETURN(UG_BOARD_KEY_N, 'N');
    _UG_CODE_SWAP_RETURN(UG_BOARD_KEY_O, 'O');
    _UG_CODE_SWAP_RETURN(UG_BOARD_KEY_P, 'P');
    _UG_CODE_SWAP_RETURN(UG_BOARD_KEY_Q, 'Q');
    _UG_CODE_SWAP_RETURN(UG_BOARD_KEY_R, 'R');
    _UG_CODE_SWAP_RETURN(UG_BOARD_KEY_S, 'S');
    _UG_CODE_SWAP_RETURN(UG_BOARD_KEY_T, 'T');
    _UG_CODE_SWAP_RETURN(UG_BOARD_KEY_U, 'U');
    _UG_CODE_SWAP_RETURN(UG_BOARD_KEY_V, 'V');
    _UG_CODE_SWAP_RETURN(UG_BOARD_KEY_W, 'W');
    _UG_CODE_SWAP_RETURN(UG_BOARD_KEY_X, 'X');
    _UG_CODE_SWAP_RETURN(UG_BOARD_KEY_Y, 'Y');
    _UG_CODE_SWAP_RETURN(UG_BOARD_KEY_Z, 'Z');
    //
    _UG_CODE_SWAP_RETURN(UG_BOARD_KEY_LEFT_BRACKET, TK_LSQUARE);
    _UG_CODE_SWAP_RETURN(UG_BOARD_KEY_BACKSLASH, TK_BACKSLASH);
    _UG_CODE_SWAP_RETURN(UG_BOARD_KEY_RIGHT_BRACKET, TK_RSQUARE);
    _UG_CODE_SWAP_RETURN(UG_BOARD_KEY_GRAVE, TK_BACKTICK);
    //
    _UG_CODE_SWAP_RETURN(UG_BOARD_KEY_ESCAPE, TK_ESCAPE);
    _UG_CODE_SWAP_RETURN(UG_BOARD_KEY_ENTER, TK_RETURN);
    _UG_CODE_SWAP_RETURN(UG_BOARD_KEY_TAB, TK_TAB);
    _UG_CODE_SWAP_RETURN(UG_BOARD_KEY_BACKSPACE, TK_BACKSPACE);
    _UG_CODE_SWAP_RETURN(UG_BOARD_KEY_INSERT, TK_INSERT);
    _UG_CODE_SWAP_RETURN(UG_BOARD_KEY_DELETE, TK_DELETE);
    _UG_CODE_SWAP_RETURN(UG_BOARD_KEY_RIGHT, TK_RIGHT);
    _UG_CODE_SWAP_RETURN(UG_BOARD_KEY_LEFT, TK_LEFT);
    _UG_CODE_SWAP_RETURN(UG_BOARD_KEY_DOWN, TK_DOWN);
    _UG_CODE_SWAP_RETURN(UG_BOARD_KEY_UP, TK_UP);
    _UG_CODE_SWAP_RETURN(UG_BOARD_KEY_PAGE_UP, TK_PAGEUP);
    _UG_CODE_SWAP_RETURN(UG_BOARD_KEY_PAGE_DOWN, TK_PAGEDN);
    _UG_CODE_SWAP_RETURN(UG_BOARD_KEY_HOME, TK_HOME);
    _UG_CODE_SWAP_RETURN(UG_BOARD_KEY_END, TK_END);
    _UG_CODE_SWAP_RETURN(UG_BOARD_KEY_CAPS_LOCK, TK_CAPSLOCK);
    _UG_CODE_SWAP_RETURN(UG_BOARD_KEY_NUM_LOCK, TK_NUMLOCK);
    //
    _UG_CODE_SWAP_RETURN(UG_BOARD_KEY_F1, TK_F1);
    _UG_CODE_SWAP_RETURN(UG_BOARD_KEY_F2, TK_F2);
    _UG_CODE_SWAP_RETURN(UG_BOARD_KEY_F3, TK_F3);
    _UG_CODE_SWAP_RETURN(UG_BOARD_KEY_F4, TK_F4);
    _UG_CODE_SWAP_RETURN(UG_BOARD_KEY_F5, TK_F5);
    _UG_CODE_SWAP_RETURN(UG_BOARD_KEY_F6, TK_F6);
    _UG_CODE_SWAP_RETURN(UG_BOARD_KEY_F7, TK_F7);
    _UG_CODE_SWAP_RETURN(UG_BOARD_KEY_F8, TK_F8);
    _UG_CODE_SWAP_RETURN(UG_BOARD_KEY_F9, TK_F9);
    _UG_CODE_SWAP_RETURN(UG_BOARD_KEY_F10, TK_F10);
    _UG_CODE_SWAP_RETURN(UG_BOARD_KEY_F11, TK_F11);
    _UG_CODE_SWAP_RETURN(UG_BOARD_KEY_F12, TK_F12);
    //
    _UG_CODE_SWAP_RETURN(UG_BOARD_PAD_0, TK_PAD0);
    _UG_CODE_SWAP_RETURN(UG_BOARD_PAD_1, TK_PAD1);
    _UG_CODE_SWAP_RETURN(UG_BOARD_PAD_2, TK_PAD2);
    _UG_CODE_SWAP_RETURN(UG_BOARD_PAD_3, TK_PAD3);
    _UG_CODE_SWAP_RETURN(UG_BOARD_PAD_4, TK_PAD4);
    _UG_CODE_SWAP_RETURN(UG_BOARD_PAD_5, TK_PAD5);
    _UG_CODE_SWAP_RETURN(UG_BOARD_PAD_6, TK_PAD6);
    _UG_CODE_SWAP_RETURN(UG_BOARD_PAD_7, TK_PAD7);
    _UG_CODE_SWAP_RETURN(UG_BOARD_PAD_8, TK_PAD8);
    _UG_CODE_SWAP_RETURN(UG_BOARD_PAD_9, TK_PAD9);
    //
    _UG_CODE_SWAP_RETURN(UG_BOARD_PAD_DECIMAL, TK_PADDOT);
    _UG_CODE_SWAP_RETURN(UG_BOARD_PAD_DIV, TK_PADDIV);
    _UG_CODE_SWAP_RETURN(UG_BOARD_PAD_MUL, TK_PADMUL);
    _UG_CODE_SWAP_RETURN(UG_BOARD_PAD_SUB, TK_PADSUB);
    _UG_CODE_SWAP_RETURN(UG_BOARD_PAD_ADD, TK_PADADD);
    _UG_CODE_SWAP_RETURN(UG_BOARD_PAD_ENTER, TK_PADENTER);
    //
    _UG_CODE_SWAP_RETURN(UG_BOARD_KEY_LEFT_SHIFT, TK_SHIFT);
    _UG_CODE_SWAP_RETURN(UG_BOARD_KEY_LEFT_CONTROL, TK_LCONTROL);
    _UG_CODE_SWAP_RETURN(UG_BOARD_KEY_LEFT_ALT, TK_LALT);
    _UG_CODE_SWAP_RETURN(UG_BOARD_KEY_LEFT_SUPER, TK_LWIN);
    _UG_CODE_SWAP_RETURN(UG_BOARD_KEY_RIGHT_SHIFT, TK_SHIFT);
    _UG_CODE_SWAP_RETURN(UG_BOARD_KEY_RIGHT_CONTROL, TK_RCONTROL);
    _UG_CODE_SWAP_RETURN(UG_BOARD_KEY_RIGHT_ALT, TK_RALT);
    _UG_CODE_SWAP_RETURN(UG_BOARD_KEY_RIGHT_SUPER, TK_RWIN);

    return code;
}

//////////////////////////////////////////////////////////////


double __tigrMouseWheel = 0;
void __tigr_on_mouve_button_event(Tigr* win, int button) {
    // if (button == _mouseScrollUp || button == _mouseScrollDown) {
    //     __tigrMouseWheel = scroll;
    // }
}

//////////////////////////////////////////////////////////////

Tigr* _tigrWindow = NULL;
bool _tigrHasCursor = true;

#define CHECK_TIGR_WINDOW_V() \
    if (_tigrWindow == NULL) { \
        printf("tigr window is not created yet!\n"); \
        return; \
    }

#define CHECK_TIGR_WINDOW_B() \
    if (_tigrWindow == NULL) { \
        printf("tigr window is not created yet!\n"); \
        return false; \
    }

#define CHECK_TIGR_WINDOW_S() \
    if (_tigrWindow == NULL) { \
        printf("tigr window is not created yet!\n"); \
        return NULL; \
    }

#define STAGE_IS_FULLSCREEN false
#define STAGE_IS_HIDDEN false
#define STAGE_IS_MINIMIZED false
#define STAGE_IS_MAXIMIZED false

#define STAGE_SET_MINIMIZE tigrTime()
#define STAGE_SET_MAXIMIZE tigrTime()
#define STAGE_SET_NORMALIZE tigrTime()

void delegate_stage_set_fps(int fps) {

}

int delegate_stage_get_fps() {
    return 60;
}

void stage_toggle_fullscreen() {
    CHECK_TIGR_WINDOW_V();
}

void stage_set_title(char *title) {
}

void stage_set_icon(char *path) {
}

void stage_set_position(int x, int y) {
}

void stage_get_position(int *x, int *y) {
}


void stage_set_size(int w, int h) {
}

void stage_get_size(int *w, int *h) {
    CHECK_TIGR_WINDOW_V();
    *w = _tigrWindow->w;
    *h = _tigrWindow->h;
}

void delegate_set_clipboard(CString text) {
}

CString *delegate_get_clipboard() {
    return "";
}

void delegate_set_mouse_visible(bool b) {
}

void delegate_set_mouse_cursor(int typ) {
}

void delegate_set_mouse_location(int x, int y) {
}

void delegate_get_mouse_location(int *x, int *y) {
    int _flags = 0;
    tigrMouse(_tigrWindow, &x, &y, &_flags);
}

double delegate_get_mouse_wheel() {
    double movement = __tigrMouseWheel;
    __tigrMouseWheel = 0;
    return movement;
}

int delegate_get_mouse_state(int key) {
    int x = 0;
    int y = 0;
    int _flags = 0;
    tigrMouse(_tigrWindow, &x, &y, &_flags);
    if (_flags & key) return UG_BUTTON_STATE_DOWN;
    return UG_BUTTON_STATE_UP;
}

int delegate_get_board_state(int key) {
    if (tigrKeyHeld(_tigrWindow, key)) return UG_BUTTON_STATE_DOWN;
    return UG_BUTTON_STATE_UP;
}

void delegate_stage_save_screenshot(CString path) {
    CHECK_TIGR_WINDOW_V();
    tigrSaveImage(path, _tigrWindow);
}

void delegate_stage_run_program(int width, int height, CString title, int mode) {
    if (mode < 0) mode = TIGR_AUTO;
    _tigrWindow = tigrWindow(width, height, title, mode);
    // __tigr_on_mouve_button_event
}

bool delegate_stage_update_program() {
    //
    CHECK_TIGR_WINDOW_B();
    bool closeable = tigrClosed(_tigrWindow);
    // 
    tigrUpdate(_tigrWindow);
    tigrClear(_tigrWindow, tigrRGB(0x33, 0x33, 0x33));
    //
    if (closeable && _tigrWindow != NULL) {
        tigrFree(_tigrWindow);
        _tigrWindow = NULL;
        return true;
    } else {
        return false;
    }
}

//////////////////////////////////////////////////////////

TPixel __plotColor;
void deletage_paint_plot(int x, int y, int r, int g, int b, int a) {
    CHECK_TIGR_WINDOW_V();
    __plotColor.r = r;
    __plotColor.g = g;
    __plotColor.b = b;
    __plotColor.a = a;
    tigrPlot(_tigrWindow, x, y, __plotColor);
}

TPixel __renderColor;
void delegate_soft_render(UGPixels pixels, int w, int h, int channel) {
    for (int y = 0; y < h; y++) {
        for (int x = 0; x < w; x++) {
            int index = (y * w + x) * channel;
            __renderColor.r = pixels[index];
            __renderColor.g = pixels[index + 1];
            __renderColor.b = pixels[index + 2];
            __renderColor.a = pixels[index + 3];
            tigrPlot(_tigrWindow, x, y, __renderColor);
        }
    }
}

//////////////////////////////////////////////////////////

void *delegate_unload_image(UGImage *img) {
    return NULL;
}

void *delegate_unload_font(UGFont *fnt) {
    return NULL;
}

UGImage *delegate_load_image(UGImage *img)
{
    return img;
}

UGFont *delegate_load_font(UGFont *fnt)
{
    return fnt;
}

//////////////////////////////////////////////////////////

#endif
