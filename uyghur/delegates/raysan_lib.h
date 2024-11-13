// raysan

#ifndef RAYSAN_DELEGATE_H
#define RAYSAN_DELEGATE_H
#include "../include.h"
#include "./define.h"

//////////////////////////////////////////////////////////////

int delegate_mouse_code_swap(int code, bool fromU) {
    _UG_CODE_SWAP_RETURN(UG_MOUSE_BUTTON_CODE_LEFT, MOUSE_BUTTON_LEFT);
    _UG_CODE_SWAP_RETURN(UG_MOUSE_BUTTON_CODE_MIDDLE, MOUSE_BUTTON_MIDDLE);
    _UG_CODE_SWAP_RETURN(UG_MOUSE_BUTTON_CODE_RIGHT, MOUSE_BUTTON_RIGHT);
    return code;
}

int delegate_mouse_cursor_swap(int type, bool fromU) {
    _UG_TYPE_SWAP_RETURN(UG_MOUSE_CURSOR_TYPE_NONE, MOUSE_CURSOR_DEFAULT);
    _UG_TYPE_SWAP_RETURN(UG_MOUSE_CURSOR_TYPE_ARROW, MOUSE_CURSOR_ARROW);
    _UG_TYPE_SWAP_RETURN(UG_MOUSE_CURSOR_TYPE_HAND, MOUSE_CURSOR_POINTING_HAND);
    _UG_TYPE_SWAP_RETURN(UG_MOUSE_CURSOR_TYPE_CROSS, MOUSE_CURSOR_CROSSHAIR);
    _UG_TYPE_SWAP_RETURN(UG_MOUSE_CURSOR_TYPE_EDIT, MOUSE_CURSOR_IBEAM);
    _UG_TYPE_SWAP_RETURN(UG_MOUSE_CURSOR_TYPE_FORBID, MOUSE_CURSOR_NOT_ALLOWED);
    _UG_TYPE_SWAP_RETURN(UG_MOUSE_CURSOR_TYPE_RESIZE, MOUSE_CURSOR_RESIZE_ALL);
    return type;
}

int delegate_board_code_swap(int code, bool fromU) {
    _UG_CODE_SWAP_RETURN(UG_BOARD_KEY_NULL, KEY_NULL);
    //
    _UG_CODE_SWAP_RETURN(UG_BOARD_KEY_SPACE, KEY_SPACE);
    _UG_CODE_SWAP_RETURN(UG_BOARD_KEY_APOSTROPHE, KEY_APOSTROPHE);
    _UG_CODE_SWAP_RETURN(UG_BOARD_KEY_COMMA, KEY_COMMA);
    _UG_CODE_SWAP_RETURN(UG_BOARD_KEY_MINUS, KEY_MINUS);
    _UG_CODE_SWAP_RETURN(UG_BOARD_KEY_PERIOD, KEY_PERIOD);
    _UG_CODE_SWAP_RETURN(UG_BOARD_KEY_SLASH, KEY_SLASH);
    //
    _UG_CODE_SWAP_RETURN(UG_BOARD_KEY_0, KEY_ZERO);
    _UG_CODE_SWAP_RETURN(UG_BOARD_KEY_1, KEY_ONE);
    _UG_CODE_SWAP_RETURN(UG_BOARD_KEY_2, KEY_TWO);
    _UG_CODE_SWAP_RETURN(UG_BOARD_KEY_3, KEY_THREE);
    _UG_CODE_SWAP_RETURN(UG_BOARD_KEY_4, KEY_FOUR);
    _UG_CODE_SWAP_RETURN(UG_BOARD_KEY_5, KEY_FIVE);
    _UG_CODE_SWAP_RETURN(UG_BOARD_KEY_6, KEY_SIX);
    _UG_CODE_SWAP_RETURN(UG_BOARD_KEY_7, KEY_SEVEN);
    _UG_CODE_SWAP_RETURN(UG_BOARD_KEY_8, KEY_EIGHT);
    _UG_CODE_SWAP_RETURN(UG_BOARD_KEY_9, KEY_NINE);
    //
    _UG_CODE_SWAP_RETURN(UG_BOARD_KEY_SEMICOLON, KEY_SEMICOLON);
    _UG_CODE_SWAP_RETURN(UG_BOARD_KEY_EQUAL, KEY_EQUAL);
    //
    _UG_CODE_SWAP_RETURN(UG_BOARD_KEY_A, KEY_A);
    _UG_CODE_SWAP_RETURN(UG_BOARD_KEY_B, KEY_B);
    _UG_CODE_SWAP_RETURN(UG_BOARD_KEY_C, KEY_C);
    _UG_CODE_SWAP_RETURN(UG_BOARD_KEY_D, KEY_D);
    _UG_CODE_SWAP_RETURN(UG_BOARD_KEY_E, KEY_E);
    _UG_CODE_SWAP_RETURN(UG_BOARD_KEY_F, KEY_F);
    _UG_CODE_SWAP_RETURN(UG_BOARD_KEY_G, KEY_G);
    _UG_CODE_SWAP_RETURN(UG_BOARD_KEY_H, KEY_H);
    _UG_CODE_SWAP_RETURN(UG_BOARD_KEY_I, KEY_I);
    _UG_CODE_SWAP_RETURN(UG_BOARD_KEY_J, KEY_J);
    _UG_CODE_SWAP_RETURN(UG_BOARD_KEY_K, KEY_K);
    _UG_CODE_SWAP_RETURN(UG_BOARD_KEY_L, KEY_L);
    _UG_CODE_SWAP_RETURN(UG_BOARD_KEY_M, KEY_M);
    _UG_CODE_SWAP_RETURN(UG_BOARD_KEY_N, KEY_N);
    _UG_CODE_SWAP_RETURN(UG_BOARD_KEY_O, KEY_O);
    _UG_CODE_SWAP_RETURN(UG_BOARD_KEY_P, KEY_P);
    _UG_CODE_SWAP_RETURN(UG_BOARD_KEY_Q, KEY_Q);
    _UG_CODE_SWAP_RETURN(UG_BOARD_KEY_R, KEY_R);
    _UG_CODE_SWAP_RETURN(UG_BOARD_KEY_S, KEY_S);
    _UG_CODE_SWAP_RETURN(UG_BOARD_KEY_T, KEY_T);
    _UG_CODE_SWAP_RETURN(UG_BOARD_KEY_U, KEY_U);
    _UG_CODE_SWAP_RETURN(UG_BOARD_KEY_V, KEY_V);
    _UG_CODE_SWAP_RETURN(UG_BOARD_KEY_W, KEY_W);
    _UG_CODE_SWAP_RETURN(UG_BOARD_KEY_X, KEY_X);
    _UG_CODE_SWAP_RETURN(UG_BOARD_KEY_Y, KEY_Y);
    _UG_CODE_SWAP_RETURN(UG_BOARD_KEY_Z, KEY_Z);
    //
    _UG_CODE_SWAP_RETURN(UG_BOARD_KEY_LEFT_BRACKET, KEY_LEFT_BRACKET);
    _UG_CODE_SWAP_RETURN(UG_BOARD_KEY_BACKSLASH, KEY_BACKSLASH);
    _UG_CODE_SWAP_RETURN(UG_BOARD_KEY_RIGHT_BRACKET, KEY_RIGHT_BRACKET);
    _UG_CODE_SWAP_RETURN(UG_BOARD_KEY_GRAVE, KEY_GRAVE);
    //
    _UG_CODE_SWAP_RETURN(UG_BOARD_KEY_ESCAPE, KEY_ESCAPE);
    _UG_CODE_SWAP_RETURN(UG_BOARD_KEY_ENTER, KEY_ENTER);
    _UG_CODE_SWAP_RETURN(UG_BOARD_KEY_TAB, KEY_TAB);
    _UG_CODE_SWAP_RETURN(UG_BOARD_KEY_BACKSPACE, KEY_BACKSPACE);
    _UG_CODE_SWAP_RETURN(UG_BOARD_KEY_INSERT, KEY_INSERT);
    _UG_CODE_SWAP_RETURN(UG_BOARD_KEY_DELETE, KEY_DELETE);
    _UG_CODE_SWAP_RETURN(UG_BOARD_KEY_RIGHT, KEY_RIGHT);
    _UG_CODE_SWAP_RETURN(UG_BOARD_KEY_LEFT, KEY_LEFT);
    _UG_CODE_SWAP_RETURN(UG_BOARD_KEY_DOWN, KEY_DOWN);
    _UG_CODE_SWAP_RETURN(UG_BOARD_KEY_UP, KEY_UP);
    _UG_CODE_SWAP_RETURN(UG_BOARD_KEY_PAGE_UP, KEY_PAGE_UP);
    _UG_CODE_SWAP_RETURN(UG_BOARD_KEY_PAGE_DOWN, KEY_PAGE_DOWN);
    _UG_CODE_SWAP_RETURN(UG_BOARD_KEY_HOME, KEY_HOME);
    _UG_CODE_SWAP_RETURN(UG_BOARD_KEY_END, KEY_END);
    _UG_CODE_SWAP_RETURN(UG_BOARD_KEY_CAPS_LOCK, KEY_CAPS_LOCK);
    _UG_CODE_SWAP_RETURN(UG_BOARD_KEY_NUM_LOCK, KEY_NUM_LOCK);
    //
    _UG_CODE_SWAP_RETURN(UG_BOARD_KEY_F1, KEY_F1);
    _UG_CODE_SWAP_RETURN(UG_BOARD_KEY_F2, KEY_F2);
    _UG_CODE_SWAP_RETURN(UG_BOARD_KEY_F3, KEY_F3);
    _UG_CODE_SWAP_RETURN(UG_BOARD_KEY_F4, KEY_F4);
    _UG_CODE_SWAP_RETURN(UG_BOARD_KEY_F5, KEY_F5);
    _UG_CODE_SWAP_RETURN(UG_BOARD_KEY_F6, KEY_F6);
    _UG_CODE_SWAP_RETURN(UG_BOARD_KEY_F7, KEY_F7);
    _UG_CODE_SWAP_RETURN(UG_BOARD_KEY_F8, KEY_F8);
    _UG_CODE_SWAP_RETURN(UG_BOARD_KEY_F9, KEY_F9);
    _UG_CODE_SWAP_RETURN(UG_BOARD_KEY_F10, KEY_F10);
    _UG_CODE_SWAP_RETURN(UG_BOARD_KEY_F11, KEY_F11);
    _UG_CODE_SWAP_RETURN(UG_BOARD_KEY_F12, KEY_F12);
    //
    _UG_CODE_SWAP_RETURN(UG_BOARD_PAD_0, KEY_KP_0);
    _UG_CODE_SWAP_RETURN(UG_BOARD_PAD_1, KEY_KP_1);
    _UG_CODE_SWAP_RETURN(UG_BOARD_PAD_2, KEY_KP_2);
    _UG_CODE_SWAP_RETURN(UG_BOARD_PAD_3, KEY_KP_3);
    _UG_CODE_SWAP_RETURN(UG_BOARD_PAD_4, KEY_KP_4);
    _UG_CODE_SWAP_RETURN(UG_BOARD_PAD_5, KEY_KP_5);
    _UG_CODE_SWAP_RETURN(UG_BOARD_PAD_6, KEY_KP_6);
    _UG_CODE_SWAP_RETURN(UG_BOARD_PAD_7, KEY_KP_7);
    _UG_CODE_SWAP_RETURN(UG_BOARD_PAD_8, KEY_KP_8);
    _UG_CODE_SWAP_RETURN(UG_BOARD_PAD_9, KEY_KP_9);
    //
    _UG_CODE_SWAP_RETURN(UG_BOARD_PAD_DECIMAL, KEY_KP_DECIMAL);
    _UG_CODE_SWAP_RETURN(UG_BOARD_PAD_DIV, KEY_KP_DIVIDE);
    _UG_CODE_SWAP_RETURN(UG_BOARD_PAD_MUL, KEY_KP_MULTIPLY);
    _UG_CODE_SWAP_RETURN(UG_BOARD_PAD_SUB, KEY_KP_SUBTRACT);
    _UG_CODE_SWAP_RETURN(UG_BOARD_PAD_ADD, KEY_KP_ADD);
    _UG_CODE_SWAP_RETURN(UG_BOARD_PAD_ENTER, KEY_KP_ENTER);
    //
    _UG_CODE_SWAP_RETURN(UG_BOARD_KEY_LEFT_SHIFT, KEY_LEFT_SHIFT);
    _UG_CODE_SWAP_RETURN(UG_BOARD_KEY_LEFT_CONTROL, KEY_LEFT_CONTROL);
    _UG_CODE_SWAP_RETURN(UG_BOARD_KEY_LEFT_ALT, KEY_LEFT_ALT);
    _UG_CODE_SWAP_RETURN(UG_BOARD_KEY_LEFT_SUPER, KEY_LEFT_SUPER);
    _UG_CODE_SWAP_RETURN(UG_BOARD_KEY_RIGHT_SHIFT, KEY_RIGHT_SHIFT);
    _UG_CODE_SWAP_RETURN(UG_BOARD_KEY_RIGHT_CONTROL, KEY_RIGHT_CONTROL);
    _UG_CODE_SWAP_RETURN(UG_BOARD_KEY_RIGHT_ALT, KEY_RIGHT_ALT);
    _UG_CODE_SWAP_RETURN(UG_BOARD_KEY_RIGHT_SUPER, KEY_RIGHT_SUPER);

    return code;
}

//////////////////////////////////////////////////////////////

// audio
#define RSound Sound
#define RMusic Music
// 
#define raudio_InitAudioDevice InitAudioDevice
#define raudio_CloseAudioDevice CloseAudioDevice
#define raudio_IsAudioDeviceReady IsAudioDeviceReady
//
#define raudio_LoadSound LoadSound
#define raudio_IsSoundReady IsSoundReady
#define raudio_UpdateSound UpdateSound
#define raudio_UnloadSound UnloadSound
//
#define raudio_LoadMusic LoadMusicStream
#define raudio_IsMusicReady IsMusicReady
#define raudio_UpdateMusic UpdateMusicStream
#define raudio_UnloadMusic UnloadMusicStream
//
#define raudio_PlaySound PlaySound
#define raudio_StopSound StopSound
#define raudio_PauseSound PauseSound
#define raudio_ResumeSound ResumeSound
#define raudio_IsSoundPlaying IsSoundPlaying
#define raudio_SetSoundVolume SetSoundVolume
//
#define raudio_PlayMusic PlayMusicStream
#define raudio_StopMusic StopMusicStream
#define raudio_PauseMusic PauseMusicStream
#define raudio_ResumeMusic ResumeMusicStream
#define raudio_IsMusicPlaying IsMusicStreamPlaying
#define raudio_SetMusicVolume SetMusicVolume
#define raudio_GetMusicLength GetMusicTimeLength
#define raudio_GetMusicTime GetMusicTimePlayed
#define raudio_SetMusicTime SeekMusicStream
//
#define raudio_SetSoundPitch SetSoundPitch
#define raudio_SetSoundPan SetSoundPan
//
#define raudio_SetMusicPitch SetMusicPitch
#define raudio_SetMusicPan SetMusicPan

//////////////////////////////////////////////////////////////

// data
Font defaultFont;
Image defaultImage;
Texture2D defaultTexture;

#define STAGE_IS_FULLSCREEN IsWindowFullscreen()
#define STAGE_IS_HIDDEN IsWindowHidden()
#define STAGE_IS_MINIMIZED IsWindowMinimized()
#define STAGE_IS_MAXIMIZED IsWindowMaximized()

#define STAGE_SET_MINIMIZE MinimizeWindow();
#define STAGE_SET_MAXIMIZE MaximizeWindow();
#define STAGE_SET_NORMALIZE RestoreWindow();

void delegate_stage_set_fps(int fps) {
    SetTargetFPS(fps);
}

int delegate_stage_get_fps() {
    return GetFPS();
}

void stage_toggle_fullscreen() {
    ToggleFullscreen();
}

void stage_set_title(char *title) {
    SetWindowTitle(title);
}

void stage_set_icon(char *path) {
    Image image = LoadImage(path);
    SetWindowIcon(image);
}

void stage_set_position(int x, int y) {
    SetWindowPosition(x, y);
}

void stage_get_position(int *x, int *y) {
    Vector2 vector2 = GetWindowPosition();
    *x = vector2.x;
    *y = vector2.y;
}


void stage_set_size(int w, int h) {
    SetWindowSize(w, h);
}

void stage_get_size(int *w, int *h) {
    *w = GetScreenWidth();
    *h = GetScreenHeight();
}

void delegate_set_clipboard(CString text) {
    SetClipboardText(text);
}

CString *delegate_get_clipboard() {
    return (CString)GetClipboardText();
}

void delegate_set_mouse_visible(bool b) {
    if (b) ShowCursor();
    if (!b) HideCursor();
}

void delegate_set_mouse_cursor(int typ) {
    SetMouseCursor(typ);
}

void delegate_set_mouse_location(int x, int y) {
    SetMousePosition(x, y);
}

void delegate_get_mouse_location(int *x, int *y) {
    *x = GetMouseX();
    *y = GetMouseY();
}

double delegate_get_mouse_wheel() {
    return (double)GetMouseWheelMove();
}

int delegate_get_mouse_action(int key) {
    if (IsMouseButtonPressed(key)) return UG_BUTTON_ACTION_PRESS;
    if (IsMouseButtonReleased(key)) return UG_BUTTON_ACTION_RELEASE;
    return UG_BUTTON_ACTION_NONE;
}

int delegate_get_mouse_state(int key) {
    if (IsMouseButtonDown(key)) return UG_BUTTON_STATE_DOWN;
    return UG_BUTTON_STATE_UP;
}

int delegate_get_board_action(int key) {
    if (IsKeyPressed(key)) return UG_BUTTON_ACTION_PRESS;
    if (IsKeyReleased(key)) return UG_BUTTON_ACTION_RELEASE;
    return UG_BUTTON_ACTION_NONE;
}

int delegate_get_board_state(int key) {
    if (IsKeyDown(key)) return UG_BUTTON_STATE_DOWN;
    return UG_BUTTON_STATE_UP;
}

void delegate_stage_save_screenshot(CString path) {
    TakeScreenshot(path);
}

void delegate_stage_run_program(int width, int height, CString title, int mode) {
    if (IsWindowReady()) return;
    if (width < 0) width = 500;
    if (height < 0) height = 500;
    if (strlen(title) == 0) title = UG_PROJECT_NAME;
    if (mode < 0) mode = FLAG_WINDOW_RESIZABLE | FLAG_WINDOW_TOPMOST; // FLAG_WINDOW_TRANSPARENT
    SetConfigFlags(mode);
    int level = UG_IS_DEVELOP ? LOG_ALL : LOG_WARNING;
    SetTraceLogLevel(level);
    InitWindow(width, height, title);
    // GuiLoadStyleAshes();
    defaultFont = GetFontDefault();
    defaultImage = GenImageGradientRadial(300, 300, 0, (Color){255, 255, 255, 50}, (Color){0, 0, 0, 50});
    defaultTexture = LoadTextureFromImage(defaultImage);
    // SetTargetFPS(60);
}

bool delegate_stage_update_program() {   
    if (IsWindowReady()) {
        DrawFPS(10, 10);
        EndDrawing();
        BeginDrawing();
        ClearBackground(BLACK);
    }
    //
    bool closeable = WindowShouldClose();
    Value *value = Value_newBoolean(closeable, NULL);
    helper_set_proxy_value(ALIAS_stage, ALIAS_stage_is_closeable, value);
    //
    if (closeable && IsWindowReady()) {
        CloseWindow();
        return true;
    } else {
        return false;
    }
}

#endif
