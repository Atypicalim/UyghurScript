// baord

#include "raylib.h"
#include "../uyghur/uyghur.c"

// tool

void raylib_on_show()
{ 

    // RLAPI void PollInputEvents(void);                                 // Register all input events
    // RLAPI void WaitTime(float ms); 

    // RLAPI void BeginDrawing(void);                                    // Setup canvas (framebuffer) to start drawing
    // RLAPI void EndDrawing(void); 

    // key
    // RLAPI bool IsKeyPressed(int key);                             // Check if a key has been pressed once
    // RLAPI bool IsKeyDown(int key);       // Check if a key is being pressed
    // RLAPI bool IsKeyReleased(int key);                            // Check if a key has been released once
    // RLAPI bool IsKeyUp(int key);         // Check if a key is NOT being pressed
    // RLAPI int GetKeyPressed(void);          // Get keycode pressed, call it multiple times for keys queued, returns 0 when empty

    // mouse
    // RLAPI bool IsMouseButtonPressed(int button);                  // Check if a mouse button has been pressed once
    // RLAPI bool IsMouseButtonDown(int button);                     // Check if a mouse button is being pressed
    // RLAPI bool IsMouseButtonReleased(int button);                 // Check if a mouse button has been released once
    // RLAPI bool IsMouseButtonUp(int button);                       // Check if a mouse button is NOT being pressed
    // RLAPI int GetMouseX(void);                                    // Get mouse position X
    // RLAPI int GetMouseY(void);                                    // Get mouse position Y
    // RLAPI Vector2 GetMouseDelta(void);                            // Get mouse delta between frames
    // RLAPI float GetMouseWheelMove(void);                          // Get mouse wheel movement Y
    // RLAPI void SetMouseCursor(int cursor);                        // Set mouse cursor

    // RLAPI void ClearBackground(Color color);                          // Set background color (framebuffer clear color)
    
    // pixel
    // DrawPixelV(Vector2 position, Color color); 

    // line
    // RLAPI void DrawLineEx(Vector2 startPos, Vector2 endPos, float thick, Color color);
    // RLAPI void DrawLineBezierQuad(Vector2 startPos, Vector2 endPos, Vector2 controlPos, float thick, Color color);
    // RLAPI void DrawLineBezierCubic(Vector2 startPos, Vector2 endPos, Vector2 startControlPos, Vector2 endControlPos, float thick, Color color);

    // circle
    // RLAPI void DrawCircleLines(int centerX, int centerY, float radius, Color color);  
    // RLAPI void DrawCircle(int centerX, int centerY, float radius, Color color);
    // RLAPI void DrawCircleGradient(int centerX, int centerY, float radius, Color color1, Color color2);

    // ellipse
    // DrawEllipse(int centerX, int centerY, float radiusH, float radiusV, Color color);             // Draw ellipse
    // DrawEllipseLines(int centerX, int centerY, float radiusH, float radiusV, Color color);        // Draw ellipse outline

    // ring
// DrawRing(Vector2 center, float innerRadius, float outerRadius, float startAngle, float endAngle, int segments, Color color); // Draw ring
// DrawRingLines(Vector2 center, float innerRadius, float outerRadius, float startAngle, float endAngle, int segments, Color color);    // Draw ring outline


    // rectangle
    // RLAPI void DrawRectanglePro(Rectangle rec, Vector2 origin, float rotation, Color color);                 // Draw a color-filled rectangle with pro parameters
    // RLAPI void DrawRectangleGradientEx(Rectangle rec, Color col1, Color col2, Color col3, Color col4);       // Draw a gradient-filled rectangle with custom vertex colors
    // RLAPI void DrawRectangleLinesEx(Rectangle rec, float lineThick, Color color);                            // Draw rectangle outline with extended parameters
    // RLAPI void DrawRectangleRounded(Rectangle rec, float roundness, int segments, Color color);              // Draw rectangle with rounded edges
    // RLAPI void DrawRectangleRoundedLines(Rectangle rec, float roundness, int segments, float lineThick, Color color); // Draw rectangle with 

    // triangle
    // RLAPI void DrawTriangle(Vector2 v1, Vector2 v2, Vector2 v3, Color color); 
    // RLAPI void DrawTriangleLines(Vector2 v1, Vector2 v2, Vector2 v3, Color color); 
    
    // polygon
    // RLAPI void DrawPoly(Vector2 center, int sides, float radius, float rotation, Color color);
    // RLAPI void DrawPolyLines(Vector2 center, int sides, float radius, float rotation, Color color);
    // RLAPI void DrawPolyLinesEx(Vector2 center, int sides, float radius, float rotation, float lineThick, Color color);


    // image
    // RLAPI Image LoadImage(const char *fileName);  
    // RLAPI void UnloadImage(Image image); 
    // RLAPI Image LoadImageFromScreen(void); 
    // RLAPI bool ExportImage(Image image, const char *fileName); 
    // RLAPI Image ImageFromImage(Image image, Rectangle rec);  
    // RLAPI Image ImageText(const char *text, int fontSize, Color color);
    // RLAPI void ImageCrop(Image *image, Rectangle crop);
    // RLAPI void ImageResize(Image *image, int newWidth, int newHeight); 
    // RLAPI void ImageResizeNN(Image *image, int newWidth,int newHeight); 
    // RLAPI void ImageFlipVertical(Image *image);                                                              // Flip image vertically
    // RLAPI void ImageFlipHorizontal(Image *image);                                                            // Flip image horizontally
    // RLAPI void ImageRotateCW(Image *image); 
    // RLAPI Color GetImageColor(Image image, int x, int y);

    // texture
    // RLAPI Texture2D LoadTextureFromImage(Image image);  
    // RLAPI void DrawTextureEx(Texture2D texture, Vector2 position, float rotation, float scale, Color tint);
    // RLAPI void DrawTextureRec(Texture2D texture, Rectangle source, Vector2 position, Color tint);
    // RLAPI void DrawTextureTiled(Texture2D texture, Rectangle source, Rectangle dest, Vector2 origin, float rotation, float scale, Color tint);

    // font
    // RLAPI Font GetFontDefault(void);                                                            // Get the default Font
    // RLAPI Font LoadFont(const char *fileName);
    // RLAPI void UnloadFont(Font font);
    // RLAPI void DrawText(const char *text, int posX, int posY, int fontSize, Color color);
    // RLAPI void DrawTextEx(Font font, const char *text, Vector2 position, float fontSize, float spacing, Color tint);
    // RLAPI void DrawTextPro(Font font, const char *text, Vector2 position, Vector2 origin, float rotation, float fontSize, float spacing, Color tint);
    // RLAPI int MeasureText(const char *text, int fontSize);
    // RLAPI Vector2 MeasureTextEx(Font font, const char *text, float fontSize, float spacing);
    
    // audio
    // RLAPI void InitAudioDevice(void);                                     // Initialize audio device and context
    // RLAPI void CloseAudioDevice(void);                                    // Close the audio device and context
    // RLAPI bool IsAudioDeviceReady(void);                                  // Check if audio device has been initialized successfully
    // RLAPI void SetMasterVolume(float volume);                             // Set master volume (listener)

    // sound
    // RLAPI Sound LoadSound(const char *fileName);                          // Load sound from file
    // RLAPI void UnloadSound(Sound sound);                                  // Unload sound
    // RLAPI void UpdateSound(Sound sound, const void *data, int sampleCount); // Update sound buffer with new data
    // RLAPI void PlaySound(Sound sound);                                    // Play a sound
    // RLAPI void StopSound(Sound sound);                                    // Stop playing a sound
    // RLAPI void PauseSound(Sound sound);                                   // Pause a sound
    // RLAPI void ResumeSound(Sound sound);                                  // Resume a paused sound
    // RLAPI bool IsSoundPlaying(Sound sound);                               // Check if a sound is currently playing
    // RLAPI void SetSoundVolume(Sound sound, float volume);                 // Set volume for a sound (1.0 is max level)

    // Music
    // RLAPI Music LoadMusicStream(const char *fileName);                    // Load music stream from file
    // RLAPI void UnloadMusicStream(Music music);                            // Unload music stream
    // RLAPI void UnloadMusicStream(Music music);                            // Unload music stream
    // RLAPI void PlayMusicStream(Music music);                              // Start music playing
    // RLAPI bool IsMusicStreamPlaying(Music music);                         // Check if music is playing
    // RLAPI void UpdateMusicStream(Music music);                            // Updates buffers for music streaming
    // RLAPI void StopMusicStream(Music music);                              // Stop music playing
    // RLAPI void PauseMusicStream(Music music);                             // Pause music playing
    // RLAPI void ResumeMusicStream(Music music);                            // Resume playing paused music
    // RLAPI void SeekMusicStream(Music music, float position);              // Seek music to a position (in seconds)
    // RLAPI void SetMusicVolume(Music music, float volume);                 // Set volume for music (1.0 is max level)
    // RLAPI float GetMusicTimeLength(Music music);                          // Get music time length (in seconds)
    // RLAPI float GetMusicTimePlayed(Music music);                          // Get current music time played (in seconds)

    // complex
    // SetWindowIcon(Image image);
    // void TakeScreenshot(const char *fileName);                  // Takes a screenshot of current screen (filename extension defines format)

}

void raylib_on_frame()
{
    Vector2 center = (Vector2){250, 250};
    Color color = (Color){50, 100, 200, 100};
    Rectangle rectangle = (Rectangle){11, 11, 198, 198};
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

void raylib_show_window(int width, int height)
{
    if (IsWindowReady()) return;
    if (width < 0) width = 500;
    if (height < 0) height = 500;
    InitWindow(width, height, "");
    raylib_on_show();
    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(RAYWHITE);
        DrawText("Congrats! You created your first window!", 190, 200, 20, LIGHTGRAY);
        raylib_on_frame();
        EndDrawing();
    }
    CloseWindow();
    raylib_on_hide();
}

// api

void ug_board_show(Bridge *bridge)
{
    int w = Bridge_popNumber(bridge);
    int h = Bridge_popNumber(bridge);
    raylib_show_window(w, h);
    Bridge_startResult(bridge);
    Bridge_return(bridge);
}

void ug_baord_hide(Bridge *bridge)
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
    SetWindowTitle(Bridge_popString(bridge));
    Bridge_startResult(bridge);
    Bridge_return(bridge);
}

void ug_baord_set_position(Bridge *bridge)
{
    int x = Bridge_popNumber(bridge);
    int y = Bridge_popNumber(bridge);
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
    int w = Bridge_popNumber(bridge);
    int h = Bridge_popNumber(bridge);
    SetWindowSize(w, h); 
    Bridge_startResult(bridge);
    Bridge_return(bridge);
}

void ug_baord_get_size(Bridge *bridge)
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
    int w = Bridge_popNumber(bridge);
    int h = Bridge_popNumber(bridge);
    SetWindowMinSize(w, h); 
    Bridge_startResult(bridge);
    Bridge_return(bridge);
}

void ug_baord_show_cursor(Bridge *bridge)
{
    bool b = Bridge_popBoolean(bridge);
    if (b) ShowCursor();
    if (!b) HideCursor();
    Bridge_startResult(bridge);
    Bridge_return(bridge);
}

void ug_baord_enable_cursor(Bridge *bridge)
{
    bool b = Bridge_popBoolean(bridge);
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

void ug_baord_set_fps(Bridge *bridge)
{
    SetTargetFPS(Bridge_popNumber(bridge));
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
    int x = Bridge_popNumber(bridge);
    int y = Bridge_popNumber(bridge);
    DrawFPS(x, y);
    Bridge_startResult(bridge);
    Bridge_return(bridge);
}

void ug_baord_set_clipboard(Bridge *bridge)
{
    char *c = Bridge_popString(bridge);
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


void ug_baord_test(Bridge *bridge)
{
    char *a = Bridge_popString(bridge);
    char *r = "";
    Bridge_startResult(bridge);
    Bridge_pushString(bridge, r);
    Bridge_return(bridge);
}

void lib_raylib_register(Bridge *bridge)
{
    Bridge_startBox(bridge, "doska");
    //
    Bridge_pushKey(bridge, "korsitish");
    Bridge_pushFunction(bridge, ug_board_show);
    //
    Bridge_register(bridge);
    //
    // raylib_show_window(0, 0);
}
