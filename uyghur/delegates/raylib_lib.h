// raylib

#include "../include.h"

// audio
#define RSound Sound
#define RMusic Music
// 
#define raudio_InitAudioDevice InitAudioDevice
#define raudio_CloseAudioDevice CloseAudioDevice
#define raudio_IsAudioDeviceReady IsAudioDeviceReady
#define raudio_SetMasterVolume SetMasterVolume
#define raudio_GetMasterVolume GetMasterVolume
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

int delegate_draw_debug(int x, int y) {
    DrawFPS(x, y);
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

void delegate_set_min_size(int w, int h) {
    SetWindowMinSize(w, h);
}

void delegate_show_cursor(bool b) {
    if (b) ShowCursor();
    if (!b) HideCursor();
}

bool delegate_have_cursor() {
    return IsCursorOnScreen();
}

void delegate_set_clipboard(CString text) {
    SetClipboardText(text);
}

CString *delegate_get_clipboard() {
    return (CString)GetClipboardText();
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

int delegate_was_mouse_pressed(int key) {
    int action = 0;
    if (IsMouseButtonPressed(key)) action = 1;
    if (IsMouseButtonReleased(key)) action = -1;
    return action;
}

int delegate_is_mouse_pressed(int key) {
    int action = 0;
    if (IsMouseButtonDown(key)) action = 1;
    if (IsMouseButtonUp(key)) action = -1;
    return action;
}

int delegate_was_key_pressed(int key) {
    int action = 0;
    if (IsKeyPressed(key)) action = 1;
    if (IsKeyReleased(key)) action = -1;
    return action;
}

int delegate_is_key_pressed(int key) {
    int action = 0;
    if (IsKeyDown(key)) action = 1;
    if (IsKeyUp(key)) action = -1;
    return action;
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
