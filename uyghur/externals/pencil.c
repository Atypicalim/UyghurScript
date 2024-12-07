// baord

#include "./externals.h"
#include "../delegates/header.h"

/////////////////////////////////////////////////

UGColor _ugColor = (UGColor){200, 255, 200, 255};
int _ugRotation = 0;
int _ugThickness = 1;

/////////////////////////////////////////////////

#define _COLOR __eColor
#define _CENTER center.x, center.y
#define _SIZE size.w, size.h
#define _POINT point.x, point.y
#define _POINT1 point1.x, point1.y
#define _POINT2 point2.x, point2.y
#define _POINT3 point3.x, point3.y
#define _RECT rect.x, rect.y, rect.w, rect.h
#define _RECT2P rect.x, rect.y
#define _RECT2S rect.w, rect.h

#define _ROTATION _ugRotation
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


// TODO:wrap tigr as replot
#if USE_SOFT_PAINT
#define __UG_PENCIL_CALL(name, ...) if (__ugPencilFocus == UG_PENCIL_FOCUS_NONE) { \
    tools_error("invalid state for pencil"); \
} else if(__ugPencilFocus == UG_PENCIL_FOCUS_STAGE) { \
}
#else
#define __UG_PENCIL_CALL(name, ...) if(__ugPencilFocus == UG_PENCIL_FOCUS_NONE) { \
    tools_error("invalid state for pencil"); \
} else if (__ugPencilFocus == UG_PENCIL_FOCUS_STAGE) { \
    delegate_pencil_##name(__VA_ARGS__); \
} else 
#endif

void __pencil_draw_line(UGPoint point1, UGPoint point2) {
    __UG_PENCIL_CALL(draw_line, point1, point2, _ugThickness, _ugColor) {
        pntr_draw_line(_PAPER, _POINT1, _POINT2, _COLOR);
    }
}

void __pencil_draw_curve1(UGPoint point1, UGPoint point2, UGPoint anchor)
{
    UGPoint _last = (UGPoint){0, 0};
    for (float t = 0; t <= 1; t += 0.01) {
        UGPoint p1 = UG_VECTOR_LERP(point1, anchor, t);
        UGPoint p2 = UG_VECTOR_LERP(anchor, point2, t);
        UGPoint p0 = UG_VECTOR_LERP(p1, p2, t);
        UGPoint _point = (UGPoint){p0.x, p0.y};
        if (t != 0) {
            __pencil_draw_line(_last, _point);
        }
        _last.x = _point.x;
        _last.y = _point.y;
    }
}

void __pencil_draw_curve2(UGPoint point1, UGPoint point2, UGPoint anchor1, UGPoint anchor2)
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
            __pencil_draw_line(_last, _point);
        }
        _last.x = _point.x;
        _last.y = _point.y;
    }
}

void __pencil_apply_styles() {
    __UG_PENCIL_CALL(customize, _ugColor, _ugRotation) {
        __eColor.rgba.r = _ugColor.r;
        __eColor.rgba.g = _ugColor.g;
        __eColor.rgba.b = _ugColor.b;
        __eColor.rgba.a = _ugColor.a;
    }
}

void native_pencil_facelize(Bridge *bridge) {
    Loadable *paper = Bridge_receiveValue(bridge, UG_TYPE_NON);
    if (Loadable_isStuf(paper) && paper->linka == __release_paper) {
        pencil_focus_to(UG_PENCIL_FOCUS_PAPER, paper->obj);
    } else {
        pencil_focus_to(UG_PENCIL_FOCUS_NONE, NULL);
    }
    __pencil_apply_styles();
    Bridge_returnEmpty(bridge);
}

void native_pencil_customize(Bridge *bridge) {
    // 
    UGColor color = color_from_bridge(bridge);
    _ugColor = color;
    // 
    if (Bridge_nextType(bridge) == UG_TYPE_NUM) {
        int rotation = Bridge_receiveNumber(bridge);
        _ugRotation = rotation;
    }
    //
    __pencil_apply_styles();
    Bridge_returnEmpty(bridge);
}

void native_pencil_draw_pixel(Bridge *bridge)
{
    UGPoint point = point_from_bridge(bridge);
    __UG_PENCIL_CALL(draw_pixel, point, _ugColor) {
        pntr_draw_point(_PAPER, _POINT, _COLOR);
    }
    Bridge_returnEmpty(bridge);
}

void native_pencil_draw_line(Bridge *bridge)
{
    UGPoint point1 = point_from_bridge(bridge);
    UGPoint point2 = point_from_bridge(bridge);
    //
    if (Bridge_nextType(bridge) != UG_TYPE_NUM) {
        __pencil_draw_line(point1, point2);
    } else {
        UGPoint controll1 = point_from_bridge(bridge);
        if (Bridge_nextType(bridge) != UG_TYPE_NUM) {
            __pencil_draw_curve1(point1, point2, controll1);
        } else {
            UGPoint controll2 = point_from_bridge(bridge);
            __pencil_draw_curve2(point1, point2, controll1, controll2);
        }
    }
    Bridge_returnEmpty(bridge);
}

void native_pencil_draw_triangle(Bridge *bridge)
{
    UGPoint point1 = point_from_bridge(bridge);
    UGPoint point2 = point_from_bridge(bridge);
    UGPoint point3 = point_from_bridge(bridge);
    __UG_PENCIL_CALL(draw_triangle, point1, point2, point3, _ugColor, _ugThickness) {
        pntr_draw_triangle(_PAPER, _POINT1, _POINT2, _POINT3, _COLOR);
    }
    Bridge_returnEmpty(bridge);
}

void native_pencil_fill_triangle(Bridge *bridge)
{
    UGPoint point1 = point_from_bridge(bridge);
    UGPoint point2 = point_from_bridge(bridge);
    UGPoint point3 = point_from_bridge(bridge);
    __UG_PENCIL_CALL(fill_triangle, point1, point2, point3, _ugColor) {
        pntr_draw_triangle_fill(_PAPER, _POINT1, _POINT2, _POINT3, _COLOR);
    }
    Bridge_returnEmpty(bridge);
}

void native_pencil_draw_rectangle(Bridge *bridge)
{
    UGPoint point = point_from_bridge(bridge);
    UGSize size = size_from_bridge(bridge);
    __UG_PENCIL_CALL(draw_rectangle, point, size, _ugColor, _ugThickness) {
        int x = point.x - size.w / 2;
        int y = point.y - size.h / 2;
        pntr_draw_rectangle(_PAPER, x, y, _SIZE, _COLOR);
    }
    Bridge_returnEmpty(bridge);
}

void native_pencil_fill_rectangle(Bridge *bridge)
{
    UGPoint point = point_from_bridge(bridge);
    UGSize size = size_from_bridge(bridge);
    __UG_PENCIL_CALL(fill_rectangle, point, size, _ugColor) {
        int x = point.x - size.w / 2;
        int y = point.y - size.h / 2;
        pntr_draw_rectangle_fill(_PAPER, x, y, _SIZE, _COLOR);
    }
    Bridge_returnEmpty(bridge);
}

void native_pencil_draw_circle(Bridge *bridge)
{
    UGPoint point = point_from_bridge(bridge);
    double radius = Bridge_receiveNumber(bridge);
    __UG_PENCIL_CALL(draw_circle, point, radius, _ugColor, _ugThickness) {
        pntr_draw_circle(_PAPER, _POINT, radius, _COLOR);
    }
    Bridge_returnEmpty(bridge);
}

void native_pencil_fill_circle(Bridge *bridge)
{
    UGPoint point = point_from_bridge(bridge);
    double radius = Bridge_receiveNumber(bridge);
    __UG_PENCIL_CALL(fill_circle, point, radius, _ugColor) {
        pntr_draw_circle_fill(_PAPER, _POINT, radius, _COLOR);
    }
    Bridge_returnEmpty(bridge);
}

#define _EPOLYGON_MAX_SIDE 64
#define _EPOLYGON_MAX_RADIUS 1024
pntr_vector __ePolygonPoints[_EPOLYGON_MAX_SIDE];
void __eGeneartePolygonPoints(UGPoint *point, int *sides, int radius) {
    *sides = MIN(_EPOLYGON_MAX_SIDE, *sides);
    radius = MIN(_EPOLYGON_MAX_RADIUS, radius);
    double angleIncrement = (2 * M_PI) / *sides; // Angle between each vertex
    for (int i = 0; i < *sides; i++) {
        double angle = i * angleIncrement; // Current angle
        __ePolygonPoints[i].x = point->x + radius * cos(angle); // x-coordinate
        __ePolygonPoints[i].y = point->y + radius * sin(angle); // y-coordinate
    }
}

void native_pencil_draw_polygon(Bridge *bridge)
{
    UGPoint point = point_from_bridge(bridge);
    int sides = Bridge_receiveNumber(bridge);
    double radius = Bridge_receiveNumber(bridge);
    __UG_PENCIL_CALL(draw_polygon, point, sides, radius, _ugRotation, _ugColor, _ugThickness) {
        __eGeneartePolygonPoints(&point, &sides, radius);
        pntr_draw_polygon(_PAPER, &__ePolygonPoints, sides, _COLOR);
    }
    Bridge_returnEmpty(bridge);
}

void native_pencil_fill_polygon(Bridge *bridge)
{
    UGPoint point = point_from_bridge(bridge);
    int sides = Bridge_receiveNumber(bridge);
    double radius = Bridge_receiveNumber(bridge);
    __UG_PENCIL_CALL(fill_polygon, point, sides, radius, _ugRotation, _ugColor) {
        __eGeneartePolygonPoints(&point, &sides, radius);
        pntr_draw_polygon_fill(_PAPER, &__ePolygonPoints, sides, _COLOR);
    }
    Bridge_returnEmpty(bridge);
}

void native_pencil_print_text(Bridge *bridge)
{
    UGPoint point = point_from_bridge(bridge);
    CString text = Bridge_receiveString(bridge);
    double size = Bridge_receiveNumber(bridge);
    // 
    if (__ugPencilFocus == UG_PENCIL_FOCUS_PAPER) {
        pntr_draw_text(_PAPER, __eFont, text, _POINT, _COLOR);
    } else {
        pntr_draw_text(__ePaper, __eFont, text, _POINT, _COLOR);
    }
    Bridge_returnEmpty(bridge);
}

void native_pencil_draw_paper(Bridge *bridge)
{
    UGPoint point = point_from_bridge(bridge);
    EPaper paper = paper_from_bridge(bridge);
    UGSize size = size_from_bridge(bridge);
    // 
    if (__ugPencilFocus == UG_PENCIL_FOCUS_PAPER) {
        pntr_draw_image(_PAPER, paper, _POINT);
    } else {
        pntr_draw_image(__ePaper, paper, _POINT);
    }
    Bridge_returnEmpty(bridge);
}

// load resource

void _pencil_release_image(Loadable *loadable) {
    if (loadable->obj) {
        UGImage *data = delegate_unload_image(loadable->obj);
        pct_free(data->path);
        rimage_free(data->pxls);
        // TODO:free texture
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
    UGImage *image = UG_NEW_IMAGE(path);
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

// font

void native_pencil_draw_font(Bridge *bridge)
{
    UGFont *font = font_from_bridge(bridge);
    float size = Bridge_receiveNumber(bridge);
    CString text = Bridge_receiveString(bridge);
    UGPoint point = point_from_bridge(bridge);
    __UG_PENCIL_CALL(draw_font, font, text, size, _ugColor, point) {
        // font not supported
    }
    Bridge_returnEmpty(bridge);
}

// image

void native_pencil_draw_image(Bridge *bridge)
{
    UGImage *image = image_from_bridge(bridge);
    UGPoint point = point_from_bridge(bridge);
    int w = 0;
    int h = 0;
    if (Bridge_nextType(bridge) == UG_TYPE_NUM) {
        UGSize size = size_from_bridge(bridge);
        w = size.w;
        h = size.h;
    } else {
        w = image->w;
        h = image->h;
    }
    __UG_PENCIL_CALL(draw_image, image, point.x, point.y, 0.5, 0.5, _ugColor, _ugRotation, 1) {
        int _size = image->w * image->w * image->c * sizeof(unsigned char);
        EPaper *paper = pntr_load_image_from_memory(PNTR_IMAGE_TYPE_PNG, image->pxls, _size);
        pntr_draw_image(_PAPER, paper, _POINT);
        EPaper_free(paper);
    }
    Bridge_returnEmpty(bridge);
}

/////////////////////////////////////////////////

#undef _COLOR
#undef _CENTER
#undef _SIZE
#undef _POINT
#undef _POINT1
#undef _POINT2
#undef _POINT3
#undef _RECT
#undef _RECT2P
#undef _RECT2S

#undef _ROTATION
#undef _PAPER

//////////////////////////////////////////////////////////
