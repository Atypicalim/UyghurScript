// baord

#include "raylib.h"
#define RAYGUI_IMPLEMENTATION
#include "src/raygui.h"  
#include "styles/ashes/ashes.h"  
#include "../uyghur/uyghur.c"

// data
Bridge *uyghurBridge;
Hashmap *resourcesMap;
Font defaultFont;
Image defaultImage;
Texture2D defaultTexture;

// callback

void raylib_on_show()
{
    // SetTargetFPS(60);
    Bridge_startFunc(uyghurBridge, "doska_korsitish_qayturmisi");
    Bridge_call(uyghurBridge);
}

void raylib_on_frame()
{
    Bridge_startFunc(uyghurBridge, "doska_sizish_qayturmisi");
    Bridge_call(uyghurBridge);
    // DrawFPS(10, 10);
}

void raylib_on_focus()
{
    // RLAPI bool IsWindowFocused(void);
}

void raylib_on_resize()
{
    // RLAPI bool IsWindowResized(void); 
}

void raylib_on_drop()
{
    // RLAPI bool IsFileDropped(void);                                   // Check if a file has been dropped into window
    // RLAPI char **GetDroppedFiles(int *count);                         // Get dropped files names (memory should be freed)
    // RLAPI void ClearDroppedFiles(void); 

}

void raylib_on_hide()
{

}

// tool

void raylib_run_program(int width, int height, char *title, int mode)
{
    if (IsWindowReady()) return;
    if (width < 0) width = 500;
    if (height < 0) height = 500;
    if (strlen(title) == 0) title = "Uyghur Script!";
    if (mode < 0) mode = FLAG_WINDOW_RESIZABLE;
    SetConfigFlags(mode);
    InitWindow(width, height, title);
    InitAudioDevice();
    GuiLoadStyleAshes();
    defaultFont = GetFontDefault();
    defaultImage = GenImageGradientRadial(300, 300, 0, (Color){255, 255, 255, 50}, (Color){0, 0, 0, 50});
    defaultTexture = LoadTextureFromImage(defaultImage);
    raylib_on_show();
    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(RAYWHITE);
        raylib_on_frame();
        EndDrawing();
    }
    CloseAudioDevice();
    CloseWindow();
    raylib_on_hide();
}

// api

void ug_baord_set_log(Bridge *bridge)
{
    int level = Bridge_receiveNumber(bridge);
    SetTraceLogLevel(level);
    Bridge_returnEmpty(bridge);
}

void ug_baord_set_fps(Bridge *bridge)
{
    SetTargetFPS(Bridge_receiveNumber(bridge));
    Bridge_returnEmpty(bridge);
}

void ug_baord_get_fps(Bridge *bridge)
{
    Bridge_returnNumber(bridge, GetFPS());
}

void ug_baord_draw_fps(Bridge *bridge)
{
    int x = Bridge_receiveNumber(bridge);
    int y = Bridge_receiveNumber(bridge);
    DrawFPS(x, y);
    Bridge_returnEmpty(bridge);
}

void ug_board_show_window(Bridge *bridge)
{
    int w = Bridge_receiveNumber(bridge);
    int h = Bridge_receiveNumber(bridge);
    char *title = Bridge_receiveString(bridge);
    int mode = Bridge_receiveNumber(bridge);
    raylib_run_program(w, h, title, mode);
    Bridge_returnEmpty(bridge);
}

void ug_baord_hide_window(Bridge *bridge)
{
    if (IsWindowReady()) CloseWindow();
    Bridge_returnEmpty(bridge);
}

void ug_baord_is_fullscreen(Bridge *bridge)
{
    Bridge_returnBoolean(bridge, IsWindowFullscreen());
}

void ug_baord_is_hidden(Bridge *bridge)
{
    Bridge_returnBoolean(bridge, IsWindowHidden());
}

void ug_baord_is_minimized(Bridge *bridge)
{
    Bridge_returnBoolean(bridge, IsWindowMinimized());
}

void ug_baord_is_maximized(Bridge *bridge)
{
    Bridge_returnBoolean(bridge, IsWindowMaximized());
}

void ug_baord_toggle_fullscreen(Bridge *bridge)
{
    ToggleFullscreen();
    Bridge_returnEmpty(bridge);
}

void ug_baord_set_minimize(Bridge *bridge)
{
    MinimizeWindow();
    Bridge_returnEmpty(bridge);
}

void ug_baord_set_maximize(Bridge *bridge)
{
    MaximizeWindow();
    Bridge_returnEmpty(bridge);
}

void ug_baord_set_normalize(Bridge *bridge)
{
    RestoreWindow();
    Bridge_returnEmpty(bridge);
}

void ug_baord_set_title(Bridge *bridge)
{
    SetWindowTitle(Bridge_receiveString(bridge));
    Bridge_returnEmpty(bridge);
}

void ug_baord_set_icon(Bridge *bridge)
{
    char *path = Bridge_receiveString(bridge);
    Image image = LoadImage(path);
    SetWindowIcon(image);
    Bridge_returnEmpty(bridge);
}

void ug_baord_set_position(Bridge *bridge)
{
    int x = Bridge_receiveNumber(bridge);
    int y = Bridge_receiveNumber(bridge);
    SetWindowPosition(x, y);
    Bridge_returnEmpty(bridge);
}

void ug_baord_get_position(Bridge *bridge)
{
    Vector2 vector2 = GetWindowPosition();
    Bridge_returnNumbers(bridge, 2, vector2.x, vector2.y);
}

void ug_baord_set_size(Bridge *bridge)
{
    int w = Bridge_receiveNumber(bridge);
    int h = Bridge_receiveNumber(bridge);
    SetWindowSize(w, h); 
    Bridge_returnEmpty(bridge);
}

void ug_baord_get_size(Bridge *bridge)
{
    Bridge_returnNumbers(bridge, 2, GetScreenWidth(), GetScreenHeight());
}

void ug_baord_set_min_size(Bridge *bridge)
{
    int w = Bridge_receiveNumber(bridge);
    int h = Bridge_receiveNumber(bridge);
    SetWindowMinSize(w, h);
    Bridge_returnEmpty(bridge);
}

void ug_baord_show_cursor(Bridge *bridge)
{
    bool b = Bridge_receiveBoolean(bridge);
    if (b) ShowCursor();
    if (!b) HideCursor();
    Bridge_returnEmpty(bridge);
}

void ug_baord_enable_cursor(Bridge *bridge)
{
    bool b = Bridge_receiveBoolean(bridge);
    if (b) EnableCursor();
    if (!b) DisableCursor();
    Bridge_returnEmpty(bridge);
}

void ug_baord_have_cursor(Bridge *bridge)
{
    Bridge_returnBoolean(bridge, IsCursorOnScreen());
} 

void ug_baord_set_clipboard(Bridge *bridge)
{
    char *c = Bridge_receiveString(bridge);
    SetClipboardText(c);
    Bridge_returnEmpty(bridge);
}

void ug_baord_get_clipboard(Bridge *bridge)
{
    char *r = (char*)GetClipboardText();
    Bridge_returnString(bridge, r);
}

void ug_baord_set_mourse_cursor(Bridge *bridge)
{
    int c = Bridge_receiveNumber(bridge);
    SetMouseCursor(c);
    Bridge_returnEmpty(bridge);
}

void ug_baord_get_mouse_position(Bridge *bridge)
{
    Bridge_returnNumbers(bridge, 2, GetMouseX(), GetMouseY());
}

void ug_baord_get_mouse_wheel(Bridge *bridge)
{
    Bridge_returnNumber(bridge, GetMouseWheelMove());
}

void ug_baord_get_mouse_key_action(Bridge *bridge)
{
    int keyCode = Bridge_receiveNumber(bridge);
    int action = 0;
    if (IsMouseButtonPressed(keyCode)) action = 1;
    if (IsMouseButtonReleased(keyCode)) action = -1;
    Bridge_returnNumber(bridge, action);
}

void ug_baord_get_mouse_key_state(Bridge *bridge)
{
    int keyCode = Bridge_receiveNumber(bridge);
    int action = 0;
    if (IsMouseButtonDown(keyCode)) action = 1;
    if (IsMouseButtonUp(keyCode)) action = -1;
    Bridge_returnNumber(bridge, action);
}

void ug_baord_get_keyboard_key_action(Bridge *bridge)
{
    int keyCode = Bridge_receiveNumber(bridge);
    int action = 0;
    if (IsKeyPressed(keyCode)) action = 1;
    if (IsKeyReleased(keyCode)) action = -1;
    Bridge_returnNumber(bridge, action);
}

void ug_baord_get_keybaord_key_state(Bridge *bridge)
{
    int keyCode = Bridge_receiveNumber(bridge);
    int action = 0;
    if (IsKeyDown(keyCode)) action = 1;
    if (IsKeyUp(keyCode)) action = -1;
    Bridge_returnNumber(bridge, action);
}

void ug_baord_save_screenshot(Bridge *bridge)
{
    char *path = Bridge_receiveString(bridge);
    TakeScreenshot(path);
    Bridge_returnEmpty(bridge);
}

void ug_board_audio_set_volume(Bridge *bridge)
{
    float v = Bridge_receiveNumber(bridge);
    SetMasterVolume(v);
    Bridge_returnEmpty(bridge);
}

// other

void lib_raylib_main_register(Bridge *bridge)
{
    uyghurBridge = bridge;
    resourcesMap = Hashmap_new(NULL);
    //
    Bridge_startBox(bridge, "doska");
    // settings
    Bridge_bindNative(bridge, "logDerijisiniBikitish", ug_baord_set_log);
    Bridge_bindNative(bridge, "fpsBikitish", ug_baord_set_fps);
    Bridge_bindNative(bridge, "fpsElish", ug_baord_get_fps);
    Bridge_bindNative(bridge, "fpsSizish", ug_baord_draw_fps);
    // window
    Bridge_bindNative(bridge, "korsitish", ug_board_show_window);
    Bridge_bindNative(bridge, "yushurush", ug_baord_hide_window);
    Bridge_bindNative(bridge, "putunIkranmu", ug_baord_is_fullscreen);
    Bridge_bindNative(bridge, "yushurunghanmu", ug_baord_is_hidden);
    Bridge_bindNative(bridge, "kichiklitilgenmu", ug_baord_is_minimized);
    Bridge_bindNative(bridge, "chongaytilghanmu", ug_baord_is_maximized);
    Bridge_bindNative(bridge, "putunIkranHalitiniAlmashturush", ug_baord_toggle_fullscreen);
    Bridge_bindNative(bridge, "kichiklitish", ug_baord_set_minimize);
    Bridge_bindNative(bridge, "chongaytish", ug_baord_set_maximize);
    Bridge_bindNative(bridge, "normallashturush", ug_baord_set_normalize);
    Bridge_bindNative(bridge, "temaBikitish", ug_baord_set_title);
    Bridge_bindNative(bridge, "sibelgeBikitish", ug_baord_set_icon);
    Bridge_bindNative(bridge, "orniniBikitish", ug_baord_set_position);
    Bridge_bindNative(bridge, "orniniElish", ug_baord_get_position);
    Bridge_bindNative(bridge, "chongliqiniBikitish", ug_baord_set_size);
    Bridge_bindNative(bridge, "chongliqiniElish", ug_baord_get_size);
    Bridge_bindNative(bridge, "engKichikChongliqiniBikitish", ug_baord_set_min_size);
    Bridge_bindNative(bridge, "istirilkaKorsitish", ug_baord_show_cursor);
    Bridge_bindNative(bridge, "istirilkaQozghitish", ug_baord_enable_cursor);
    Bridge_bindNative(bridge, "istirilkaIchidimu", ug_baord_have_cursor);
    // system
    Bridge_bindNative(bridge, "chaplashTaxtisiniBikitish", ug_baord_set_clipboard);
    Bridge_bindNative(bridge, "chaplashTaxtisiniElish", ug_baord_get_clipboard);
    Bridge_bindNative(bridge, "mausIstirilkaBelgisiniBikitish", ug_baord_set_mourse_cursor);
    Bridge_bindNative(bridge, "mausIstirilkaOrniniElish", ug_baord_get_mouse_position);
    Bridge_bindNative(bridge, "mausAylanmaOrniniElish", ug_baord_get_mouse_wheel);
    Bridge_bindNative(bridge, "mausKunupkisiHerkitiniElish", ug_baord_get_mouse_key_action);
    Bridge_bindNative(bridge, "mausKunupkisiHalitiniElish", ug_baord_get_mouse_key_state);
    Bridge_bindNative(bridge, "tahtayKunupkisiHerkitiniElish", ug_baord_get_keyboard_key_action);
    Bridge_bindNative(bridge, "tahtayKunupkisiHalitiniElish", ug_baord_get_keybaord_key_state);
    // other
    Bridge_bindNative(bridge, "ikranniResimgeTartipSaxlash", ug_baord_save_screenshot);
    Bridge_bindNative(bridge, "programmaAwaziniBikitish", ug_board_audio_set_volume);
    //
    Bridge_register(bridge);
}
