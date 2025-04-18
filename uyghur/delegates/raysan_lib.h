// raysan

#ifndef UG_DELEGATE_RAYSAN_H
#define UG_DELEGATE_RAYSAN_H
#include "./delegates.h"

//////////////////////////////////////////////////////////////

int delegate_stage_mode_swap(int mode) {
    if (mode < 0) return mode;
    int _mode = 0;
    _UG_MODE_SWAP_CONVET(UG_STAGE_WINDOW_MODE_DEFAULT, _GU_INVALID_MAX_BITWISE);
    // _UG_MODE_SWAP_CONVET(UG_STAGE_WINDOW_MODE_FIXED, FLAG_WINDOW_RESIZABLE);
    _UG_MODE_SWAP_CONVET(UG_STAGE_WINDOW_MODE_MAXIMIZED, FLAG_WINDOW_MAXIMIZED);
    _UG_MODE_SWAP_CONVET(UG_STAGE_WINDOW_MODE_FULLSCREEN, FLAG_FULLSCREEN_MODE);
    _UG_MODE_SWAP_CONVET(UG_STAGE_WINDOW_MODE_TRANSPARENT, FLAG_WINDOW_TRANSPARENT);
    _UG_MODE_SWAP_CONVET(UG_STAGE_WINDOW_MODE_BORDERLESS, FLAG_WINDOW_UNDECORATED);
    _UG_MODE_SWAP_CONVET(UG_STAGE_WINDOW_MODE_PINNED, FLAG_WINDOW_TOPMOST);
    return _mode;
}

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
#define replay_InitAudioDevice InitAudioDevice
#define replay_CloseAudioDevice CloseAudioDevice
#define replay_IsAudioDeviceReady IsAudioDeviceReady
//
#define replay_LoadSound LoadSound
#define replay_IsSoundReady IsSoundReady
#define replay_UpdateSound UpdateSound
#define replay_UnloadSound UnloadSound
//
#define replay_LoadMusic LoadMusicStream
#define replay_IsMusicReady IsMusicReady
#define replay_UpdateMusic UpdateMusicStream
#define replay_UnloadMusic UnloadMusicStream
//
#define replay_PlaySound PlaySound
#define replay_StopSound StopSound
#define replay_PauseSound PauseSound
#define replay_ResumeSound ResumeSound
#define replay_IsSoundPlaying IsSoundPlaying
#define replay_SetSoundVolume SetSoundVolume
//
#define replay_PlayMusic PlayMusicStream
#define replay_StopMusic StopMusicStream
#define replay_PauseMusic PauseMusicStream
#define replay_ResumeMusic ResumeMusicStream
#define replay_IsMusicPlaying IsMusicStreamPlaying
#define replay_SetMusicVolume SetMusicVolume
#define replay_GetMusicLength GetMusicTimeLength
#define replay_GetMusicTime GetMusicTimePlayed
#define replay_SetMusicTime SeekMusicStream
//
#define replay_SetSoundPitch SetSoundPitch
#define replay_SetSoundPan SetSoundPan
//
#define replay_SetMusicPitch SetMusicPitch
#define replay_SetMusicPan SetMusicPan

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

int delegate_get_mouse_state(int key) {
    if (IsMouseButtonDown(key)) return UG_BUTTON_STATE_DOWN;
    return UG_BUTTON_STATE_UP;
}

int delegate_get_board_state(int key) {
    if (IsKeyDown(key)) return UG_BUTTON_STATE_DOWN;
    return UG_BUTTON_STATE_UP;
}

void delegate_stage_run_program(int width, int height, CString title, int mode) {
    if (IsWindowReady()) return;
    if (width < 0) width = 500;
    if (height < 0) height = 500;
    if (strlen(title) == 0) title = UG_PROJECT_NAME;
    if (mode < 0) mode = FLAG_WINDOW_RESIZABLE | FLAG_WINDOW_TOPMOST;
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
    //
    bool closeable = WindowShouldClose();
    // 
    if (IsWindowReady()) {
        DrawFPS(10, 10);
        EndDrawing();
        BeginDrawing();
        ClearBackground((Color){33, 33, 33, 255});
    }
    //
    if (closeable && IsWindowReady()) {
        CloseWindow();
        return true;
    } else {
        return false;
    }
}

//////////////////////////////////////////////////////////

#define _COLOR (Color){color.r, color.g, color.b, color.a}
#define _VECTOR (Vector2){vector.x, vector.y}
#define _CENTER (Vector2){center.x, center.y}
#define _ANCHOR (Vector2){anchor.x, anchor.y}
#define _POINT (Vector2){point.x, point.y}
#define _POINT1 (Vector2){point1.x, point1.y}
#define _POINT2 (Vector2){point2.x, point2.y}
#define _POINT3 (Vector2){point3.x, point3.y}
#define _RECT (Rectangle){rect.x, rect.y, rect.w, rect.h}

//////////////////////////////////////////////////////////

Color __plotColor;
Vector2 __plotPoint;
void deletage_paint_plot(int x, int y, int r, int g, int b, int a) {
    __plotColor.r = r;
    __plotColor.g = g;
    __plotColor.b = b;
    __plotColor.a = a;
    __plotPoint.x = x;
    __plotPoint.y = y;
    DrawPixelV(__plotPoint, __plotColor);
}

int rlLoadTexture(UGPixels, int, int, int, int);

Texture2D *__renderTexture = NULL;
Rectangle __renderRect;
void delegate_soft_render(UGPixels pixels, int w, int h, int channel) {
    // 
    if (__renderTexture == NULL) {
        __renderTexture = (Texture2D *)malloc(sizeof(Texture2D));
        __renderTexture->format = PIXELFORMAT_UNCOMPRESSED_R8G8B8A8;
        __renderTexture->mipmaps = 1;
        __renderTexture->width = w;
        __renderTexture->height = h;
        __renderTexture->id = rlLoadTexture(pixels, w, h, __renderTexture->format, __renderTexture->mipmaps);
    } else {
        UpdateTexture(*__renderTexture, pixels);
    }
    //
    __renderRect.x = 0;
    __renderRect.y = 0;
    __renderRect.width = w;
    __renderRect.height = h;
    Vector2 origin = (Vector2){0, 0};
    DrawTexturePro(*__renderTexture, __renderRect, __renderRect, origin, 0, WHITE);
    // 
    // UnloadTexture(__renderTexture);
}

////////////////////////////////////////////////////////// pencil

void delegate_pencil_customize(UGColor color, int rotation) {

}

void delegate_pencil_draw_pixel(UGPoint point, UGColor color) {
    DrawPixelV(_POINT, _COLOR);
}

void delegate_pencil_draw_line(UGPoint point1, UGPoint point2, int thickness, UGColor color) {
    DrawLineEx(_POINT1, _POINT2, thickness, _COLOR);
}

void delegate_pencil_draw_triangle(UGPoint point1, UGPoint point2, UGPoint point3, UGColor color, double thickness)
{
    DrawTriangleLines(_POINT1, _POINT2, _POINT3, _COLOR);
}

void delegate_pencil_fill_triangle(UGPoint point1, UGPoint point2, UGPoint point3, UGColor color)
{
    int t1 = point1.y / point1.x;
    int t2 = point2.y / point2.x;
    int t3 = point3.y / point3.x;
    if(t1 < t2) UG_POINT_SWAP(&point1, &point2);
    if(t2 < t3) UG_POINT_SWAP(&point2, &point3);
    if(t1 < t2) UG_POINT_SWAP(&point1, &point2);
    DrawTriangle(_POINT1, _POINT2, _POINT3, _COLOR); // have to be counter-clock-wise.
}

void delegate_pencil_draw_rectangle(UGPoint point, UGSize size, UGColor color, double thickness)
{
    Rectangle _rect = (Rectangle){point.x - size.w / 2, point.y - size.h / 2, size.w, size.h};
    DrawRectangleLinesEx(_rect, thickness, _COLOR);
}

void delegate_pencil_fill_rectangle(UGPoint point, UGSize size, UGColor color)
{
    Rectangle _rect = (Rectangle){point.x - size.w / 2, point.y - size.h / 2, size.w, size.h};
    Vector2 _anchor = (Vector2){0.5, 0.5};
    DrawRectanglePro(_rect, _anchor, 0, _COLOR);
}

void delegate_pencil_draw_circle(UGPoint point, int radius, UGColor color, double thickness)
{
    DrawEllipseLines(point.x, point.y, radius, radius, _COLOR);
}

void delegate_pencil_fill_circle(UGPoint point, int radius, UGColor color)
{
    DrawEllipse(point.x, point.y, radius, radius, _COLOR);
}

void delegate_pencil_draw_polygon(UGPoint center, int sides, double radius, double rotation, UGColor color, double thickness)
{
    DrawPolyLinesEx(_CENTER, sides, radius, rotation, thickness, _COLOR);
}

void delegate_pencil_fill_polygon(UGPoint center, int sides, double radius, double rotation, UGColor color)
{
    DrawPoly(_CENTER, sides, radius, rotation, _COLOR);
}

//////////////////////////////////////////////////////////

void *delegate_unload_image(UGImage *img) {
    Texture *tex = img->txtr;
    if (tex != NULL) {UnloadTexture(tex[0]);}
    return tex;
}

void *delegate_unload_font(UGFont *fnt) {
    Font *_font = fnt->font;
    if (_font != NULL) {UnloadFont(_font[0]);}
    return _font;
}


UGImage *delegate_load_image(UGImage *img)
{
    int format = PIXELFORMAT_UNCOMPRESSED_R8G8B8A8;
    int mipmap = 1;
    Texture2D texture = { 0 };
    texture.id = rlLoadTexture(img->pxls, img->w, img->h, format, mipmap);
    texture.width = img->w;
    texture.height = img->h;
    texture.mipmaps = mipmap;
    texture.format = format;
    //
    Texture *_texture = (Texture *)malloc(sizeof(Texture));
    _texture[0] = texture;
    img->txtr = (void*)_texture;
    return img;
}

UGFont *delegate_load_font(UGFont *fnt)
{
    int codepoints[1024 * 3] = { 0 };
    for (int i = 0; i < 1024; i++) codepoints[0 + i] = 0 + i; // latin
    for (int i = 0; i < 1024; i++) codepoints[1024 + i] = 0x0400 + i; // cyrillic
    for (int i = 0; i < 1024; i++) codepoints[2048 + i] = 0xfb00 + i; // arabic
    Font font = LoadFontEx(fnt->path, fnt->size, codepoints, 1024 * 3);
    //
    Font *_font = (Font *)malloc(sizeof(Font));
    _font[0] = font;
    fnt->font = (void*)_font;
    return fnt;
}

//////////////////////////////////////////////////////////

void delegate_pencil_draw_image(UGImage *image, int x, int y, float anchorX, float anchorY, UGColor color, float rotation, float scale) {

    Texture *tex = image->txtr;
    Texture texture = tex[0];
    //
    Rectangle source = (Rectangle){0, 0, image->w, image->h};
    float destW = texture.width * scale;
    float destH = texture.height * scale;
    float destX = x - destW / 2;
    float destY = y - destH / 2;
    Rectangle dest = (Rectangle){destX, destY, destW, destH};
    Vector2 origin = (Vector2){0, 0};
    DrawTexturePro(texture, source, dest, origin, rotation, _COLOR);
}

void delegate_pencil_draw_font(UGFont *font, char *text, int size, UGColor color, UGPoint point) {
    Font *fnt = font->font;
    Font _fnt = fnt[0];
    Vector2 space = MeasureTextEx(_fnt, text, size, 0);
    point.x = point.x - space.x / 2;
    point.y = point.y - space.y / 2;
    DrawTextEx(_fnt, text, _POINT, size, 0, _COLOR);
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
