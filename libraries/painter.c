// baord

#include "raylib.h"
#include "../uyghur/uyghur.c"

// tool

Color color_from_bridge(Bridge *bridge)
{
    char *str = Bridge_nexString(bridge);
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
    float x = Bridge_nexNumber(bridge);
    float y = Bridge_nexNumber(bridge);
    return (Vector2){x, y};
}

Rectangle rectangle_from_bridge(Bridge *bridge)
{
    float x = Bridge_nexNumber(bridge);
    float y = Bridge_nexNumber(bridge);
    float w = Bridge_nexNumber(bridge);
    float h = Bridge_nexNumber(bridge);
    return (Rectangle){x, y, w, h};
}

Image raylib_load_image(char *path)
{
    if (path == NULL) path = "";
    if (strlen(path) == 0) return defaultImage;
    Image *img;
    img = Hashmap_get(resourcesMap, path);
    if (img != NULL) {
        return img[0];
    }
    Image image = LoadImage(path);
    if (!image.data) image = defaultImage;
    img = (Image *)malloc(sizeof(image));
    img[0] = image;
    Hashmap_set(resourcesMap, path, img);
    return image;
}

void raylib_unload_image(char *path)
{
    if (path == NULL) path = "";
    Image *img = Hashmap_get(resourcesMap, path);
    if (img == NULL) return;
    Hashmap_del(resourcesMap, path);
    Image image = img[0];
    UnloadImage(image);
    free(img);
}

Font raylib_load_font(char *path)
{
    if (path == NULL) path = "";
    if (strlen(path) == 0) return defaultFont;
    Font *fnt;
    fnt = Hashmap_get(resourcesMap, path);
    if (fnt != NULL) {
        return fnt[0];
    }
    Font font = LoadFont(path);
    fnt = (Font *)malloc(sizeof(font));
    fnt[0] = font;
    Hashmap_set(resourcesMap, path, fnt);
    return font;
}

void raylib_unload_font(char *path)
{
    if (path == NULL) path = "";
    Font *fnt = Hashmap_get(resourcesMap, path);
    if (fnt == NULL) return;
    Hashmap_del(resourcesMap, path);
    Font image = fnt[0];
    UnloadFont(image);
    free(fnt);
}

typedef struct ImgInfo {
    char *path;
    int x;
    int y;
    int w;
    int h;
    bool flipX;
    bool flipY;
} ImgInfo;

char *get_texture_tag_for_image(ImgInfo info)
{
    int x = info.x;
    int y = info.y;
    int w = info.w;
    int h = info.h;
    return tools_format(
        "<R-IMAGE:%s:%d,%d:%d,%d:%s,%s>",
        info.path,
        x, y, w, h,
        b2s(info.flipX), b2s(info.flipY)
    );
}

Texture raylib_create_texture_from_image(ImgInfo info, char *tag)
{
    Texture *tex = Hashmap_get(resourcesMap, tag);
    if (tex != NULL) {
        return tex[0];
    }
    //
    int x = info.x;
    int y = info.y;
    int w = info.w;
    int h = info.h;
    //
    Image img = raylib_load_image(info.path);
    int imgW = img.width;
    int imgH = img.height;
    x = MAX(0, MIN(x, imgW - 1));
    y = MAX(0, MIN(y, imgH - 1));
    int leftX = imgW - x;
    int leftY = imgH - y;
    w = MAX(1, MIN((w <= 0 ? imgW : w), leftX));
    h = MAX(1, MIN((h <= 0 ? imgH : h), leftY));
    img = ImageFromImage(img, (Rectangle){x, y, w, h});
    //
    if (info.flipX) ImageFlipHorizontal(&img);
    if (info.flipY) ImageFlipVertical(&img);
    //
    Texture texture = LoadTextureFromImage(img);
    tex = (Texture *)malloc(sizeof(texture));
    tex[0] = texture;
    Hashmap_set(resourcesMap, tag, tex);
    UnloadImage(img);
    return texture;
}

typedef struct TxtInfo {
    char *path;
    char *text;
    float size;
    float spacing;
} TxtInfo;

char *get_texture_tag_for_text(TxtInfo info)
{
    return tools_format(
        "<R-TEXT:%s:%s:%d:%d>",
        info.path,
        info.text,
        info.size,
        info.spacing
    );
}

Texture raylib_create_texture_from_text(TxtInfo info, char *tag)
{
    Texture *tex = Hashmap_get(resourcesMap, tag);
    if (tex != NULL) {
        return tex[0];
    }
    Font font = raylib_load_font(info.path);
    Image img = ImageTextEx(font, info.text, info.size, info.spacing, WHITE);
    Texture texture = LoadTextureFromImage(img);
    tex = (Texture *)malloc(sizeof(texture));
    tex[0] = texture;
    Hashmap_set(resourcesMap, tag, tex);
    UnloadImage(img);
    return texture;
}

Texture ralib_get_texture_by_tag(char *tag)
{
    Texture *tex = Hashmap_get(resourcesMap, tag);
    if (tex == NULL) return defaultTexture;
    return tex[0];
}

void raylib_draw_texture_by_texture(
    Texture texture,
    int x, int y,
    float anchorX, float anchorY,
    Color color,
    int fromX, int fromY, int width, int height,
    float rotation,
    float scale
)
{
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
    DrawTextureTiled(texture, source, dest, origin, rotation, scale, color);
}

// draw

void ug_baord_draw_start(Bridge *bridge)
{
    BeginDrawing();
    Bridge_startResult(bridge);
    Bridge_return(bridge);
}

void ug_baord_draw_end(Bridge *bridge)
{
    EndDrawing();
    Bridge_startResult(bridge);
    Bridge_return(bridge);
}

void ug_baord_draw_pixel(Bridge *bridge)
{
    Vector2 poit = vector_from_bridge(bridge);
    Color color = color_from_bridge(bridge);
    DrawPixelV(poit, color);
    Bridge_startResult(bridge);
    Bridge_return(bridge);
}

void ug_baord_draw_line_no_controll(Bridge *bridge)
{
    Vector2 poit1 = vector_from_bridge(bridge);
    Vector2 poit2 = vector_from_bridge(bridge);
    int thickness = Bridge_nexNumber(bridge);
    Color color = color_from_bridge(bridge);
    DrawLineEx(poit1, poit2, thickness, color);
    Bridge_startResult(bridge);
    Bridge_return(bridge);
}

void ug_baord_draw_line_one_controll(Bridge *bridge)
{
    Vector2 poit1 = vector_from_bridge(bridge);
    Vector2 poit2 = vector_from_bridge(bridge);
    int thickness = Bridge_nexNumber(bridge);
    Color color = color_from_bridge(bridge);
    Vector2 controll1 = vector_from_bridge(bridge);
    DrawLineEx(poit1, poit2, thickness, color);
    DrawLineBezierQuad(poit1, poit2, controll1, thickness, color);
    Bridge_startResult(bridge);
    Bridge_return(bridge);
}

void ug_baord_draw_line_two_controll(Bridge *bridge)
{
    Vector2 poit1 = vector_from_bridge(bridge);
    Vector2 poit2 = vector_from_bridge(bridge);
    int thickness = Bridge_nexNumber(bridge);
    Color color = color_from_bridge(bridge);
    Vector2 controll1 = vector_from_bridge(bridge);
    Vector2 controll2 = vector_from_bridge(bridge);
    DrawLineEx(poit1, poit2, thickness, color);
    DrawLineBezierCubic(poit1, poit2, controll1, controll1, thickness, color);
    Bridge_startResult(bridge);
    Bridge_return(bridge);
}

void ug_baord_draw_rectangle_fill_transformed(Bridge *bridge)
{
    Rectangle rectangle = rectangle_from_bridge(bridge);
    Color color = color_from_bridge(bridge);
    Vector2 anchor = vector_from_bridge(bridge);
    anchor.x = anchor.x * rectangle.width;
    anchor.y = anchor.y * rectangle.height;
    float rotation = Bridge_nexNumber(bridge);
    DrawRectanglePro(rectangle, anchor, rotation, color);
    Bridge_startResult(bridge);
    Bridge_return(bridge);
}

void ug_baord_draw_rectangle_fill_colorful(Bridge *bridge)
{
    Rectangle rectangle = rectangle_from_bridge(bridge);
    Color leftTop = color_from_bridge(bridge);
    Color leftBottom = color_from_bridge(bridge);
    Color rightBottom = color_from_bridge(bridge);
    Color rightTop = color_from_bridge(bridge);
    DrawRectangleGradientEx(rectangle, leftTop, leftBottom, rightBottom, rightTop);
    Bridge_startResult(bridge);
    Bridge_return(bridge);
}

void ug_baord_draw_rectangle_fill_round(Bridge *bridge)
{
    Rectangle rectangle = rectangle_from_bridge(bridge);
    Color color = color_from_bridge(bridge);
    int roundness = Bridge_nexNumber(bridge);
    DrawRectangleRounded(rectangle, roundness, 0, color);
    Bridge_startResult(bridge);
    Bridge_return(bridge);
}

void ug_baord_draw_rectangle_stroke(Bridge *bridge)
{
    Rectangle rectangle = rectangle_from_bridge(bridge);
    Color color = color_from_bridge(bridge);
    double roundness = Bridge_nexNumber(bridge);
    double thickness = Bridge_nexNumber(bridge);
    DrawRectangleRoundedLines(rectangle, roundness, 0, thickness, color);
    Bridge_startResult(bridge);
    Bridge_return(bridge);
}

void ug_baord_draw_circle_fill(Bridge *bridge)
{
    int centerX = Bridge_nexNumber(bridge);
    int centerY = Bridge_nexNumber(bridge);
    int radiusH = Bridge_nexNumber(bridge);
    int radiusV = Bridge_nexNumber(bridge);
    Color color = color_from_bridge(bridge);
    DrawEllipse(centerX, centerY, radiusH, radiusV, color);
    Bridge_startResult(bridge);
    Bridge_return(bridge);
}

void ug_baord_draw_circle_stroke(Bridge *bridge)
{
    int centerX = Bridge_nexNumber(bridge);
    int centerY = Bridge_nexNumber(bridge);
    int radiusH = Bridge_nexNumber(bridge);
    int radiusV = Bridge_nexNumber(bridge);
    Color color = color_from_bridge(bridge);
    DrawEllipseLines(centerX, centerY, radiusH, radiusV, color);
    Bridge_startResult(bridge);
    Bridge_return(bridge);
}

void ug_baord_draw_ring_fill(Bridge *bridge)
{
    Vector2 center = vector_from_bridge(bridge);
    double innerRadius = Bridge_nexNumber(bridge);
    double outerRadius = Bridge_nexNumber(bridge);
    Color color = color_from_bridge(bridge);
    double startAngle = Bridge_nexNumber(bridge);
    double endAngle = Bridge_nexNumber(bridge);
    DrawRing(center, innerRadius, outerRadius, startAngle, endAngle, 0, color);
    Bridge_startResult(bridge);
    Bridge_return(bridge);
}

void ug_baord_draw_ring_stroke(Bridge *bridge)
{
    Vector2 center = vector_from_bridge(bridge);
    double innerRadius = Bridge_nexNumber(bridge);
    double outerRadius = Bridge_nexNumber(bridge);
    Color color = color_from_bridge(bridge);
    double startAngle = Bridge_nexNumber(bridge);
    double endAngle = Bridge_nexNumber(bridge);
    DrawRingLines(center, innerRadius, outerRadius, startAngle, endAngle, 0, color);
    Bridge_startResult(bridge);
    Bridge_return(bridge);
}

void ug_baord_draw_triangle_fill(Bridge *bridge)
{
    Vector2 point1 = vector_from_bridge(bridge);
    Vector2 point2 = vector_from_bridge(bridge);
    Vector2 point3 = vector_from_bridge(bridge);
    Color color = color_from_bridge(bridge);
    DrawTriangle(point1, point2, point3, color);
    Bridge_startResult(bridge);
    Bridge_return(bridge);
}

void ug_baord_draw_triangle_stroke(Bridge *bridge)
{
    Vector2 point1 = vector_from_bridge(bridge);
    Vector2 point2 = vector_from_bridge(bridge);
    Vector2 point3 = vector_from_bridge(bridge);
    Color color = color_from_bridge(bridge);
    DrawTriangleLines(point1, point2, point3, color);
    Bridge_startResult(bridge);
    Bridge_return(bridge);
}

void ug_baord_draw_polygon_fill(Bridge *bridge)
{
    Vector2 center = vector_from_bridge(bridge);
    int sides = Bridge_nexNumber(bridge);
    double radius = Bridge_nexNumber(bridge);
    double rotation = Bridge_nexNumber(bridge);
    Color color = color_from_bridge(bridge);
    DrawPoly(center, sides, radius, rotation, color);
    Bridge_startResult(bridge);
    Bridge_return(bridge);
}

void ug_baord_draw_polygon_stroke(Bridge *bridge)
{
    Vector2 center = vector_from_bridge(bridge);
    int sides = Bridge_nexNumber(bridge);
    double radius = Bridge_nexNumber(bridge);
    double rotation = Bridge_nexNumber(bridge);
    double thickness = Bridge_nexNumber(bridge);
    Color color = color_from_bridge(bridge);
    DrawPolyLinesEx(center, sides, radius, rotation, thickness, color);
    Bridge_startResult(bridge);
    Bridge_return(bridge);
}

// text

void ug_baord_draw_text(Bridge *bridge)
{
    char *font = Bridge_nexString(bridge);
    char *text = Bridge_nexString(bridge);
    float size = Bridge_nexNumber(bridge);
    float spacing = Bridge_nexNumber(bridge);
    Color color = color_from_bridge(bridge);
    Vector2 position = vector_from_bridge(bridge);
    Font fnt = raylib_load_font(font);
    DrawTextEx(fnt, text, position, size, spacing, color);
    Bridge_startResult(bridge);
    Bridge_return(bridge);
}

void ug_baord_measure_text(Bridge *bridge)
{
    char *font = Bridge_nexString(bridge);
    char *text = Bridge_nexString(bridge);
    float size = Bridge_nexNumber(bridge);
    float spacing = Bridge_nexNumber(bridge);
    Font fnt = raylib_load_font(font);
    Vector2 space = MeasureTextEx(fnt, text, size, spacing);
    Bridge_startResult(bridge);
    Bridge_pushNumber(bridge, space.x);
    Bridge_return(bridge);
}

// image texture

void ug_baord_create_texture_from_image(Bridge *bridge)
{
    char *image = Bridge_nexString(bridge);
    int x = Bridge_nexNumber(bridge);
    int y = Bridge_nexNumber(bridge);
    int w = Bridge_nexNumber(bridge);
    int h = Bridge_nexNumber(bridge);
    bool flipX = Bridge_nexBoolean(bridge);
    bool flipY = Bridge_nexBoolean(bridge);
    ImgInfo info = (ImgInfo) {image, x, y, w, h, flipX, flipY};
    char *tag = get_texture_tag_for_image(info);
    Texture texture = raylib_create_texture_from_image(info, tag);
    Bridge_startResult(bridge);
    Bridge_pushString(bridge, tag);
    Bridge_return(bridge);
    free(tag);
}

// text texture

void ug_baord_create_texture_from_text(Bridge *bridge)
{
    char *font = Bridge_nexString(bridge);
    char *text = Bridge_nexString(bridge);
    float size = Bridge_nexNumber(bridge);
    float spacing = Bridge_nexNumber(bridge);
    TxtInfo info = (TxtInfo) {font, text, size, spacing};
    char *tag = get_texture_tag_for_text(info);
    Texture texture = raylib_create_texture_from_text(info, tag);
    Bridge_startResult(bridge);
    Bridge_pushString(bridge, tag);
    Bridge_return(bridge);
    free(tag);
}

// texture

void ug_baord_draw_texture_by_tag(Bridge *bridge)
{
    char *tag = Bridge_nexString(bridge);
    int x = Bridge_nexNumber(bridge);
    int y = Bridge_nexNumber(bridge);
    float anchorX = Bridge_nexNumber(bridge);
    float anchorY = Bridge_nexNumber(bridge);
    Color color = color_from_bridge(bridge);
    int fromX = Bridge_nexNumber(bridge);
    int fromY = Bridge_nexNumber(bridge);
    int width = Bridge_nexNumber(bridge);
    int height = Bridge_nexNumber(bridge);
    float rotation = Bridge_nexNumber(bridge);
    float scale = Bridge_nexNumber(bridge);
    //
    Texture texture = ralib_get_texture_by_tag(tag);
    raylib_draw_texture_by_texture(texture, x, y, anchorX, anchorY, color, fromX, fromY, width, height, rotation, scale);
    //
    Bridge_startResult(bridge);
    Bridge_return(bridge);
}

// other

void lib_raylib_painter_register(Bridge *bridge)
{
    uyghurBridge = bridge;
    resourcesMap = Hashmap_new(NULL);
    //
    Bridge_startBox(bridge, "ressam");
    // draw
    Bridge_pushKey(bridge, "sizishniBashlash");
    Bridge_pushFunction(bridge, ug_baord_draw_start);
    Bridge_pushKey(bridge, "sizishniTamamlash");
    Bridge_pushFunction(bridge, ug_baord_draw_end);
    // draw point & line
    Bridge_pushKey(bridge, "nuqtaSizish");
    Bridge_pushFunction(bridge, ug_baord_draw_pixel);
    Bridge_pushKey(bridge, "siziqSizish");
    Bridge_pushFunction(bridge, ug_baord_draw_line_no_controll);
    Bridge_pushKey(bridge, "siziqSizishBirKontrolluq");
    Bridge_pushFunction(bridge, ug_baord_draw_line_one_controll);
    Bridge_pushKey(bridge, "siziqSizishIkkiKontirolluq");
    Bridge_pushFunction(bridge, ug_baord_draw_line_two_controll);
    // draw rectangle
    Bridge_pushKey(bridge, "rayunToldurushAylandurulghan");
    Bridge_pushFunction(bridge, ug_baord_draw_rectangle_fill_transformed);
    Bridge_pushKey(bridge, "rayunToldurushRenggareng");
    Bridge_pushFunction(bridge, ug_baord_draw_rectangle_fill_colorful);
    Bridge_pushKey(bridge, "rayunToldurushSiliqlanghan");
    Bridge_pushFunction(bridge, ug_baord_draw_rectangle_fill_round);
    Bridge_pushKey(bridge, "rayunSizishSiliqlanghan");
    Bridge_pushFunction(bridge, ug_baord_draw_rectangle_stroke);
    // draw circle & ring
    Bridge_pushKey(bridge, "chemberToldurush");
    Bridge_pushFunction(bridge, ug_baord_draw_circle_fill);
    Bridge_pushKey(bridge, "chemberSizish");
    Bridge_pushFunction(bridge, ug_baord_draw_circle_stroke);
    Bridge_pushKey(bridge, "uzukToldurush");
    Bridge_pushFunction(bridge, ug_baord_draw_ring_fill);
    Bridge_pushKey(bridge, "uzukSizish");
    Bridge_pushFunction(bridge, ug_baord_draw_ring_stroke);
    // draw triangle & polygon
    Bridge_pushKey(bridge, "uchTereplikToldurush");
    Bridge_pushFunction(bridge, ug_baord_draw_triangle_fill);
    Bridge_pushKey(bridge, "uchTereplikSizish");
    Bridge_pushFunction(bridge, ug_baord_draw_triangle_stroke);
    Bridge_pushKey(bridge, "kopTereplikToldurush");
    Bridge_pushFunction(bridge, ug_baord_draw_polygon_fill);
    Bridge_pushKey(bridge, "kopTereplikSizish");
    Bridge_pushFunction(bridge, ug_baord_draw_polygon_stroke);
    // font
    Bridge_pushKey(bridge, "xetYezish");
    Bridge_pushFunction(bridge, ug_baord_draw_text);
    Bridge_pushKey(bridge, "xetMolcherlesh");
    Bridge_pushFunction(bridge, ug_baord_measure_text);
    // texture
    Bridge_pushKey(bridge, "resimEkirish");
    Bridge_pushFunction(bridge, ug_baord_create_texture_from_image);
    Bridge_pushKey(bridge, "xetEkirish");
    Bridge_pushFunction(bridge, ug_baord_create_texture_from_text);
    Bridge_pushKey(bridge, "tamghaBesish");
    Bridge_pushFunction(bridge, ug_baord_draw_texture_by_tag);
    //
    Bridge_register(bridge);
}
