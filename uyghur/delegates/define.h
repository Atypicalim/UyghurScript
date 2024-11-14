// delegate define

#ifndef UG_DELEGATE_DEFINE
#define UG_DELEGATE_DEFINE

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

#endif
