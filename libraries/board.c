// baord

#include "raylib.h"
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
    Bridge_startFunc(uyghurBridge, "doska_korsitish_qayturmisi");
    Bridge_call(uyghurBridge);
}

void raylib_on_frame()
{
    Bridge_startFunc(uyghurBridge, "doska_sizish_qayturmisi");
    Bridge_call(uyghurBridge);
    //
    // Vector2 center = (Vector2){250, 250};
    // Color color = (Color){50, 100, 200, 255};
    Rectangle rectangle = (Rectangle){250, 250, 200, 200};
    Vector2 anchor = (Vector2){100, 100};
    //
    DrawFPS(400, 450);


    // Image img = LoadImage("../resources/rose.png");             // Load image in CPU memory (RAM)
    // Image img = raylib_load_image("../resources/rose.png");
    // // // Image img = raylib_load_image("../resources/gift.png");
    // img = ImageFromImage(img, (Rectangle){0, 0, 300, 300});
    // int w = img.width;
    // int h = img.height;


    
    // Texture texture = LoadTextureFromImage(img);
    // DrawTextureEx(texture, (Vector2){100, 100}, 0, 1, WHITE);
    // DrawTextureRec(texture, (Rectangle){0, 150, 300, 150}, (Vector2){0, 0}, WHITE);
    // DrawRectangle(100, 100, 200, 200, GREEN); 
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
    int level = Bridge_nexNumber(bridge);
    SetTraceLogLevel(level);
    Bridge_startResult(bridge);
    Bridge_return(bridge);
}

void ug_baord_set_fps(Bridge *bridge)
{
    SetTargetFPS(Bridge_nexNumber(bridge));
    Bridge_startResult(bridge);
    Bridge_return(bridge);
}

void ug_baord_get_fps(Bridge *bridge)
{
    Bridge_startResult(bridge);
    Bridge_pushNumber(bridge, GetFPS());
    Bridge_return(bridge);
}

void ug_baord_draw_fps(Bridge *bridge)
{
    int x = Bridge_nexNumber(bridge);
    int y = Bridge_nexNumber(bridge);
    DrawFPS(x, y);
    Bridge_startResult(bridge);
    Bridge_return(bridge);
}

void ug_board_show_window(Bridge *bridge)
{
    int w = Bridge_nexNumber(bridge);
    int h = Bridge_nexNumber(bridge);
    char *title = Bridge_nexString(bridge);
    int mode = Bridge_nexNumber(bridge);
    raylib_run_program(w, h, title, mode);
    Bridge_startResult(bridge);
    Bridge_return(bridge);
}

void ug_baord_hide_window(Bridge *bridge)
{
    if (IsWindowReady()) CloseWindow();
    Bridge_startResult(bridge);
    Bridge_return(bridge);
}

void ug_baord_is_fullscreen(Bridge *bridge)
{
    Bridge_startResult(bridge);
    Bridge_pushBoolean(bridge, IsWindowFullscreen());
    Bridge_return(bridge);
}

void ug_baord_is_hidden(Bridge *bridge)
{
    Bridge_startResult(bridge);
    Bridge_pushBoolean(bridge, IsWindowHidden());
    Bridge_return(bridge);
}

void ug_baord_is_minimized(Bridge *bridge)
{
    Bridge_startResult(bridge);
    Bridge_pushBoolean(bridge, IsWindowMinimized());
    Bridge_return(bridge);
}

void ug_baord_is_maximized(Bridge *bridge)
{
    Bridge_startResult(bridge);
    Bridge_pushBoolean(bridge, IsWindowMaximized());
    Bridge_return(bridge);
}

void ug_baord_toggle_fullscreen(Bridge *bridge)
{
    ToggleFullscreen();
    Bridge_startResult(bridge);
    Bridge_return(bridge);
}

void ug_baord_set_minimize(Bridge *bridge)
{
    MinimizeWindow();
    Bridge_startResult(bridge);
    Bridge_return(bridge);
}

void ug_baord_set_maximize(Bridge *bridge)
{
    MaximizeWindow();
    Bridge_startResult(bridge);
    Bridge_return(bridge);
}

void ug_baord_set_normalize(Bridge *bridge)
{
    RestoreWindow();
    Bridge_startResult(bridge);
    Bridge_return(bridge);
}

void ug_baord_set_title(Bridge *bridge)
{
    SetWindowTitle(Bridge_nexString(bridge));
    Bridge_startResult(bridge);
    Bridge_return(bridge);
}

void ug_baord_set_icon(Bridge *bridge)
{
    char *path = Bridge_nexString(bridge);
    Image image = LoadImage(path);
    SetWindowIcon(image);
    Bridge_startResult(bridge);
    Bridge_return(bridge);
}

void ug_baord_set_position(Bridge *bridge)
{
    int x = Bridge_nexNumber(bridge);
    int y = Bridge_nexNumber(bridge);
    SetWindowPosition(x, y); 
    Bridge_startResult(bridge);
    Bridge_return(bridge);
}

void ug_baord_get_position(Bridge *bridge)
{
    Vector2 vector2 = GetWindowPosition();
    Bridge_startResult(bridge);
    Bridge_pushNumber(bridge, vector2.x);
    Bridge_pushNumber(bridge, vector2.y);
    Bridge_return(bridge);
}

void ug_baord_set_size(Bridge *bridge)
{
    int w = Bridge_nexNumber(bridge);
    int h = Bridge_nexNumber(bridge);
    SetWindowSize(w, h); 
    Bridge_startResult(bridge);
    Bridge_return(bridge);
}

void ug_baord_get_size(Bridge *bridge)
{
    Bridge_startResult(bridge);
    Bridge_pushNumber(bridge, GetScreenWidth());
    Bridge_pushNumber(bridge, GetScreenHeight());
    Bridge_return(bridge);
}

void ug_baord_get_screen_size(Bridge *bridge)
{
    int w = GetScreenWidth();
    int h = GetScreenHeight();
    Bridge_startResult(bridge);
    Bridge_pushNumber(bridge, w);
    Bridge_pushNumber(bridge, h);
    Bridge_return(bridge);
}

void ug_baord_set_min_size(Bridge *bridge)
{
    int w = Bridge_nexNumber(bridge);
    int h = Bridge_nexNumber(bridge);
    SetWindowMinSize(w, h); 
    Bridge_startResult(bridge);
    Bridge_return(bridge);
}

void ug_baord_show_cursor(Bridge *bridge)
{
    bool b = Bridge_nexBoolean(bridge);
    if (b) ShowCursor();
    if (!b) HideCursor();
    Bridge_startResult(bridge);
    Bridge_return(bridge);
}

void ug_baord_enable_cursor(Bridge *bridge)
{
    bool b = Bridge_nexBoolean(bridge);
    if (b) EnableCursor();
    if (!b) DisableCursor();
    Bridge_startResult(bridge);
    Bridge_return(bridge);
}

void ug_baord_have_cursor(Bridge *bridge)
{
    Bridge_startResult(bridge);
    Bridge_pushBoolean(bridge, IsCursorOnScreen());
    Bridge_return(bridge);
} 

void ug_baord_set_clipboard(Bridge *bridge)
{
    char *c = Bridge_nexString(bridge);
    SetClipboardText(c);
    Bridge_startResult(bridge);
    Bridge_return(bridge);
}

void ug_baord_get_clipboard(Bridge *bridge)
{
    char *r = (char*)GetClipboardText();
    Bridge_startResult(bridge);
    Bridge_pushString(bridge, r);
    Bridge_return(bridge);
}

void ug_baord_set_mourse_cursor(Bridge *bridge)
{
    SetMouseCursor(Bridge_nexNumber(bridge));
    Bridge_startResult(bridge);
    Bridge_return(bridge);
}

void ug_baord_get_mouse_position(Bridge *bridge)
{
    Bridge_startResult(bridge);
    Bridge_pushNumber(bridge, GetMouseX());
    Bridge_pushNumber(bridge, GetMouseY());
    Bridge_return(bridge);
}

void ug_baord_get_mouse_wheel(Bridge *bridge)
{
    Bridge_startResult(bridge);
    Bridge_pushNumber(bridge, GetMouseWheelMove());
    Bridge_return(bridge);
}

void ug_baord_get_mouse_key_action(Bridge *bridge)
{
    int keyCode = Bridge_nexNumber(bridge);
    int action = 0;
    if (IsMouseButtonPressed(keyCode)) action = 1;
    if (IsMouseButtonReleased(keyCode)) action = -1;
    Bridge_startResult(bridge);
    Bridge_pushNumber(bridge, action);
    Bridge_return(bridge);
}

void ug_baord_get_mouse_key_state(Bridge *bridge)
{
    int keyCode = Bridge_nexNumber(bridge);
    int action = 0;
    if (IsMouseButtonDown(keyCode)) action = 1;
    if (IsMouseButtonUp(keyCode)) action = -1;
    Bridge_startResult(bridge);
    Bridge_pushNumber(bridge, action);
    Bridge_return(bridge);
}

void ug_baord_get_keyboard_key_action(Bridge *bridge)
{
    int keyCode = Bridge_nexNumber(bridge);
    int action = 0;
    if (IsKeyPressed(keyCode)) action = 1;
    if (IsKeyReleased(keyCode)) action = -1;
    Bridge_startResult(bridge);
    Bridge_pushNumber(bridge, action);
    Bridge_return(bridge);
}

void ug_baord_get_keybaord_key_state(Bridge *bridge)
{
    int keyCode = Bridge_nexNumber(bridge);
    int action = 0;
    if (IsKeyDown(keyCode)) action = 1;
    if (IsKeyUp(keyCode)) action = -1;
    Bridge_startResult(bridge);
    Bridge_pushNumber(bridge, action);
    Bridge_return(bridge);
}

void ug_baord_save_screenshot(Bridge *bridge)
{
    char *path = Bridge_nexString(bridge);
    TakeScreenshot(path);
    Bridge_startResult(bridge);
    Bridge_return(bridge);
}

void ug_board_audio_set_volume(Bridge *bridge)
{
    float v = Bridge_nexNumber(bridge);
    SetMasterVolume(v);
    Bridge_startResult(bridge);
    Bridge_return(bridge);
}

// other

void lib_raylib_main_register(Bridge *bridge)
{
    uyghurBridge = bridge;
    resourcesMap = Hashmap_new(NULL);
    //
    Bridge_startBox(bridge, "doska");
    // settings
    Bridge_pushKey(bridge, "logDerijisiniBikitish");
    Bridge_pushFunction(bridge, ug_baord_set_log);
    Bridge_pushKey(bridge, "fpsBikitish");
    Bridge_pushFunction(bridge, ug_baord_set_fps);
    Bridge_pushKey(bridge, "fpsElish");
    Bridge_pushFunction(bridge, ug_baord_get_fps);
    Bridge_pushKey(bridge, "fpsSizish");
    Bridge_pushFunction(bridge, ug_baord_draw_fps);
    // window
    Bridge_pushKey(bridge, "korsitish");
    Bridge_pushFunction(bridge, ug_board_show_window);
    Bridge_pushKey(bridge, "yushurush");
    Bridge_pushFunction(bridge, ug_baord_hide_window);
    Bridge_pushKey(bridge, "putunIkranmu");
    Bridge_pushFunction(bridge, ug_baord_is_fullscreen);
    Bridge_pushKey(bridge, "yushurunghanmu");
    Bridge_pushFunction(bridge, ug_baord_is_hidden);
    Bridge_pushKey(bridge, "kichiklitilgenmu");
    Bridge_pushFunction(bridge, ug_baord_is_minimized);
    Bridge_pushKey(bridge, "chongaytilghanmu");
    Bridge_pushFunction(bridge, ug_baord_is_maximized);
    Bridge_pushKey(bridge, "putunIkranHalitiniAlmashturush");
    Bridge_pushFunction(bridge, ug_baord_toggle_fullscreen);
    Bridge_pushKey(bridge, "kichiklitish");
    Bridge_pushFunction(bridge, ug_baord_set_minimize);
    Bridge_pushKey(bridge, "chongaytish");
    Bridge_pushFunction(bridge, ug_baord_set_maximize);
    Bridge_pushKey(bridge, "normallashturush");
    Bridge_pushFunction(bridge, ug_baord_set_normalize);
    Bridge_pushKey(bridge, "temaBikitish");
    Bridge_pushFunction(bridge, ug_baord_set_title);
    Bridge_pushKey(bridge, "sibelgeBikitish");
    Bridge_pushFunction(bridge, ug_baord_set_icon);
    Bridge_pushKey(bridge, "orniniBikitish");
    Bridge_pushFunction(bridge, ug_baord_set_position);
    Bridge_pushKey(bridge, "orniniElish");
    Bridge_pushFunction(bridge, ug_baord_get_position);
    Bridge_pushKey(bridge, "chongliqiniBikitish");
    Bridge_pushFunction(bridge, ug_baord_set_size);
    Bridge_pushKey(bridge, "chongliqiniElish");
    Bridge_pushFunction(bridge, ug_baord_get_size);
    Bridge_pushKey(bridge, "ikranChongliqiniElish");
    Bridge_pushFunction(bridge, ug_baord_get_screen_size);
    Bridge_pushKey(bridge, "engKichikChongliqiniBikitish");
    Bridge_pushFunction(bridge, ug_baord_set_min_size);
    Bridge_pushKey(bridge, "istirilkaKorsitish");
    Bridge_pushFunction(bridge, ug_baord_show_cursor);
    Bridge_pushKey(bridge, "istirilkaQozghitish");
    Bridge_pushFunction(bridge, ug_baord_enable_cursor);
    Bridge_pushKey(bridge, "istirilkaIchidimu");
    Bridge_pushFunction(bridge, ug_baord_have_cursor);
    // system
    Bridge_pushKey(bridge, "chaplashTaxtisiniBikitish");
    Bridge_pushFunction(bridge, ug_baord_set_clipboard);
    Bridge_pushKey(bridge, "chaplashTaxtisiniElish");
    Bridge_pushFunction(bridge, ug_baord_get_clipboard);
    Bridge_pushKey(bridge, "mausIstirilkaBelgisiniBikitish");
    Bridge_pushFunction(bridge, ug_baord_set_mourse_cursor);
    Bridge_pushKey(bridge, "mausIstirilkaOrniniElish");
    Bridge_pushFunction(bridge, ug_baord_get_mouse_position);
    Bridge_pushKey(bridge, "mausAylanmaOrniniElish");
    Bridge_pushFunction(bridge, ug_baord_get_mouse_wheel);
    Bridge_pushKey(bridge, "mausKunupkisiHerkitiniElish");
    Bridge_pushFunction(bridge, ug_baord_get_mouse_key_action);
    Bridge_pushKey(bridge, "mausKunupkisiHalitiniElish");
    Bridge_pushFunction(bridge, ug_baord_get_mouse_key_state);
    Bridge_pushKey(bridge, "tahtayKunupkisiHerkitiniElish");
    Bridge_pushFunction(bridge, ug_baord_get_keyboard_key_action);
    Bridge_pushKey(bridge, "tahtayKunupkisiHalitiniElish");
    Bridge_pushFunction(bridge, ug_baord_get_keybaord_key_state);
    // other
    Bridge_pushKey(bridge, "ikranniResimgeTartipSaxlash");
    Bridge_pushFunction(bridge, ug_baord_save_screenshot);
    Bridge_pushKey(bridge, "programmaAwaziniBikitish");
    Bridge_pushFunction(bridge, ug_board_audio_set_volume);
    //
    Bridge_register(bridge);
}
