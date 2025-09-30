// externals

#ifndef UG_EXTERNALS_HEADER
#define UG_EXTERNALS_HEADER

#include "../include.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <math.h>
#include <stdarg.h>

#define _GU_INVALID_MAX_BITWISE 1024

// swap helper
#define _UG_MODE_SWAP_CONVET(uMode, oMode) if(mode & uMode) _mode = _mode | oMode;
#define _UG_CODE_SWAP_RETURN(uCode, oCode) if(fromU && code == uCode) return oCode; if (!fromU && code == oCode) return uCode;
#define _UG_TYPE_SWAP_RETURN(uType, oType) if(fromU && type == uType) return oType; if (!fromU && type == oType) return uType;

// stage window
#define UG_STAGE_WINDOW_MODE_DEFAULT 0
#define UG_STAGE_WINDOW_MODE_FIXED 2
#define UG_STAGE_WINDOW_MODE_MAXIMIZED 8
#define UG_STAGE_WINDOW_MODE_FULLSCREEN 16
#define UG_STAGE_WINDOW_MODE_TRANSPARENT 32
#define UG_STAGE_WINDOW_MODE_BORDERLESS 32
#define UG_STAGE_WINDOW_MODE_PINNED 4

// button type
#define UG_BUTTON_TYPE_MOUSE 1
#define UG_BUTTON_TYPE_BOARD 2

// mouse btn
#define UG_MOUSE_BUTTON_CODE_LEFT 1
#define UG_MOUSE_BUTTON_CODE_MIDDLE 2
#define UG_MOUSE_BUTTON_CODE_RIGHT 3

// mouse cursor
#define UG_MOUSE_CURSOR_TYPE_NONE 0
#define UG_MOUSE_CURSOR_TYPE_ARROW 1
#define UG_MOUSE_CURSOR_TYPE_HAND 2
#define UG_MOUSE_CURSOR_TYPE_CROSS 3
#define UG_MOUSE_CURSOR_TYPE_EDIT 4
#define UG_MOUSE_CURSOR_TYPE_FORBID 5
#define UG_MOUSE_CURSOR_TYPE_RESIZE 6

// button state
#define UG_BUTTON_STATE_UP 0
#define UG_BUTTON_STATE_DOWN 1

// button action
#define UG_BUTTON_ACTION_NONE 0
#define UG_BUTTON_ACTION_PRESS 1
#define UG_BUTTON_ACTION_RELEASE -1


// board keys (same as raylib)
typedef enum UG_BOARD_KEYS {
    UG_BOARD_KEY_NULL = 0,
    //
    UG_BOARD_KEY_SPACE      = 32, // Key: Space
    UG_BOARD_KEY_APOSTROPHE = 39,       // Key: '
    UG_BOARD_KEY_COMMA      = 44,       // Key: ,
    UG_BOARD_KEY_MINUS      = 45,       // Key: -
    UG_BOARD_KEY_PERIOD     = 46,       // Key: .
    UG_BOARD_KEY_SLASH      = 47,       // Key: /
    //
    UG_BOARD_KEY_0 = 48,
    UG_BOARD_KEY_1,
    UG_BOARD_KEY_2,
    UG_BOARD_KEY_3,
    UG_BOARD_KEY_4,
    UG_BOARD_KEY_5,
    UG_BOARD_KEY_6,
    UG_BOARD_KEY_7,
    UG_BOARD_KEY_8,
    UG_BOARD_KEY_9,
    //
    UG_BOARD_KEY_SEMICOLON       = 59,       // Key: ;
    UG_BOARD_KEY_EQUAL           = 61,       // Key: =
    //
    UG_BOARD_KEY_A = 65,
    UG_BOARD_KEY_B,
    UG_BOARD_KEY_C,
    UG_BOARD_KEY_D,
    UG_BOARD_KEY_E,
    UG_BOARD_KEY_F,
    UG_BOARD_KEY_G,
    UG_BOARD_KEY_H,
    UG_BOARD_KEY_I,
    UG_BOARD_KEY_J,
    UG_BOARD_KEY_K,
    UG_BOARD_KEY_L,
    UG_BOARD_KEY_M,
    UG_BOARD_KEY_N,
    UG_BOARD_KEY_O,
    UG_BOARD_KEY_P,
    UG_BOARD_KEY_Q,
    UG_BOARD_KEY_R,
    UG_BOARD_KEY_S,
    UG_BOARD_KEY_T,
    UG_BOARD_KEY_U,
    UG_BOARD_KEY_V,
    UG_BOARD_KEY_W,
    UG_BOARD_KEY_X,
    UG_BOARD_KEY_Y,
    UG_BOARD_KEY_Z,
    //
    UG_BOARD_KEY_LEFT_BRACKET = 91, // Key: [
    UG_BOARD_KEY_BACKSLASH,         // Key: '\'
    UG_BOARD_KEY_RIGHT_BRACKET,     // Key: ]
    UG_BOARD_KEY_GRAVE,             // Key: `
    //
    UG_BOARD_KEY_ESCAPE = 256, // Key: Esc
    UG_BOARD_KEY_ENTER,         // Key: Enter
    UG_BOARD_KEY_TAB,           // Key: Tab
    UG_BOARD_KEY_BACKSPACE,     // Key: Backspace
    UG_BOARD_KEY_INSERT,        // Key: Ins
    UG_BOARD_KEY_DELETE,        // Key: Del
    UG_BOARD_KEY_RIGHT,         // Key: Cursor right
    UG_BOARD_KEY_LEFT,          // Key: Cursor left
    UG_BOARD_KEY_DOWN,          // Key: Cursor down
    UG_BOARD_KEY_UP,            // Key: Cursor up
    UG_BOARD_KEY_PAGE_UP,       // Key: Page up
    UG_BOARD_KEY_PAGE_DOWN,     // Key: Page down
    UG_BOARD_KEY_HOME,          // Key: Home
    UG_BOARD_KEY_END,           // Key: End
    UG_BOARD_KEY_CAPS_LOCK,     // Key: Caps lock
    UG_BOARD_KEY_NUM_LOCK,      // Key: Num lock
    //
    UG_BOARD_KEY_F1 = 290,
    UG_BOARD_KEY_F2,
    UG_BOARD_KEY_F3,
    UG_BOARD_KEY_F4,
    UG_BOARD_KEY_F5,
    UG_BOARD_KEY_F6,
    UG_BOARD_KEY_F7,
    UG_BOARD_KEY_F8,
    UG_BOARD_KEY_F9,
    UG_BOARD_KEY_F10,
    UG_BOARD_KEY_F11,
    UG_BOARD_KEY_F12,
    //
    UG_BOARD_PAD_0 = 320,
    UG_BOARD_PAD_1,
    UG_BOARD_PAD_2,
    UG_BOARD_PAD_3,
    UG_BOARD_PAD_4,
    UG_BOARD_PAD_5,
    UG_BOARD_PAD_6,
    UG_BOARD_PAD_7,
    UG_BOARD_PAD_8,
    UG_BOARD_PAD_9,
    //
    UG_BOARD_PAD_DECIMAL = 330, // Keypad .
    UG_BOARD_PAD_DIV,      // Keypad /
    UG_BOARD_PAD_MUL,      // Keypad *
    UG_BOARD_PAD_SUB,      // Keypad -
    UG_BOARD_PAD_ADD,      // Keypad +
    UG_BOARD_PAD_ENTER,    // Keypad Enter
    //
    UG_BOARD_KEY_LEFT_SHIFT = 340,  // Key: Shift left
    UG_BOARD_KEY_LEFT_CONTROL,      // Key: Control left
    UG_BOARD_KEY_LEFT_ALT,          // Key: Alt left
    UG_BOARD_KEY_LEFT_SUPER,        // Key: Super left
    UG_BOARD_KEY_RIGHT_SHIFT,       // Key: Shift right
    UG_BOARD_KEY_RIGHT_CONTROL,     // Key: Control right
    UG_BOARD_KEY_RIGHT_ALT,         // Key: Alt right
    UG_BOARD_KEY_RIGHT_SUPER,       // Key: Super right
} UG_BOARD_KEYS;

#define UGPixels unsigned char*
#define UGTexture void*

typedef struct UGColor {
    unsigned char r;
    unsigned char g;
    unsigned char b;
    unsigned char a;
} UGColor;

typedef struct UGSize {
    float w;
    float h;
} UGSize;

typedef struct UGPoint {
    float x;
    float y;
} UGPoint;

typedef struct UGRect {
    float x;
    float y;
    union {
        float w;
        float width;
    };
    union {
        float h;
        float height;
    };
} UGRect;


UGPoint UG_VECTOR_LERP(UGPoint p1, UGPoint p2, float t) {
    return (UGPoint) {math_lerp(p1.x, p2.x, t), math_lerp(p1.y, p2.y, t)};
}

UGPoint UG_POINT_LERP(UGPoint p1, UGPoint p2, float t) {
    return (UGPoint) {math_lerp(p1.x, p2.x, t), math_lerp(p1.y, p2.y, t)};
}

void UG_POINT_SWAP(UGPoint *p1, UGPoint *p2) {
    int t = (*p1).x;
    (*p1).x = (*p2).x;
    (*p2).x = t;
    t = (*p1).y;
    (*p1).y = (*p2).y;
    (*p2).y = t;
}

void UG_POINT_PRINT(UGPoint *p) {
    printf("<point:%p %f,%f>", *p, (*p).x, (*p).y);
}

UGRect UG_RECT_GENERATE(UGPoint *p, UGSize *s) {
    UGRect r;
    r.x = p->x - s->w / 2;
    r.y = p->y - s->h / 2;
    r.w = s->w;
    r.h = s->h;
    return r;
}

typedef struct UGImage {
    char *path;
    UGPixels pxls;
    UGTexture txtr;
    int w;
    int h;
    int c;
} UGImage;

typedef struct UGFont {
    char *path;
    void *font;
} UGFont;


UGImage *UG_NEW_IMAGE(char *path, int x0, int y0, int w0, int h0) {
    //
    int w1;
    int h1;
    int _c;
    RTexture _texture = rimage_read(path, &w1, &h1, &_c);
    //
    if (x0 < 0) x0 = 0;
    if (y0 < 0) y0 = 0;
    if (w0 <= 0) w0 = w1;
    if (h0 <= 0) h0 = h1;
    //
    int x2 = MIN(MAX(0, x0), w1 - 1);
    int y2 = MIN(MAX(0, y0), h1 - 1);
    int w2 = MIN(MAX(1, w0), w1 - x2);
    int h2 = MIN(MAX(1, h0), h1 - y2);
    //
    int size = w2*h2*_c*sizeof(RByte);
    RTexture texture = (RTexture)malloc(size);
    for (int y = 0; y < h2; y++) {
        for (int x = 0; x < w2; x++) {
	        size_t index1 = ((y2 + y) * w1) + (x2 + x);
	        size_t index2 = ((0 + y) * w2) + (0 + x);
            size_t indent1 = index1 * _c;
            size_t indent2 = index2 * _c;
            RPIXEL_COPY_TO(texture, indent2, _texture, indent1);
        }
    }
    //
    UGImage *img = (UGImage *)malloc(sizeof(UGImage));
    img->pxls = texture;
    img->path = strdup(path);
    img->txtr = NULL;
    img->w = w2;
    img->h = h2;
    img->c = _c;
    //
    rimage_free(&_texture);
    return img;
}

UGFont *UG_NEW_FONT(char *path) {
    UGFont *fnt = (UGFont *)malloc(sizeof(UGFont));
    fnt->path = strdup(path);
    fnt->font = NULL;
    return fnt;
}

// tool

UGColor color_from_bridge(Bridge *bridge)
{
    Value *color = Bridge_receiveValue(bridge, UG_TYPE_NON);
    unsigned char r = 255; 
    unsigned char g = 255; 
    unsigned char b = 255; 
    unsigned char a = 255;
    //
    if (color->type == UG_TYPE_NUM) {
        unsigned int num = (unsigned int)color->number;
        color_hex_to_rgb(num, &r, &g, &b);
    } else if (color->type == UG_TYPE_STR) {
        CString str = color->string[0] == '0' && color->string[1] == 'x' ? color->string + 2 : color->string;
        int len = strlen(str);
        if (len == 6 || len == 8) {
            r = char_to_int(str[0]) * 16 + char_to_int(str[1]);
            g = char_to_int(str[2]) * 16 + char_to_int(str[3]);
            b = char_to_int(str[4]) * 16 + char_to_int(str[5]);
            a = len == 6 ? 255 : char_to_int(str[6]) * 16 + char_to_int(str[7]);
        }
    }
    //
    return (UGColor){r, g, b, a};
}

UGPoint point_from_bridge(Bridge *bridge)
{
    float x = Bridge_receiveNumber(bridge);
    float y = Bridge_receiveNumber(bridge);
    return (UGPoint){x, y};
}

UGSize size_from_bridge(Bridge *bridge)
{
    float w = Bridge_receiveNumber(bridge);
    float h = Bridge_receiveNumber(bridge);
    return (UGSize){w, h};
}

UGRect rect_from_bridge(Bridge *bridge)
{
    float x = Bridge_receiveNumber(bridge);
    float y = Bridge_receiveNumber(bridge);
    float w = Bridge_receiveNumber(bridge);
    float h = Bridge_receiveNumber(bridge);
    return (UGRect){x, y, w, h};
}

#define ECanvas Replot*
#define EColor RColor

Replot *__eReplot = NULL;
RColor __eColor;

UGColor gPenColor = (UGColor){200, 255, 200, 255};
int gPenRotation = 0;
float gPenScale = 1.0f;
int gPenThickness = 1;

#define UG_PENCIL_FOCUS_NONE 0
#define UG_PENCIL_FOCUS_PAPER 1
#define UG_PENCIL_FOCUS_STAGE 2

char gPenFocus = 0;
void* gPenTarget = NULL;

void pencil_focus_to(int type, void *target) {
    gPenFocus = type;
    gPenTarget = target;
}

ECanvas paper_from_bridge(Bridge *bridge)
{
    Loadable *loadable = Bridge_receiveValue(bridge, UG_TYPE_STF);
    ECanvas canvas = loadable->obj;
    return canvas;
}

void __release_paper(Loadable *loadable) {
    if (loadable->obj) {
        ECanvas canvas = loadable->obj;
        if (canvas == gPenTarget) {
            pencil_focus_to(UG_PENCIL_FOCUS_NONE, NULL);
        }
        Replot_free(canvas);
        loadable->obj = NULL;
    }
}

#define __UG_BOARD_SIZE 512
#define __UG_MOUSE_SIZE 512
int __ugBoardPressedOld[__UG_BOARD_SIZE];
int __ugBoardPressedNew[__UG_BOARD_SIZE];
int __ugMousePressedOld[__UG_MOUSE_SIZE];
int __ugMousePressedNew[__UG_MOUSE_SIZE];

void externals_stage_start(int w, int h) {
    if (__eReplot != NULL) Replot_free(__eReplot);
    __eReplot = Replot_new(w, h);
    __eColor = (RColor){255, 255, 255, 255};
    pencil_focus_to(UG_PENCIL_FOCUS_STAGE, __eReplot);
}

void externals_stage_end() {
    pencil_focus_to(UG_PENCIL_FOCUS_NONE, NULL);
}

void externals_stage_update() {
    memcpy(__ugBoardPressedOld, __ugBoardPressedNew, sizeof(int) * __UG_BOARD_SIZE);
    memcpy(__ugMousePressedOld, __ugMousePressedNew, sizeof(int) * __UG_MOUSE_SIZE);
}

int externals_get_button_action(bool isMouse, int key, int newState) {
    int oldState = isMouse ? __ugMousePressedOld[key] : __ugBoardPressedOld[key];
    if (isMouse) {
        __ugMousePressedNew[key] = newState;
    } else {
        __ugBoardPressedNew[key] = newState;
    }
    if (oldState != newState) {
        return newState ? UG_BUTTON_ACTION_PRESS : UG_BUTTON_ACTION_RELEASE;
    }
    return UG_BUTTON_ACTION_NONE;
}

int externals_get_button_state(bool isMouse, int key) {
    int buttonState = isMouse ? __ugMousePressedOld[key] : __ugBoardPressedOld[key];
    return buttonState;
}

#endif
