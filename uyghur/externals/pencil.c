// baord

#include "raylib.h"
#include "../uyghur.c"

// tool

Color color_from_bridge(Bridge *bridge)
{
    CString str = Bridge_receiveString(bridge);
    int len = strlen(str);
    if (len != 6 && len != 8) return BLACK;
    int r = char_to_int(str[0]) * 16 + char_to_int(str[1]);
    int g = char_to_int(str[2]) * 16 + char_to_int(str[3]);
    int b = char_to_int(str[4]) * 16 + char_to_int(str[5]);
    int a = len == 6 ? 255 : char_to_int(str[6]) * 16 + char_to_int(str[7]);;
    return (Color){r, g, b, a};
}

Vector2 vector_from_bridge(Bridge *bridge)
{
    float x = Bridge_receiveNumber(bridge);
    float y = Bridge_receiveNumber(bridge);
    return (Vector2){x, y};
}

Rectangle rectangle_from_bridge(Bridge *bridge)
{
    float x = Bridge_receiveNumber(bridge);
    float y = Bridge_receiveNumber(bridge);
    float w = Bridge_receiveNumber(bridge);
    float h = Bridge_receiveNumber(bridge);
    return (Rectangle){x, y, w, h};
}

void _raylib_release_texture(Loadable *loadable) {
    if (loadable->obj) {
        pct_free(loadable->obj);
        loadable->obj = NULL;
    }
}

Texture texture_from_bridge(Bridge *bridge)
{
    Loadable *loadable = Bridge_receiveValue(bridge, UG_TYPE_STF);
    Texture *texture = loadable->obj;
    Texture t = texture != NULL ? texture[0] : defaultTexture;
    return t;
}

Image raylib_load_image(CString path)
{
    if (path == NULL || strlen(path) == 0) return defaultImage;
    Image image = LoadImage(path);
    if (!image.data) image = defaultImage;
    return image;
}

Font raylib_load_font(CString path, int size)
{
    if (path == NULL || strlen(path) == 0) return defaultFont;
    //
    int codepoints[1024 * 3] = { 0 };
    for (int i = 0; i < 1024; i++) codepoints[0 + i] = 0 + i; // latin
    for (int i = 0; i < 1024; i++) codepoints[1024 + i] = 0x0400 + i; // cyrillic
    for (int i = 0; i < 1024; i++) codepoints[2048 + i] = 0xfb00 + i; // arabic
    Font font = LoadFontEx(path, size, codepoints, 1024 * 3);
    // Font font = LoadFont(path);
    return font;
}

typedef struct ImgInfo {
    CString path;
    bool flipX;
    bool flipY;
    bool isGray;
} ImgInfo;

typedef struct TxtInfo {
    CString path;
    CString text;
    float size;
    float spacing;
} TxtInfo;

Texture *_pencil_reference_texture(Image img) {
    Texture texture = LoadTextureFromImage(img);
    Texture *tex = (Texture *)malloc(sizeof(texture));
    tex[0] = texture;
    UnloadImage(img);
    return tex;
}

Texture *raylib_texturize_image(ImgInfo info)
{
    Image image = raylib_load_image(info.path);
    Image img = ImageFromImage(image, (Rectangle){0, 0, image.width, image.height});
    if (info.flipX) ImageFlipHorizontal(&img);
    if (info.flipY) ImageFlipVertical(&img);
    if (info.isGray) ImageColorGrayscale(&img);
    Texture *texture = _pencil_reference_texture(img);
    UnloadImage(image);
    //
    return texture;
}

Texture *raylib_texturize_text(TxtInfo info)
{
    Font font = raylib_load_font(info.path, info.size);
    Image img = ImageTextEx(font, info.text, info.size, info.spacing, WHITE);
    Texture *texture = _pencil_reference_texture(img);
    UnloadFont(font);
    //
    return texture;
}

// draw

void native_pencil_draw_pixel(Bridge *bridge)
{
    Vector2 poit = vector_from_bridge(bridge);
    Color color = color_from_bridge(bridge);
    DrawPixelV(poit, color);
    Bridge_returnEmpty(bridge);
}

void native_pencil_draw_line_no_controll(Bridge *bridge)
{
    Vector2 poit1 = vector_from_bridge(bridge);
    Vector2 poit2 = vector_from_bridge(bridge);
    int thickness = Bridge_receiveNumber(bridge);
    Color color = color_from_bridge(bridge);
    DrawLineEx(poit1, poit2, thickness, color);
    Bridge_returnEmpty(bridge);
}

void native_pencil_draw_line_one_controll(Bridge *bridge)
{
    Vector2 poit1 = vector_from_bridge(bridge);
    Vector2 poit2 = vector_from_bridge(bridge);
    int thickness = Bridge_receiveNumber(bridge);
    Color color = color_from_bridge(bridge);
    Vector2 controll1 = vector_from_bridge(bridge);
    DrawLineEx(poit1, poit2, thickness, color);
    DrawLineBezierQuad(poit1, poit2, controll1, thickness, color);
    Bridge_returnEmpty(bridge);
}

void native_pencil_draw_line_two_controll(Bridge *bridge)
{
    Vector2 poit1 = vector_from_bridge(bridge);
    Vector2 poit2 = vector_from_bridge(bridge);
    int thickness = Bridge_receiveNumber(bridge);
    Color color = color_from_bridge(bridge);
    Vector2 controll1 = vector_from_bridge(bridge);
    Vector2 controll2 = vector_from_bridge(bridge);
    DrawLineEx(poit1, poit2, thickness, color);
    DrawLineBezierCubic(poit1, poit2, controll1, controll1, thickness, color);
    Bridge_returnEmpty(bridge);
}

void native_pencil_draw_rectangle_fill_transformed(Bridge *bridge)
{
    Rectangle rectangle = rectangle_from_bridge(bridge);
    Color color = color_from_bridge(bridge);
    Vector2 anchor = vector_from_bridge(bridge);
    anchor.x = anchor.x * rectangle.width;
    anchor.y = anchor.y * rectangle.height;
    float rotation = Bridge_receiveNumber(bridge);
    DrawRectanglePro(rectangle, anchor, rotation, color);
    Bridge_returnEmpty(bridge);
}

void native_pencil_draw_rectangle_fill_colorful(Bridge *bridge)
{
    Rectangle rectangle = rectangle_from_bridge(bridge);
    Color leftTop = color_from_bridge(bridge);
    Color leftBottom = color_from_bridge(bridge);
    Color rightBottom = color_from_bridge(bridge);
    Color rightTop = color_from_bridge(bridge);
    DrawRectangleGradientEx(rectangle, leftTop, leftBottom, rightBottom, rightTop);
    Bridge_returnEmpty(bridge);
}

void native_pencil_draw_rectangle_fill_round(Bridge *bridge)
{
    Rectangle rectangle = rectangle_from_bridge(bridge);
    Color color = color_from_bridge(bridge);
    int roundness = Bridge_receiveNumber(bridge);
    DrawRectangleRounded(rectangle, roundness, 0, color);
    Bridge_returnEmpty(bridge);
}

void native_pencil_draw_rectangle_stroke(Bridge *bridge)
{
    Rectangle rectangle = rectangle_from_bridge(bridge);
    Color color = color_from_bridge(bridge);
    double roundness = Bridge_receiveNumber(bridge);
    double thickness = Bridge_receiveNumber(bridge);
    DrawRectangleRoundedLines(rectangle, roundness, 0, thickness, color);
    Bridge_returnEmpty(bridge);
}

void native_pencil_draw_circle_fill(Bridge *bridge)
{
    int centerX = Bridge_receiveNumber(bridge);
    int centerY = Bridge_receiveNumber(bridge);
    int radiusH = Bridge_receiveNumber(bridge);
    int radiusV = Bridge_receiveNumber(bridge);
    Color color = color_from_bridge(bridge);
    DrawEllipse(centerX, centerY, radiusH, radiusV, color);
    Bridge_returnEmpty(bridge);
}

void native_pencil_draw_circle_stroke(Bridge *bridge)
{
    int centerX = Bridge_receiveNumber(bridge);
    int centerY = Bridge_receiveNumber(bridge);
    int radiusH = Bridge_receiveNumber(bridge);
    int radiusV = Bridge_receiveNumber(bridge);
    Color color = color_from_bridge(bridge);
    DrawEllipseLines(centerX, centerY, radiusH, radiusV, color);
    Bridge_returnEmpty(bridge);
}

void native_pencil_draw_ring_fill(Bridge *bridge)
{
    Vector2 center = vector_from_bridge(bridge);
    double innerRadius = Bridge_receiveNumber(bridge);
    double outerRadius = Bridge_receiveNumber(bridge);
    Color color = color_from_bridge(bridge);
    double startAngle = Bridge_receiveNumber(bridge);
    double endAngle = Bridge_receiveNumber(bridge);
    DrawRing(center, innerRadius, outerRadius, startAngle, endAngle, 0, color);
    Bridge_returnEmpty(bridge);
}

void native_pencil_draw_ring_stroke(Bridge *bridge)
{
    Vector2 center = vector_from_bridge(bridge);
    double innerRadius = Bridge_receiveNumber(bridge);
    double outerRadius = Bridge_receiveNumber(bridge);
    Color color = color_from_bridge(bridge);
    double startAngle = Bridge_receiveNumber(bridge);
    double endAngle = Bridge_receiveNumber(bridge);
    DrawRingLines(center, innerRadius, outerRadius, startAngle, endAngle, 0, color);
    Bridge_returnEmpty(bridge);
}

void native_pencil_draw_triangle_fill(Bridge *bridge)
{
    Vector2 point1 = vector_from_bridge(bridge);
    Vector2 point2 = vector_from_bridge(bridge);
    Vector2 point3 = vector_from_bridge(bridge);
    Color color = color_from_bridge(bridge);
    DrawTriangle(point1, point2, point3, color);
    Bridge_returnEmpty(bridge);
}

void native_pencil_draw_triangle_stroke(Bridge *bridge)
{
    Vector2 point1 = vector_from_bridge(bridge);
    Vector2 point2 = vector_from_bridge(bridge);
    Vector2 point3 = vector_from_bridge(bridge);
    Color color = color_from_bridge(bridge);
    DrawTriangleLines(point1, point2, point3, color);
    Bridge_returnEmpty(bridge);
}

void native_pencil_draw_polygon_fill(Bridge *bridge)
{
    Vector2 center = vector_from_bridge(bridge);
    int sides = Bridge_receiveNumber(bridge);
    double radius = Bridge_receiveNumber(bridge);
    double rotation = Bridge_receiveNumber(bridge);
    Color color = color_from_bridge(bridge);
    DrawPoly(center, sides, radius, rotation, color);
    Bridge_returnEmpty(bridge);
}

void native_pencil_draw_polygon_stroke(Bridge *bridge)
{
    Vector2 center = vector_from_bridge(bridge);
    int sides = Bridge_receiveNumber(bridge);
    double radius = Bridge_receiveNumber(bridge);
    double rotation = Bridge_receiveNumber(bridge);
    double thickness = Bridge_receiveNumber(bridge);
    Color color = color_from_bridge(bridge);
    DrawPolyLinesEx(center, sides, radius, rotation, thickness, color);
    Bridge_returnEmpty(bridge);
}

// text

void native_pencil_draw_text(Bridge *bridge)
{
    CString font = Bridge_receiveString(bridge);
    CString text = Bridge_receiveString(bridge);
    float size = Bridge_receiveNumber(bridge);
    float spacing = Bridge_receiveNumber(bridge);
    Color color = color_from_bridge(bridge);
    Vector2 position = vector_from_bridge(bridge);
    Font fnt = raylib_load_font(font, 32);
    DrawTextEx(fnt, text, position, size, spacing, color);
    Bridge_returnEmpty(bridge);
}

void native_pencil_measure_text(Bridge *bridge)
{
    CString font = Bridge_receiveString(bridge);
    CString text = Bridge_receiveString(bridge);
    float size = Bridge_receiveNumber(bridge);
    float spacing = Bridge_receiveNumber(bridge);
    Font fnt = raylib_load_font(font, 32);
    Vector2 space = MeasureTextEx(fnt, text, size, spacing);
    Bridge_returnNumber(bridge, space.x);
}

// image texture

void native_pencil_texturize_image(Bridge *bridge)
{
    CString image = Bridge_receiveString(bridge);
    bool isGray = Bridge_receiveBoolean(bridge);
    bool flipX = Bridge_receiveBoolean(bridge);
    bool flipY = Bridge_receiveBoolean(bridge);
    //
    ImgInfo info = (ImgInfo) {image, flipX, flipY, isGray};
    Texture *texture = raylib_texturize_image(info);
    Loadable *loadable = Loadable_newStuf(texture, ALIAS_texture, image, _raylib_release_texture);
    Bridge_returnValue(bridge, loadable);
}

// text texture

void native_pencil_texturize_text(Bridge *bridge)
{
    CString font = Bridge_receiveString(bridge);
    CString text = Bridge_receiveString(bridge);
    float size = Bridge_receiveNumber(bridge);
    float spacing = Bridge_receiveNumber(bridge);
    //
    TxtInfo info = (TxtInfo) {font, text, size, spacing};
    Texture *texture = raylib_texturize_text(info);
    Loadable *loadable = Loadable_newStuf(texture, ALIAS_texture, font, _raylib_release_texture);
    Bridge_returnValue(bridge, loadable);
}

// texture

void native_pencil_draw_texture(Bridge *bridge)
{
    Texture texture = texture_from_bridge(bridge);
    int x = Bridge_receiveNumber(bridge);
    int y = Bridge_receiveNumber(bridge);
    float anchorX = Bridge_receiveNumber(bridge);
    float anchorY = Bridge_receiveNumber(bridge);
    Color color = color_from_bridge(bridge);
    int fromX = Bridge_receiveNumber(bridge);
    int fromY = Bridge_receiveNumber(bridge);
    int width = Bridge_receiveNumber(bridge);
    int height = Bridge_receiveNumber(bridge);
    float rotation = Bridge_receiveNumber(bridge);
    float scale = Bridge_receiveNumber(bridge);
    //
    fromX = MAX(0, MIN(fromX, texture.width - 1));
    fromY = MAX(0, MIN(fromY, texture.height - 1));
    int leftX = texture.width - fromX;
    int leftY = texture.height - fromY;
    width = MAX(1, MIN((width <= 0 ? texture.width : width), leftX));
    height = MAX(1, MIN((height <= 0 ? texture.height : height), leftY));
    //
    Rectangle source = (Rectangle){fromX, fromY, width, height};
    float destW = width * scale;
    float destH = height * scale;
    float destX = x;
    float destY = y;
    Rectangle dest = (Rectangle){destX, destY, destW, destH};
    Vector2 origin = (Vector2){destW * anchorX, destH * anchorY};
    DrawTexturePro(texture, source, dest, origin, rotation, color);
    //
    Bridge_returnEmpty(bridge);
}
