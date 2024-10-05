// stage

#include "raylib.h"
#define RAYGUI_IMPLEMENTATION
#include "src/raygui.h"  
#include "styles/ashes/ashes.h"  
#include "../uyghur.c"

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
    Bridge_startFunc(uyghurBridge);
    Bridge_call(uyghurBridge, ALIAS_STAGE_ON_SHOW);
}

void raylib_on_draw()
{
    Bridge_startFunc(uyghurBridge);
    Bridge_call(uyghurBridge, ALIAS_STAGE_ON_DRAW);
    DrawFPS(10, 10);
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

void raylib_run_program(int width, int height, USTRING title, int mode)
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
        raylib_on_draw();
        EndDrawing();
    }
    CloseAudioDevice();
    CloseWindow();
    raylib_on_hide();
}

// api

void native_stage_set_log(Bridge *bridge)
{
    int level = Bridge_receiveNumber(bridge);
    SetTraceLogLevel(level);
    Bridge_returnEmpty(bridge);
}

void native_stage_set_fps(Bridge *bridge)
{
    SetTargetFPS(Bridge_receiveNumber(bridge));
    Bridge_returnEmpty(bridge);
}

void native_stage_get_fps(Bridge *bridge)
{
    Bridge_returnNumber(bridge, GetFPS());
}

void native_stage_draw_fps(Bridge *bridge)
{
    int x = Bridge_receiveNumber(bridge);
    int y = Bridge_receiveNumber(bridge);
    DrawFPS(x, y);
    Bridge_returnEmpty(bridge);
}

void native_stage_show_window(Bridge *bridge)
{
    int w = Bridge_receiveNumber(bridge);
    int h = Bridge_receiveNumber(bridge);
    USTRING title = Bridge_receiveString(bridge);
    int mode = Bridge_receiveNumber(bridge);
    raylib_run_program(w, h, title, mode);
    Bridge_returnEmpty(bridge);
}

void native_stage_hide_window(Bridge *bridge)
{
    if (IsWindowReady()) CloseWindow();
    Bridge_returnEmpty(bridge);
}

void native_stage_is_fullscreen(Bridge *bridge)
{
    Bridge_returnBoolean(bridge, IsWindowFullscreen());
}

void native_stage_is_hidden(Bridge *bridge)
{
    Bridge_returnBoolean(bridge, IsWindowHidden());
}

void native_stage_is_minimized(Bridge *bridge)
{
    Bridge_returnBoolean(bridge, IsWindowMinimized());
}

void native_stage_is_maximized(Bridge *bridge)
{
    Bridge_returnBoolean(bridge, IsWindowMaximized());
}

void native_stage_toggle_fullscreen(Bridge *bridge)
{
    ToggleFullscreen();
    Bridge_returnEmpty(bridge);
}

void native_stage_set_minimize(Bridge *bridge)
{
    MinimizeWindow();
    Bridge_returnEmpty(bridge);
}

void native_stage_set_maximize(Bridge *bridge)
{
    MaximizeWindow();
    Bridge_returnEmpty(bridge);
}

void native_stage_set_normalize(Bridge *bridge)
{
    RestoreWindow();
    Bridge_returnEmpty(bridge);
}

void native_stage_set_title(Bridge *bridge)
{
    SetWindowTitle(Bridge_receiveString(bridge));
    Bridge_returnEmpty(bridge);
}

void native_stage_set_icon(Bridge *bridge)
{
    USTRING path = Bridge_receiveString(bridge);
    Image image = LoadImage(path);
    SetWindowIcon(image);
    Bridge_returnEmpty(bridge);
}

void native_stage_set_position(Bridge *bridge)
{
    int x = Bridge_receiveNumber(bridge);
    int y = Bridge_receiveNumber(bridge);
    SetWindowPosition(x, y);
    Bridge_returnEmpty(bridge);
}

void native_stage_get_position(Bridge *bridge)
{
    Vector2 vector2 = GetWindowPosition();
    Bridge_returnNumbers(bridge, 2, vector2.x, vector2.y);
}

void native_stage_set_size(Bridge *bridge)
{
    int w = Bridge_receiveNumber(bridge);
    int h = Bridge_receiveNumber(bridge);
    SetWindowSize(w, h); 
    Bridge_returnEmpty(bridge);
}

void native_stage_get_size(Bridge *bridge)
{
    Bridge_returnNumbers(bridge, 2, GetScreenWidth(), GetScreenHeight());
}

void native_stage_set_min_size(Bridge *bridge)
{
    int w = Bridge_receiveNumber(bridge);
    int h = Bridge_receiveNumber(bridge);
    SetWindowMinSize(w, h);
    Bridge_returnEmpty(bridge);
}

void native_stage_show_cursor(Bridge *bridge)
{
    bool b = Bridge_receiveBoolean(bridge);
    if (b) ShowCursor();
    if (!b) HideCursor();
    Bridge_returnEmpty(bridge);
}

void native_stage_enable_cursor(Bridge *bridge)
{
    bool b = Bridge_receiveBoolean(bridge);
    if (b) EnableCursor();
    if (!b) DisableCursor();
    Bridge_returnEmpty(bridge);
}

void native_stage_have_cursor(Bridge *bridge)
{
    Bridge_returnBoolean(bridge, IsCursorOnScreen());
} 

void native_stage_set_clipboard(Bridge *bridge)
{
    USTRING c = Bridge_receiveString(bridge);
    SetClipboardText(c);
    Bridge_returnEmpty(bridge);
}

void native_stage_get_clipboard(Bridge *bridge)
{
    USTRING r = (USTRING)GetClipboardText();
    Bridge_returnString(bridge, r);
}

void native_stage_set_mouse_cursor(Bridge *bridge)
{
    int c = Bridge_receiveNumber(bridge);
    SetMouseCursor(c);
    Bridge_returnEmpty(bridge);
}

void native_stage_get_mouse_position(Bridge *bridge)
{
    Bridge_returnNumbers(bridge, 2, GetMouseX(), GetMouseY());
}

void native_stage_get_mouse_wheel(Bridge *bridge)
{
    Bridge_returnNumber(bridge, GetMouseWheelMove());
}

void native_stage_get_mouse_key_action(Bridge *bridge)
{
    int keyCode = Bridge_receiveNumber(bridge);
    int action = 0;
    if (IsMouseButtonPressed(keyCode)) action = 1;
    if (IsMouseButtonReleased(keyCode)) action = -1;
    Bridge_returnNumber(bridge, action);
}

void native_stage_get_mouse_key_state(Bridge *bridge)
{
    int keyCode = Bridge_receiveNumber(bridge);
    int action = 0;
    if (IsMouseButtonDown(keyCode)) action = 1;
    if (IsMouseButtonUp(keyCode)) action = -1;
    Bridge_returnNumber(bridge, action);
}

void native_stage_get_keyboard_key_action(Bridge *bridge)
{
    int keyCode = Bridge_receiveNumber(bridge);
    int action = 0;
    if (IsKeyPressed(keyCode)) action = 1;
    if (IsKeyReleased(keyCode)) action = -1;
    Bridge_returnNumber(bridge, action);
}

void native_stage_get_keyboard_key_state(Bridge *bridge)
{
    int keyCode = Bridge_receiveNumber(bridge);
    int action = 0;
    if (IsKeyDown(keyCode)) action = 1;
    if (IsKeyUp(keyCode)) action = -1;
    Bridge_returnNumber(bridge, action);
}

void native_stage_save_screenshot(Bridge *bridge)
{
    USTRING path = Bridge_receiveString(bridge);
    TakeScreenshot(path);
    Bridge_returnEmpty(bridge);
}

void native_stage_audio_set_volume(Bridge *bridge)
{
    float v = Bridge_receiveNumber(bridge);
    SetMasterVolume(v);
    Bridge_returnEmpty(bridge);
}

// other

void lib_stage_register(Bridge *bridge)
{
    uyghurBridge = bridge;
    resourcesMap = Hashmap_new(false);
    //
    Bridge_startBox(bridge);
    // settings
    BRIDGE_BIND_NATIVE(stage_set_log);
    BRIDGE_BIND_NATIVE(stage_set_fps);
    BRIDGE_BIND_NATIVE(stage_get_fps);
    BRIDGE_BIND_NATIVE(stage_draw_fps);
    // window
    BRIDGE_BIND_NATIVE(stage_show_window);
    BRIDGE_BIND_NATIVE(stage_hide_window);
    BRIDGE_BIND_NATIVE(stage_is_fullscreen);
    BRIDGE_BIND_NATIVE(stage_is_hidden);
    BRIDGE_BIND_NATIVE(stage_is_minimized);
    BRIDGE_BIND_NATIVE(stage_is_maximized);
    BRIDGE_BIND_NATIVE(stage_toggle_fullscreen);
    BRIDGE_BIND_NATIVE(stage_set_minimize);
    BRIDGE_BIND_NATIVE(stage_set_maximize);
    BRIDGE_BIND_NATIVE(stage_set_normalize);
    BRIDGE_BIND_NATIVE(stage_set_title);
    BRIDGE_BIND_NATIVE(stage_set_icon);
    BRIDGE_BIND_NATIVE(stage_set_position);
    BRIDGE_BIND_NATIVE(stage_get_position);
    BRIDGE_BIND_NATIVE(stage_set_size);
    BRIDGE_BIND_NATIVE(stage_get_size);
    BRIDGE_BIND_NATIVE(stage_set_min_size);
    BRIDGE_BIND_NATIVE(stage_show_cursor);
    BRIDGE_BIND_NATIVE(stage_enable_cursor);
    BRIDGE_BIND_NATIVE(stage_have_cursor);
    // system
    BRIDGE_BIND_NATIVE(stage_set_clipboard);
    BRIDGE_BIND_NATIVE(stage_get_clipboard);
    BRIDGE_BIND_NATIVE(stage_set_mouse_cursor);
    BRIDGE_BIND_NATIVE(stage_get_mouse_position);
    BRIDGE_BIND_NATIVE(stage_get_mouse_wheel);
    BRIDGE_BIND_NATIVE(stage_get_mouse_key_action);
    BRIDGE_BIND_NATIVE(stage_get_mouse_key_state);
    BRIDGE_BIND_NATIVE(stage_get_keyboard_key_action);
    BRIDGE_BIND_NATIVE(stage_get_keyboard_key_state);
    // other
    BRIDGE_BIND_NATIVE(stage_save_screenshot);
    BRIDGE_BIND_NATIVE(stage_audio_set_volume);
    //
    Bridge_register(bridge, ALIAS_STAGE);
}
