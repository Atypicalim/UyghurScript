// baord

#include "../delegates/header.h"
#include "../uyghur.c"

// tool

UGColor color_from_bridge(Bridge *bridge)
{
    CString str = Bridge_receiveString(bridge);
    int len = strlen(str);
    if (len != 6 && len != 8) return (UGColor){0, 0, 0, 255};
    int r = char_to_int(str[0]) * 16 + char_to_int(str[1]);
    int g = char_to_int(str[2]) * 16 + char_to_int(str[3]);
    int b = char_to_int(str[4]) * 16 + char_to_int(str[5]);
    int a = len == 6 ? 255 : char_to_int(str[6]) * 16 + char_to_int(str[7]);;
    return (UGColor){r, g, b, a};
}

UGVector vector_from_bridge(Bridge *bridge)
{
    float x = Bridge_receiveNumber(bridge);
    float y = Bridge_receiveNumber(bridge);
    return (UGVector){x, y};
}

UGSize size_from_bridge(Bridge *bridge)
{
    float w = Bridge_receiveNumber(bridge);
    float h = Bridge_receiveNumber(bridge);
    return (UGSize){w, h};
}

UGRectangle rectangle_from_bridge(Bridge *bridge)
{
    float x = Bridge_receiveNumber(bridge);
    float y = Bridge_receiveNumber(bridge);
    float w = Bridge_receiveNumber(bridge);
    float h = Bridge_receiveNumber(bridge);
    return (UGRectangle){x, y, w, h};
}

/////////////////////////////////////////////////

UGImage *image_from_bridge(Bridge *bridge)
{
    Loadable *loadable = Bridge_receiveValue(bridge, UG_TYPE_STF);
    UGImage *image = loadable->obj;
    return image;
}

UGFont *font_from_bridge(Bridge *bridge)
{
    Loadable *loadable = Bridge_receiveValue(bridge, UG_TYPE_STF);
    UGFont *font = loadable->obj;
    return font;
}

/////////////////////////////////////////////////

// draw

#define __UG_PENCIL_CALL(name, ...) if(true) { delegate_pencil_##name(__VA_ARGS__); } else

void native_pencil_draw_pixel(Bridge *bridge)
{
    UGVector poit = vector_from_bridge(bridge);
    UGColor color = color_from_bridge(bridge);
    __UG_PENCIL_CALL(draw_pixel, poit, color) {

    }
    Bridge_returnEmpty(bridge);
}

void __pencil_draw_line(UGVector point1, UGVector point2, int thickness, UGColor color) {
    __UG_PENCIL_CALL(draw_line, point1, point2, thickness, color) {
    }
}

void __pencil_draw_curve1(UGVector point1, UGVector point2, int thickness, UGColor color, UGVector anchor)
{
    UGVector _last = (UGVector){0, 0};
    for (float t = 0; t <= 1; t += 0.01) {
        UGVector p1 = UG_VECTOR_LERP(point1, anchor, t);
        UGVector p2 = UG_VECTOR_LERP(anchor, point2, t);
        UGVector p0 = UG_VECTOR_LERP(p1, p2, t);
        UGVector _point = (UGVector){p0.x, p0.y};
        if (t != 0) {
            __pencil_draw_line(_last, _point, thickness, color);
        }
        _last.x = _point.x;
        _last.y = _point.y;
    }
}

void __pencil_draw_curve2(UGVector point1, UGVector point2, int thickness, UGColor color, UGVector anchor1, UGVector anchor2)
{
    UGVector _last = (UGVector){0, 0};
    for (float t = 0; t <= 1; t += 0.01) {
        UGVector p1 = UG_VECTOR_LERP(point1, anchor1, t);
        UGVector p2 = UG_VECTOR_LERP(anchor1, anchor2, t);
        UGVector p3 = UG_VECTOR_LERP(anchor2, point2, t);
        UGVector _p1 = UG_VECTOR_LERP(p1, p2, t);
        UGVector _p2 = UG_VECTOR_LERP(p2, p3, t);
        UGVector p0 = UG_VECTOR_LERP(_p1, _p2, t);
        UGVector _point = (UGVector){p0.x, p0.y};
        if (t != 0) {
            __pencil_draw_line(_last, _point, thickness, color);
        }
        _last.x = _point.x;
        _last.y = _point.y;
    }
}

void native_pencil_draw_line(Bridge *bridge)
{
    UGVector point1 = vector_from_bridge(bridge);
    UGVector point2 = vector_from_bridge(bridge);
    int thickness = Bridge_receiveNumber(bridge);
    UGColor color = color_from_bridge(bridge);
    //
    if (Bridge_nextType(bridge) != UG_TYPE_NUM) {
        __pencil_draw_line(point1, point2, thickness, color);
    } else {
        UGVector controll = vector_from_bridge(bridge);
        if (Bridge_nextType(bridge) != UG_TYPE_NUM) {
            __pencil_draw_curve1(point1, point2, thickness, color, controll);
        } else {
            UGVector _controll = vector_from_bridge(bridge);
            __pencil_draw_curve2(point1, point2, thickness, color, controll, _controll);
        }
    }
    Bridge_returnEmpty(bridge);
}

void native_pencil_draw_rectangle_fill(Bridge *bridge)
{
    UGRectangle rectangle = rectangle_from_bridge(bridge);
    UGColor color = color_from_bridge(bridge);
    __UG_PENCIL_CALL(draw_rectangle_fill, rectangle, color) {
    }
    Bridge_returnEmpty(bridge);
}

void native_pencil_draw_rectangle_fill_transformed(Bridge *bridge)
{
    UGRectangle rectangle = rectangle_from_bridge(bridge);
    UGColor color = color_from_bridge(bridge);
    UGVector anchor = vector_from_bridge(bridge);
    anchor.x = anchor.x * rectangle.width;
    anchor.y = anchor.y * rectangle.height;
    float rotation = Bridge_receiveNumber(bridge);
    __UG_PENCIL_CALL(draw_rectangle_fill_transformed, rectangle, color, anchor, rotation) {
    }
    Bridge_returnEmpty(bridge);
}

void native_pencil_draw_rectangle_fill_colorful(Bridge *bridge)
{
    UGRectangle rectangle = rectangle_from_bridge(bridge);
    UGColor leftTop = color_from_bridge(bridge);
    UGColor leftBottom = color_from_bridge(bridge);
    UGColor rightBottom = color_from_bridge(bridge);
    UGColor rightTop = color_from_bridge(bridge);
    __UG_PENCIL_CALL(draw_rectangle_fill_colorful, rectangle, leftTop, leftBottom, rightBottom, rightTop) {
    }
    Bridge_returnEmpty(bridge);
}

void native_pencil_draw_rectangle_fill_round(Bridge *bridge)
{
    UGRectangle rectangle = rectangle_from_bridge(bridge);
    UGColor color = color_from_bridge(bridge);
    int roundness = Bridge_receiveNumber(bridge);
    __UG_PENCIL_CALL(draw_rectangle_fill_round, rectangle, color, roundness) {
    }
    Bridge_returnEmpty(bridge);
}

void native_pencil_draw_rectangle_stroke(Bridge *bridge)
{
    UGRectangle rectangle = rectangle_from_bridge(bridge);
    UGColor color = color_from_bridge(bridge);
    double thickness = Bridge_receiveNumber(bridge);
    __UG_PENCIL_CALL(draw_rectangle_stroke, rectangle, color, thickness) {
    }
    Bridge_returnEmpty(bridge);
}

void native_pencil_draw_circle_fill(Bridge *bridge)
{
    UGVector point = vector_from_bridge(bridge);
    UGSize size = size_from_bridge(bridge);
    UGColor color = color_from_bridge(bridge);
    __UG_PENCIL_CALL(draw_circle_fill, point, size, color) {
    }
    Bridge_returnEmpty(bridge);
}

void native_pencil_draw_circle_stroke(Bridge *bridge)
{
    UGVector point = vector_from_bridge(bridge);
    UGSize size = size_from_bridge(bridge);
    UGColor color = color_from_bridge(bridge);
    double thickness = Bridge_receiveNumber(bridge);
    __UG_PENCIL_CALL(draw_circle_stroke, point, size, color, thickness) {
    }
    Bridge_returnEmpty(bridge);
}

void native_pencil_draw_triangle_fill(Bridge *bridge)
{
    UGVector point1 = vector_from_bridge(bridge);
    UGVector point2 = vector_from_bridge(bridge);
    UGVector point3 = vector_from_bridge(bridge);
    UGColor color = color_from_bridge(bridge);
    __UG_PENCIL_CALL(draw_triangle_fill, point1, point2, point3, color) {
    }
    Bridge_returnEmpty(bridge);
}

void native_pencil_draw_triangle_stroke(Bridge *bridge)
{
    UGVector point1 = vector_from_bridge(bridge);
    UGVector point2 = vector_from_bridge(bridge);
    UGVector point3 = vector_from_bridge(bridge);
    UGColor color = color_from_bridge(bridge);
    double thickness = Bridge_receiveNumber(bridge);
    __UG_PENCIL_CALL(draw_triangle_stroke, point1, point2, point3, color, thickness) {
    }
    Bridge_returnEmpty(bridge);
}

void native_pencil_draw_polygon_fill(Bridge *bridge)
{
    UGVector center = vector_from_bridge(bridge);
    int sides = Bridge_receiveNumber(bridge);
    double radius = Bridge_receiveNumber(bridge);
    double rotation = Bridge_receiveNumber(bridge);
    UGColor color = color_from_bridge(bridge);
    __UG_PENCIL_CALL(draw_polygon_fill, center, sides, radius, rotation, color) {
    }
    Bridge_returnEmpty(bridge);
}

void native_pencil_draw_polygon_stroke(Bridge *bridge)
{
    UGVector center = vector_from_bridge(bridge);
    int sides = Bridge_receiveNumber(bridge);
    double radius = Bridge_receiveNumber(bridge);
    double rotation = Bridge_receiveNumber(bridge);
    UGColor color = color_from_bridge(bridge);
    double thickness = Bridge_receiveNumber(bridge);
    __UG_PENCIL_CALL(draw_polygon_stroke, center, sides, radius, rotation, color, thickness) {
    }
    Bridge_returnEmpty(bridge);
}

// load resource

void _pencil_release_image(Loadable *loadable) {
    if (loadable->obj) {
        void *data = delegate_unload_image(loadable->obj);
        pct_free(data);
        pct_free(loadable->obj);
        loadable->obj = NULL;
    }
}

void _pencil_release_font(Loadable *loadable) {
    if (loadable->obj) {
        void *data = delegate_unload_font(loadable->obj);
        pct_free(data);
        pct_free(loadable->obj);
        loadable->obj = NULL;
    }
}

void native_pencil_load_image(Bridge *bridge)
{
    CString path = Bridge_receiveString(bridge);
    bool needArea = Bridge_nextType(bridge) == UG_TYPE_NUM;
    double x = needArea ? Bridge_receiveNumber(bridge) : 0;
    double y = needArea ? Bridge_receiveNumber(bridge) : 0;
    double w = needArea ? Bridge_receiveNumber(bridge) : -1;
    double h = needArea ? Bridge_receiveNumber(bridge) : -1;
    UGImage *image = UG_NEW_IMAGE(path, x, y, w, h);
    delegate_load_image(image);
    Loadable *loadable = Loadable_newStuf(image, ALIAS_texture, path, _pencil_release_image);
    Bridge_returnValue(bridge, loadable);
}

void native_pencil_load_font(Bridge *bridge)
{
    CString path = Bridge_receiveString(bridge);
    bool needSize = Bridge_nextType(bridge) == UG_TYPE_NUM;
    double size = needSize ? Bridge_receiveNumber(bridge) : 48;
    UGFont *font = UG_NEW_FONT(path, size);
    delegate_load_font(font);
    Loadable *loadable = Loadable_newStuf(font, ALIAS_texture, path, _pencil_release_font);
    Bridge_returnValue(bridge, loadable);
}

// text

void native_pencil_draw_text(Bridge *bridge)
{
    UGFont *font = font_from_bridge(bridge);
    CString text = Bridge_receiveString(bridge);
    float size = Bridge_receiveNumber(bridge);
    float spacing = Bridge_receiveNumber(bridge);
    UGColor color = color_from_bridge(bridge);
    UGVector position = vector_from_bridge(bridge);
    delegate_draw_font(font, text, size, spacing, color, position);
    Bridge_returnEmpty(bridge);
}

void native_pencil_measure_text(Bridge *bridge)
{
    UGFont *font = font_from_bridge(bridge);
    CString text = Bridge_receiveString(bridge);
    float size = Bridge_receiveNumber(bridge);
    float spacing = Bridge_receiveNumber(bridge);
    int width = delegate_measure_font(font, text, size, spacing);
    Bridge_returnNumber(bridge, width);
}

// image

void native_pencil_draw_image(Bridge *bridge)
{
    UGImage *image = image_from_bridge(bridge);
    int x = Bridge_receiveNumber(bridge);
    int y = Bridge_receiveNumber(bridge);
    float anchorX = Bridge_receiveNumber(bridge);
    float anchorY = Bridge_receiveNumber(bridge);
    UGColor color = color_from_bridge(bridge);
    float rotation = Bridge_receiveNumber(bridge);
    float scale = Bridge_receiveNumber(bridge);
    delegate_draw_image(image, x, y, anchorX, anchorY, color, rotation, scale);
    Bridge_returnEmpty(bridge);
}
