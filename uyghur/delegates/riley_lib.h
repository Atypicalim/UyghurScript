// riley

#ifndef UG_DELEGATE_RILEY_H
#define UG_DELEGATE_RILEY_H
#include "./delegates.h"

//////////////////////////////////////////////////////////////

int delegate_stage_mode_swap(int mode) {
    if (mode < 0) return mode;
    int _mode = 0;
    _UG_MODE_SWAP_CONVET(UG_STAGE_WINDOW_MODE_DEFAULT, _GU_INVALID_MAX_BITWISE);
    // _UG_MODE_SWAP_CONVET(UG_STAGE_WINDOW_MODE_FIXED, RGFW_NO_RESIZE);
    _UG_MODE_SWAP_CONVET(UG_STAGE_WINDOW_MODE_MAXIMIZED, RGFW_SCALE_TO_MONITOR);
    _UG_MODE_SWAP_CONVET(UG_STAGE_WINDOW_MODE_FULLSCREEN, RGFW_FULLSCREEN);
    _UG_MODE_SWAP_CONVET(UG_STAGE_WINDOW_MODE_TRANSPARENT, RGFW_TRANSPARENT_WINDOW);
    _UG_MODE_SWAP_CONVET(UG_STAGE_WINDOW_MODE_BORDERLESS, RGFW_NO_BORDER);
    return _mode;
}

int delegate_mouse_code_swap(int code, bool fromU) {
    _UG_CODE_SWAP_RETURN(UG_MOUSE_BUTTON_CODE_LEFT, RGFW_mouseLeft);
    _UG_CODE_SWAP_RETURN(UG_MOUSE_BUTTON_CODE_MIDDLE, RGFW_mouseMiddle);
    _UG_CODE_SWAP_RETURN(UG_MOUSE_BUTTON_CODE_RIGHT, RGFW_mouseRight);
    return code;
}

int delegate_mouse_cursor_swap(int type, bool fromU) {
    _UG_TYPE_SWAP_RETURN(UG_MOUSE_CURSOR_TYPE_NONE, RGFW_MOUSE_NORMAL);
    _UG_TYPE_SWAP_RETURN(UG_MOUSE_CURSOR_TYPE_ARROW, RGFW_MOUSE_ARROW);
    _UG_TYPE_SWAP_RETURN(UG_MOUSE_CURSOR_TYPE_HAND, RGFW_MOUSE_POINTING_HAND);
    _UG_TYPE_SWAP_RETURN(UG_MOUSE_CURSOR_TYPE_CROSS, RGFW_MOUSE_CROSSHAIR);
    _UG_TYPE_SWAP_RETURN(UG_MOUSE_CURSOR_TYPE_EDIT, RGFW_MOUSE_IBEAM);
    _UG_TYPE_SWAP_RETURN(UG_MOUSE_CURSOR_TYPE_FORBID, RGFW_MOUSE_NOT_ALLOWED);
    _UG_TYPE_SWAP_RETURN(UG_MOUSE_CURSOR_TYPE_RESIZE, RGFW_MOUSE_RESIZE_ALL);
    return type;
}

int delegate_board_code_swap(int code, bool fromU) {
    _UG_CODE_SWAP_RETURN(UG_BOARD_KEY_NULL, RGFW_KEY_NULL);
    //
    _UG_CODE_SWAP_RETURN(UG_BOARD_KEY_SPACE, RGFW_Space);
    _UG_CODE_SWAP_RETURN(UG_BOARD_KEY_APOSTROPHE, RGFW_Quote);
    _UG_CODE_SWAP_RETURN(UG_BOARD_KEY_COMMA, RGFW_Comma);
    _UG_CODE_SWAP_RETURN(UG_BOARD_KEY_MINUS, RGFW_Minus);
    _UG_CODE_SWAP_RETURN(UG_BOARD_KEY_PERIOD, RGFW_Period);
    _UG_CODE_SWAP_RETURN(UG_BOARD_KEY_SLASH, RGFW_Slash);
    //
    _UG_CODE_SWAP_RETURN(UG_BOARD_KEY_0, RGFW_0);
    _UG_CODE_SWAP_RETURN(UG_BOARD_KEY_1, RGFW_1);
    _UG_CODE_SWAP_RETURN(UG_BOARD_KEY_2, RGFW_2);
    _UG_CODE_SWAP_RETURN(UG_BOARD_KEY_3, RGFW_3);
    _UG_CODE_SWAP_RETURN(UG_BOARD_KEY_4, RGFW_4);
    _UG_CODE_SWAP_RETURN(UG_BOARD_KEY_5, RGFW_5);
    _UG_CODE_SWAP_RETURN(UG_BOARD_KEY_6, RGFW_6);
    _UG_CODE_SWAP_RETURN(UG_BOARD_KEY_7, RGFW_7);
    _UG_CODE_SWAP_RETURN(UG_BOARD_KEY_8, RGFW_8);
    _UG_CODE_SWAP_RETURN(UG_BOARD_KEY_9, RGFW_9);
    //
    _UG_CODE_SWAP_RETURN(UG_BOARD_KEY_SEMICOLON, RGFW_Semicolon);
    _UG_CODE_SWAP_RETURN(UG_BOARD_KEY_EQUAL, RGFW_Equals);
    //
    _UG_CODE_SWAP_RETURN(UG_BOARD_KEY_A, RGFW_a);
    _UG_CODE_SWAP_RETURN(UG_BOARD_KEY_B, RGFW_b);
    _UG_CODE_SWAP_RETURN(UG_BOARD_KEY_C, RGFW_c);
    _UG_CODE_SWAP_RETURN(UG_BOARD_KEY_D, RGFW_d);
    _UG_CODE_SWAP_RETURN(UG_BOARD_KEY_E, RGFW_e);
    _UG_CODE_SWAP_RETURN(UG_BOARD_KEY_F, RGFW_f);
    _UG_CODE_SWAP_RETURN(UG_BOARD_KEY_G, RGFW_g);
    _UG_CODE_SWAP_RETURN(UG_BOARD_KEY_H, RGFW_h);
    _UG_CODE_SWAP_RETURN(UG_BOARD_KEY_I, RGFW_i);
    _UG_CODE_SWAP_RETURN(UG_BOARD_KEY_J, RGFW_j);
    _UG_CODE_SWAP_RETURN(UG_BOARD_KEY_K, RGFW_k);
    _UG_CODE_SWAP_RETURN(UG_BOARD_KEY_L, RGFW_l);
    _UG_CODE_SWAP_RETURN(UG_BOARD_KEY_M, RGFW_m);
    _UG_CODE_SWAP_RETURN(UG_BOARD_KEY_N, RGFW_n);
    _UG_CODE_SWAP_RETURN(UG_BOARD_KEY_O, RGFW_o);
    _UG_CODE_SWAP_RETURN(UG_BOARD_KEY_P, RGFW_p);
    _UG_CODE_SWAP_RETURN(UG_BOARD_KEY_Q, RGFW_q);
    _UG_CODE_SWAP_RETURN(UG_BOARD_KEY_R, RGFW_r);
    _UG_CODE_SWAP_RETURN(UG_BOARD_KEY_S, RGFW_s);
    _UG_CODE_SWAP_RETURN(UG_BOARD_KEY_T, RGFW_t);
    _UG_CODE_SWAP_RETURN(UG_BOARD_KEY_U, RGFW_u);
    _UG_CODE_SWAP_RETURN(UG_BOARD_KEY_V, RGFW_v);
    _UG_CODE_SWAP_RETURN(UG_BOARD_KEY_W, RGFW_w);
    _UG_CODE_SWAP_RETURN(UG_BOARD_KEY_X, RGFW_x);
    _UG_CODE_SWAP_RETURN(UG_BOARD_KEY_Y, RGFW_y);
    _UG_CODE_SWAP_RETURN(UG_BOARD_KEY_Z, RGFW_z);
    //
    _UG_CODE_SWAP_RETURN(UG_BOARD_KEY_LEFT_BRACKET, RGFW_Bracket);
    _UG_CODE_SWAP_RETURN(UG_BOARD_KEY_BACKSLASH, RGFW_BackSlash);
    _UG_CODE_SWAP_RETURN(UG_BOARD_KEY_RIGHT_BRACKET, RGFW_CloseBracket);
    _UG_CODE_SWAP_RETURN(UG_BOARD_KEY_GRAVE, RGFW_Backtick);
    //
    _UG_CODE_SWAP_RETURN(UG_BOARD_KEY_ESCAPE, RGFW_Escape);
    _UG_CODE_SWAP_RETURN(UG_BOARD_KEY_ENTER, RGFW_Return);
    _UG_CODE_SWAP_RETURN(UG_BOARD_KEY_TAB, RGFW_Tab);
    _UG_CODE_SWAP_RETURN(UG_BOARD_KEY_BACKSPACE, RGFW_BackSpace);
    _UG_CODE_SWAP_RETURN(UG_BOARD_KEY_INSERT, RGFW_Insert);
    _UG_CODE_SWAP_RETURN(UG_BOARD_KEY_DELETE, RGFW_Delete);
    _UG_CODE_SWAP_RETURN(UG_BOARD_KEY_RIGHT, RGFW_Right);
    _UG_CODE_SWAP_RETURN(UG_BOARD_KEY_LEFT, RGFW_Left);
    _UG_CODE_SWAP_RETURN(UG_BOARD_KEY_DOWN, RGFW_Down);
    _UG_CODE_SWAP_RETURN(UG_BOARD_KEY_UP, RGFW_Up);
    _UG_CODE_SWAP_RETURN(UG_BOARD_KEY_PAGE_UP, RGFW_PageUp);
    _UG_CODE_SWAP_RETURN(UG_BOARD_KEY_PAGE_DOWN, RGFW_PageDown);
    _UG_CODE_SWAP_RETURN(UG_BOARD_KEY_HOME, RGFW_Home);
    _UG_CODE_SWAP_RETURN(UG_BOARD_KEY_END, RGFW_End);
    _UG_CODE_SWAP_RETURN(UG_BOARD_KEY_CAPS_LOCK, RGFW_CapsLock);
    _UG_CODE_SWAP_RETURN(UG_BOARD_KEY_NUM_LOCK, RGFW_Numlock);
    //
    _UG_CODE_SWAP_RETURN(UG_BOARD_KEY_F1, RGFW_F1);
    _UG_CODE_SWAP_RETURN(UG_BOARD_KEY_F2, RGFW_F2);
    _UG_CODE_SWAP_RETURN(UG_BOARD_KEY_F3, RGFW_F3);
    _UG_CODE_SWAP_RETURN(UG_BOARD_KEY_F4, RGFW_F4);
    _UG_CODE_SWAP_RETURN(UG_BOARD_KEY_F5, RGFW_F5);
    _UG_CODE_SWAP_RETURN(UG_BOARD_KEY_F6, RGFW_F6);
    _UG_CODE_SWAP_RETURN(UG_BOARD_KEY_F7, RGFW_F7);
    _UG_CODE_SWAP_RETURN(UG_BOARD_KEY_F8, RGFW_F8);
    _UG_CODE_SWAP_RETURN(UG_BOARD_KEY_F9, RGFW_F9);
    _UG_CODE_SWAP_RETURN(UG_BOARD_KEY_F10, RGFW_F10);
    _UG_CODE_SWAP_RETURN(UG_BOARD_KEY_F11, RGFW_F11);
    _UG_CODE_SWAP_RETURN(UG_BOARD_KEY_F12, RGFW_F12);
    //
    _UG_CODE_SWAP_RETURN(UG_BOARD_PAD_0, RGFW_KP_0);
    _UG_CODE_SWAP_RETURN(UG_BOARD_PAD_1, RGFW_KP_1);
    _UG_CODE_SWAP_RETURN(UG_BOARD_PAD_2, RGFW_KP_2);
    _UG_CODE_SWAP_RETURN(UG_BOARD_PAD_3, RGFW_KP_3);
    _UG_CODE_SWAP_RETURN(UG_BOARD_PAD_4, RGFW_KP_4);
    _UG_CODE_SWAP_RETURN(UG_BOARD_PAD_5, RGFW_KP_5);
    _UG_CODE_SWAP_RETURN(UG_BOARD_PAD_6, RGFW_KP_6);
    _UG_CODE_SWAP_RETURN(UG_BOARD_PAD_7, RGFW_KP_7);
    _UG_CODE_SWAP_RETURN(UG_BOARD_PAD_8, RGFW_KP_8);
    _UG_CODE_SWAP_RETURN(UG_BOARD_PAD_9, RGFW_KP_9);
    //
    _UG_CODE_SWAP_RETURN(UG_BOARD_PAD_DECIMAL, RGFW_KP_Period);
    _UG_CODE_SWAP_RETURN(UG_BOARD_PAD_DIV, RGFW_KP_Slash);
    _UG_CODE_SWAP_RETURN(UG_BOARD_PAD_MUL, RGFW_Multiply);
    _UG_CODE_SWAP_RETURN(UG_BOARD_PAD_SUB, RGFW_KP_Minus);
    _UG_CODE_SWAP_RETURN(UG_BOARD_PAD_ADD, final_key);
    _UG_CODE_SWAP_RETURN(UG_BOARD_PAD_ENTER, RGFW_KP_Return);
    //
    _UG_CODE_SWAP_RETURN(UG_BOARD_KEY_LEFT_SHIFT, RGFW_ShiftL);
    _UG_CODE_SWAP_RETURN(UG_BOARD_KEY_LEFT_CONTROL, RGFW_ControlL);
    _UG_CODE_SWAP_RETURN(UG_BOARD_KEY_LEFT_ALT, RGFW_AltL);
    _UG_CODE_SWAP_RETURN(UG_BOARD_KEY_LEFT_SUPER, RGFW_SuperL);
    _UG_CODE_SWAP_RETURN(UG_BOARD_KEY_RIGHT_SHIFT, RGFW_ShiftR);
    _UG_CODE_SWAP_RETURN(UG_BOARD_KEY_RIGHT_CONTROL, RGFW_ControlR);
    _UG_CODE_SWAP_RETURN(UG_BOARD_KEY_RIGHT_ALT, RGFW_AltR);
    _UG_CODE_SWAP_RETURN(UG_BOARD_KEY_RIGHT_SUPER, RGFW_SuperR);

    return code;
}

//////////////////////////////////////////////////////////////


double __rileyMouseWheel = 0;
void __riley_on_mouve_button_event(RGFW_window* win, u8 button, double scroll, b8 pressed) {
    if (button == RGFW_mouseScrollUp || button == RGFW_mouseScrollDown) {
        __rileyMouseWheel = scroll;
    }
}

//////////////////////////////////////////////////////////////

RGFW_window* _rileyWindow = NULL;
bool _rileyHasCursor = true;

#define CHECK_RILEY_WINDOW_V() \
    if (_rileyWindow == NULL) { \
        printf("riley window is not created yet!\n"); \
        return; \
    }

#define CHECK_RILEY_WINDOW_B() \
    if (_rileyWindow == NULL) { \
        printf("riley window is not created yet!\n"); \
        return false; \
    }

#define CHECK_RILEY_WINDOW_S() \
    if (_rileyWindow == NULL) { \
        printf("riley window is not created yet!\n"); \
        return NULL; \
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

void stage_toggle_fullscreen() {
    CHECK_RILEY_WINDOW_V();
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
    CHECK_RILEY_WINDOW_V();
    RGFW_rect rect = _rileyWindow->r;
    *x = rect.x;
    *y = rect.y;
}


void stage_set_size(int w, int h) {
    RGFW_window_resize(_rileyWindow, RGFW_AREA(w, h));
}

void stage_get_size(int *w, int *h) {
    CHECK_RILEY_WINDOW_V();
    RGFW_rect rect = _rileyWindow->r;
    *w = rect.w;
    *h = rect.h;
}

void delegate_set_clipboard(CString text) {
    RGFW_writeClipboard(text, strlen(text));
}

CString *delegate_get_clipboard() {
    int size;
    return RGFW_readClipboard(&size);
}

void delegate_set_mouse_visible(bool b) {
    RGFW_window_showMouse(_rileyWindow, b);
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

double delegate_get_mouse_wheel() {
    double movement = __rileyMouseWheel;
    __rileyMouseWheel = 0;
    return movement;
}

int delegate_get_mouse_state(int key) {
    if (RGFW_isMousePressed(_rileyWindow, key)) return UG_BUTTON_STATE_DOWN;
    return UG_BUTTON_STATE_UP;
}

int delegate_get_board_state(int key) {
    if (RGFW_isPressed(_rileyWindow, key)) return UG_BUTTON_STATE_DOWN;
    return UG_BUTTON_STATE_UP;
}

void delegate_stage_save_screenshot(CString path) {
}

void delegate_stage_run_program(int width, int height, CString title, int mode) {
    if (mode < 0) mode = RGFW_CENTER;
    _rileyWindow = RGFW_createWindow(title, (RSGL_rect){500, 500, width, height}, mode);
	RSGL_init(RSGL_AREA(_rileyWindow->r.w, _rileyWindow->r.h), RGFW_getProcAddress);
    // RSGL_setFont(RSGL_loadFont("../resources/ukij.ttf"));
    RGFW_setMouseButtonCallback(__riley_on_mouve_button_event);
}

bool delegate_stage_update_program() {
    //
    CHECK_RILEY_WINDOW_B();
    bool closeable = RGFW_window_shouldClose(_rileyWindow);
    // 
    RSGL_checkEvent(_rileyWindow);
    RGFW_window_swapBuffers(_rileyWindow);
    RSGL_clear(RSGL_RGB(33, 33, 33));
    RGFW_window_checkFPS(_rileyWindow, 0);
    //
    if (closeable && _rileyWindow != NULL) {
        RSGL_free();
        RGFW_window_close(_rileyWindow);
        _rileyWindow = NULL;
        return true;
    } else {
        return false;
    }
}

//////////////////////////////////////////////////////////

#define _COLOR (RSGL_color){color.a, color.b, color.g, color.r}
#define _VECTOR (RSGL_pointF){vector.x, vector.y}
#define _CENTER (RSGL_pointF){center.x, center.y}
#define _ANCHOR (RSGL_pointF){anchor.x, anchor.y}
#define _POINT (RSGL_pointF){point.x, point.y}
#define _POINT1 (RSGL_pointF){point1.x, point1.y}
#define _POINT2 (RSGL_pointF){point2.x, point2.y}
#define _POINT3 (RSGL_pointF){point3.x, point3.y}
#define _RECT (RSGL_rectF){rect.x, rect.y, rect.w, rect.h}

////////////////////////////////////////////////////////// pencil

void delegate_pencil_customize(UGColor color, int rotation) {
    RSGL_rotate(RSGL_POINT3D(0, 0, rotation));
}

void delegate_pencil_draw_pixel(UGPoint point, UGColor color) {
    RSGL_drawPointF(_POINT, _COLOR);
}

void delegate_pencil_draw_line(UGPoint point1, UGPoint point2, int thickness, UGColor color) {
    RSGL_drawLineF(_POINT1, _POINT2, thickness, _COLOR);
}

void delegate_pencil_draw_triangle(UGPoint point1, UGPoint point2, UGPoint point3, UGColor color, int thickness)
{
    RSGL_triangleF _trianlge = (RSGL_triangleF){_POINT1, _POINT2, _POINT3};
    RSGL_drawTriangleFOutline(_trianlge, thickness, _COLOR);
}

void delegate_pencil_fill_triangle(UGPoint point1, UGPoint point2, UGPoint point3, UGColor color)
{
    RSGL_triangleF _trianlge = (RSGL_triangleF){_POINT1, _POINT2, _POINT3};	
    RSGL_drawTriangleF(_trianlge, _COLOR);
}

void delegate_pencil_draw_rectangle(UGPoint point, UGSize size, UGColor color, double thickness)
{
    RSGL_rectF _rect = (RSGL_rectF){point.x - size.w / 2, point.y - size.h / 2, size.w, size.h};
    RSGL_drawRectFOutline(_rect, thickness, _COLOR);
}

void delegate_pencil_fill_rectangle(UGPoint point, UGSize size, UGColor color)
{
    RSGL_rectF _rect = (RSGL_rectF){point.x - size.w / 2, point.y - size.h / 2, size.w, size.h};
    RSGL_drawRectF(_rect, _COLOR);
}

void delegate_pencil_draw_circle(UGPoint point, int radius, UGColor color, int thickness)
{
    RSGL_rectF _rect = (RSGL_rectF){point.x - radius, point.y - radius, radius * 2, radius * 2};
    RSGL_drawOvalFOutline(_rect, thickness, _COLOR);
}

void delegate_pencil_fill_circle(UGPoint point, int radius, UGColor color)
{
    RSGL_rectF _rect = (RSGL_rectF){point.x - radius, point.y - radius, radius * 2, radius * 2};
    RSGL_drawOvalF(_rect, _COLOR);
}

void delegate_pencil_draw_polygon(UGPoint center, int sides, double radius, double rotation, UGColor color, double thickness)
{
    RSGL_rectF _rect = (RSGL_rectF){center.x - radius, center.y - radius, radius * 2, radius * 2};
    RSGL_rotate(RSGL_POINT3D(0, 0, rotation));
    RSGL_drawPolygonFOutline(_rect, sides, thickness, _COLOR);
    RSGL_rotate(RSGL_POINT3D(0, 0, 0));
}

void delegate_pencil_fill_polygon(UGPoint center, int sides, double radius, double rotation, UGColor color)
{
    RSGL_rectF _rect = (RSGL_rectF){center.x - radius, center.y - radius, radius * 2, radius * 2};
    RSGL_rotate(RSGL_POINT3D(0, 0, rotation));
    RSGL_drawPolygonF(_rect, sides, _COLOR);
    RSGL_rotate(RSGL_POINT3D(0, 0, 0));
}

//////////////////////////////////////////////////////////

void *delegate_unload_image(UGImage *img) {
    RSGL_image *_image = img->image;
    if (_image != NULL) {}
    return _image;
}

void *delegate_unload_font(UGFont *fnt) {
    int *_font = fnt->font;
    if (_font != NULL) {}
    return _font;
}

UGImage *delegate_load_image(UGImage *img)
{
    RSGL_image image0 = RSGL_loadImage(img->path);
    RSGL_area size = image0.srcSize;
    int x = img->x > 0 ? img->x : 0;
    int y = img->y > 0 ? img->y : 0;
    int w = img->w > 0 ? img->w : size.w;
    int h = img->h > 0 ? img->h : size.h;
    //
    img->w = size.w;
    img->h = size.h;
    // //
    RSGL_image *_image = (RSGL_image *)malloc(sizeof(RSGL_image));
    _image[0] = image0;
    img->image = (void*)_image;
    return img;
}

UGFont *delegate_load_font(UGFont *fnt)
{
    int font = RSGL_loadFont(fnt->path);
    int *_font = (int *)malloc(sizeof(int));
    *_font = font;
    fnt->font = (void*)_font;
    return fnt;
}

//////////////////////////////////////////////////////////

void delegate_pencil_draw_image(UGImage *image, int x, int y, float anchorX, float anchorY, UGColor color, float rotation, float scale) {
    RSGL_image *img = image->image;
    RSGL_image _image = img[0];
    // 
    int wVal = image->w * scale;
    int hVal = image->h * scale;
    int xPos = x - anchorX * wVal;
    int yPos = y - anchorY * hVal;
    //
    RSGL_rect _rect = (RSGL_rect){xPos, yPos, wVal, hVal};
    RSGL_rotate(RSGL_POINT3D(0, 0, rotation));
    RSGL_drawImage(image->path, _rect);
    RSGL_rotate(RSGL_POINT3D(0, 0, 0));
}

void delegate_pencil_draw_font(UGFont *font, char *text, int size, UGColor color, UGPoint point) {
    int *fnt = font->font;
    // 
    int count = strlen(text);
    point.x = point.x - count*size/3.25;
    point.y = point.y - size/2;
    // 
    RSGL_circle _circle = (RSGL_circle) {point.x, point.y, size};
    RSGL_setFont(*fnt);
    RSGL_drawText(text, _circle, _COLOR);
}

//////////////////////////////////////////////////////////

#undef _COLOR
#undef _VECTOR
#undef _CENTER
#undef _ANCHOR
#undef _POINT
#undef _POINT1
#undef _POINT2
#undef _POINT3
#undef _RECT

//////////////////////////////////////////////////////////

#endif
