// baord

#include "./externals.h"
#include "../delegates/header.h"

/////////////////////////////////////////////////

#define _COLOR (RColor){color.r, color.g, color.b, color.a}
#define _VECTOR (RPoint){vector.x, vector.y}
#define _CENTER (RPoint){center.x, center.y}
#define _ANCHOR (RPoint){anchor.x, anchor.y}
#define _POINT (RPoint){point.x, point.y}
#define _POINT1 (RPoint){point1.x, point1.y}
#define _POINT2 (RPoint){point2.x, point2.y}
#define _POINT3 (RPoint){point3.x, point3.y}
#define _RECT (RRect){rect.x, rect.y, rect.w, rect.h}

#define _PAPER __ugPencilTarget

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

#define __UG_PENCIL_CALL(name, ...) if(__ugPencilFocus == UG_PENCIL_FOCUS_STAGE) { \
    delegate_pencil_##name(__VA_ARGS__); \
    } else if (__ugPencilFocus != UG_PENCIL_FOCUS_PAPER) { \
    tools_error("invalid state for pencil"); \
    } else \

void native_pencil_draw_pixel(Bridge *bridge)
{
    UGPoint point = point_from_bridge(bridge);
    UGColor color = color_from_bridge(bridge);
    __UG_PENCIL_CALL(draw_pixel, point, color) {
        Replot_setColor(_PAPER, _COLOR);
        Replot_drawPoint(_PAPER, _POINT, 1);
    }
    Bridge_returnEmpty(bridge);
}

void __pencil_draw_line(UGPoint point1, UGPoint point2, int thickness, UGColor color) {
    __UG_PENCIL_CALL(draw_line, point1, point2, thickness, color) {
        // TODO:fix
        Replot_setColor(_PAPER, _COLOR);
        Replot_drawLine(_PAPER, _POINT1, _POINT2, 1);
    }
}

void __pencil_draw_curve1(UGPoint point1, UGPoint point2, int thickness, UGColor color, UGPoint anchor)
{
    UGPoint _last = (UGPoint){0, 0};
    for (float t = 0; t <= 1; t += 0.01) {
        UGPoint p1 = UG_VECTOR_LERP(point1, anchor, t);
        UGPoint p2 = UG_VECTOR_LERP(anchor, point2, t);
        UGPoint p0 = UG_VECTOR_LERP(p1, p2, t);
        UGPoint _point = (UGPoint){p0.x, p0.y};
        if (t != 0) {
            __pencil_draw_line(_last, _point, thickness, color);
        }
        _last.x = _point.x;
        _last.y = _point.y;
    }
}

void __pencil_draw_curve2(UGPoint point1, UGPoint point2, int thickness, UGColor color, UGPoint anchor1, UGPoint anchor2)
{
    UGPoint _last = (UGPoint){0, 0};
    for (float t = 0; t <= 1; t += 0.01) {
        UGPoint p1 = UG_VECTOR_LERP(point1, anchor1, t);
        UGPoint p2 = UG_VECTOR_LERP(anchor1, anchor2, t);
        UGPoint p3 = UG_VECTOR_LERP(anchor2, point2, t);
        UGPoint _p1 = UG_VECTOR_LERP(p1, p2, t);
        UGPoint _p2 = UG_VECTOR_LERP(p2, p3, t);
        UGPoint p0 = UG_VECTOR_LERP(_p1, _p2, t);
        UGPoint _point = (UGPoint){p0.x, p0.y};
        if (t != 0) {
            __pencil_draw_line(_last, _point, thickness, color);
        }
        _last.x = _point.x;
        _last.y = _point.y;
    }
}

void native_pencil_draw_line(Bridge *bridge)
{
    UGPoint point1 = point_from_bridge(bridge);
    UGPoint point2 = point_from_bridge(bridge);
    int thickness = Bridge_receiveNumber(bridge);
    UGColor color = color_from_bridge(bridge);
    //
    if (Bridge_nextType(bridge) != UG_TYPE_NUM) {
        __pencil_draw_line(point1, point2, thickness, color);
    } else {
        UGPoint controll = point_from_bridge(bridge);
        if (Bridge_nextType(bridge) != UG_TYPE_NUM) {
            __pencil_draw_curve1(point1, point2, thickness, color, controll);
        } else {
            UGPoint _controll = point_from_bridge(bridge);
            __pencil_draw_curve2(point1, point2, thickness, color, controll, _controll);
        }
    }
    Bridge_returnEmpty(bridge);
}

void native_pencil_draw_rectangle_fill(Bridge *bridge)
{
    UGRect rect = rect_from_bridge(bridge);
    UGColor color = color_from_bridge(bridge);
    __UG_PENCIL_CALL(draw_rectangle_fill, rect, color) {
    }
    Bridge_returnEmpty(bridge);
}

void native_pencil_draw_rectangle_fill_transformed(Bridge *bridge)
{
    UGRect rect = rect_from_bridge(bridge);
    UGColor color = color_from_bridge(bridge);
    UGPoint anchor = point_from_bridge(bridge);
    anchor.x = anchor.x * rect.width;
    anchor.y = anchor.y * rect.height;
    float rotation = Bridge_receiveNumber(bridge);
    __UG_PENCIL_CALL(draw_rectangle_fill_transformed, rect, color, anchor, rotation) {
    }
    Bridge_returnEmpty(bridge);
}

void native_pencil_draw_rectangle_fill_colorful(Bridge *bridge)
{
    UGRect rect = rect_from_bridge(bridge);
    UGColor leftTop = color_from_bridge(bridge);
    UGColor leftBottom = color_from_bridge(bridge);
    UGColor rightBottom = color_from_bridge(bridge);
    UGColor rightTop = color_from_bridge(bridge);
    __UG_PENCIL_CALL(draw_rectangle_fill_colorful, rect, leftTop, leftBottom, rightBottom, rightTop) {
    }
    Bridge_returnEmpty(bridge);
}

void native_pencil_draw_rectangle_fill_round(Bridge *bridge)
{
    UGRect rect = rect_from_bridge(bridge);
    UGColor color = color_from_bridge(bridge);
    int roundness = Bridge_receiveNumber(bridge);
    __UG_PENCIL_CALL(draw_rectangle_fill_round, rect, color, roundness) {
    }
    Bridge_returnEmpty(bridge);
}

void native_pencil_draw_rectangle_stroke(Bridge *bridge)
{
    UGRect rect = rect_from_bridge(bridge);
    UGColor color = color_from_bridge(bridge);
    double thickness = Bridge_receiveNumber(bridge);
    __UG_PENCIL_CALL(draw_rectangle_stroke, rect, color, thickness) {
    }
    Bridge_returnEmpty(bridge);
}

void native_pencil_draw_circle_fill(Bridge *bridge)
{
    UGPoint point = point_from_bridge(bridge);
    UGSize size = size_from_bridge(bridge);
    UGColor color = color_from_bridge(bridge);
    __UG_PENCIL_CALL(draw_circle_fill, point, size, color) {
    }
    Bridge_returnEmpty(bridge);
}

void native_pencil_draw_circle_stroke(Bridge *bridge)
{
    UGPoint point = point_from_bridge(bridge);
    UGSize size = size_from_bridge(bridge);
    UGColor color = color_from_bridge(bridge);
    double thickness = Bridge_receiveNumber(bridge);
    __UG_PENCIL_CALL(draw_circle_stroke, point, size, color, thickness) {
    }
    Bridge_returnEmpty(bridge);
}

void native_pencil_draw_triangle_fill(Bridge *bridge)
{
    UGPoint point1 = point_from_bridge(bridge);
    UGPoint point2 = point_from_bridge(bridge);
    UGPoint point3 = point_from_bridge(bridge);
    UGColor color = color_from_bridge(bridge);
    __UG_PENCIL_CALL(draw_triangle_fill, point1, point2, point3, color) {
    }
    Bridge_returnEmpty(bridge);
}

void native_pencil_draw_triangle_stroke(Bridge *bridge)
{
    UGPoint point1 = point_from_bridge(bridge);
    UGPoint point2 = point_from_bridge(bridge);
    UGPoint point3 = point_from_bridge(bridge);
    UGColor color = color_from_bridge(bridge);
    double thickness = Bridge_receiveNumber(bridge);
    __UG_PENCIL_CALL(draw_triangle_stroke, point1, point2, point3, color, thickness) {
    }
    Bridge_returnEmpty(bridge);
}

void native_pencil_draw_polygon_fill(Bridge *bridge)
{
    UGPoint center = point_from_bridge(bridge);
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
    UGPoint center = point_from_bridge(bridge);
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
        UGImage *data = delegate_unload_image(loadable->obj);
        pct_free(data->path);
        pct_free(data);
        pct_free(loadable->obj);
        loadable->obj = NULL;
    }
}

void _pencil_release_font(Loadable *loadable) {
    if (loadable->obj) {
        UGFont *data = delegate_unload_font(loadable->obj);
        pct_free(data->path);
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
    UGPoint position = point_from_bridge(bridge);
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

/////////////////////////////////////////////////

#undef _COLOR
#undef _VECTOR
#undef _CENTER
#undef _ANCHOR
#undef _POINT
#undef _POINT1
#undef _POINT2
#undef _POINT3
#undef _RECT

#undef _PAPER

//////////////////////////////////////////////////////////
