// baord

#include "./externals.h"
#include "../delegates/header.h"

/////////////////////////////////////////////////

UGColor _ugColor = (UGColor){200, 255, 200, 255};
int _ugRotation = 0;
int _ugThickness = 1;

/////////////////////////////////////////////////

#define _COLOR RCOLOR(_ugColor.r, _ugColor.g, _ugColor.b, _ugColor.a)
#define _CENTER RPOINT(center.x, center.y)
#define _SIZE RSIZE(size.w, size.h)
#define _POINT RPOINT(point.x, point.y)
#define _POINT1 RPOINT(point1.x, point1.y)
#define _POINT2 RPOINT(point2.x, point2.y)
#define _POINT3 RPOINT(point3.x, point3.y)
#define _RECT RRect(rect.x, rect.y, rect.w, rect.h)
#define _RECT2P RPOINT(rect.x, rect.y)
#define _RECT2S RSIZE(rect.w, rect.h)

#define _ROTATION _ugRotation
#define _TARGET __pencilTarget

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
#if USTAGE_USE_SOFT
#define __UG_PENCIL_CALL(name, ...) if (__pencilFocus == UG_PENCIL_FOCUS_NONE) { \
    tools_error("invalid state for pencil"); \
} else if(__pencilFocus == UG_PENCIL_FOCUS_STAGE) { \
    __eReplot->plot = NULL; \
}
#else
#define __UG_PENCIL_CALL(name, ...) if(__pencilFocus == UG_PENCIL_FOCUS_NONE) { \
    tools_error("invalid state for pencil"); \
} else if (__pencilFocus == UG_PENCIL_FOCUS_STAGE) { \
    delegate_pencil_##name(__VA_ARGS__); \
} else 
#endif

void __prepare_soft_paint() {
    bool _needPlot = !USTAGE_USE_SOFT && __pencilFocus == UG_PENCIL_FOCUS_STAGE;
    __eReplot->plot = _needPlot ? deletage_paint_plot : NULL;
}

void __pencil_draw_line(UGPoint point1, UGPoint point2) {
    __UG_PENCIL_CALL(draw_line, point1, point2, _ugThickness, _ugColor) {
        Replot_drawLine(_TARGET, _POINT1, _POINT2, 1);
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
        Replot_setColor(_TARGET, _COLOR);
        Replot_setRotation(_TARGET, _ROTATION);
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
        Replot_drawPoint(_TARGET, _POINT, 1);
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
        Replot_drawTriangle(_TARGET, _POINT1, _POINT2, _POINT3);
    }
    Bridge_returnEmpty(bridge);
}

void native_pencil_fill_triangle(Bridge *bridge)
{
    UGPoint point1 = point_from_bridge(bridge);
    UGPoint point2 = point_from_bridge(bridge);
    UGPoint point3 = point_from_bridge(bridge);
    __UG_PENCIL_CALL(fill_triangle, point1, point2, point3, _ugColor) {
        Replot_fillTriangle(_TARGET, _POINT1, _POINT2, _POINT3);
    }
    Bridge_returnEmpty(bridge);
}

void native_pencil_draw_rectangle(Bridge *bridge)
{
    UGPoint point = point_from_bridge(bridge);
    UGSize size = size_from_bridge(bridge);
    __UG_PENCIL_CALL(draw_rectangle, point, size, _ugColor, _ugThickness) {
        Replot_drawRect(_TARGET, _POINT, _SIZE);
    }
    Bridge_returnEmpty(bridge);
}

void native_pencil_fill_rectangle(Bridge *bridge)
{
    UGPoint point = point_from_bridge(bridge);
    UGSize size = size_from_bridge(bridge);
    __UG_PENCIL_CALL(fill_rectangle, point, size, _ugColor) {
        Replot_fillRect(_TARGET, _POINT, _SIZE);
    }
    Bridge_returnEmpty(bridge);
}

void native_pencil_draw_circle(Bridge *bridge)
{
    UGPoint point = point_from_bridge(bridge);
    double radius = Bridge_receiveNumber(bridge);
    __UG_PENCIL_CALL(draw_circle, point, radius, _ugColor, _ugThickness) {
        Replot_drawCircle(_TARGET, _POINT, radius);
    }
    Bridge_returnEmpty(bridge);
}

void native_pencil_fill_circle(Bridge *bridge)
{
    UGPoint point = point_from_bridge(bridge);
    double radius = Bridge_receiveNumber(bridge);
    __UG_PENCIL_CALL(fill_circle, point, radius, _ugColor) {
        Replot_fillCircle(_TARGET, _POINT, radius);
    }
    Bridge_returnEmpty(bridge);
}

void native_pencil_draw_arc(Bridge *bridge)
{
    UGPoint point = point_from_bridge(bridge);
    double radius = Bridge_receiveNumber(bridge);
    double fromAngle = Bridge_receiveNumber(bridge);
    double toAngle = Bridge_receiveNumber(bridge);
    __prepare_soft_paint();
    Replot_drawArc(_TARGET, _POINT, radius, fromAngle, toAngle);
    Bridge_returnEmpty(bridge);
}

void native_pencil_fill_arc(Bridge *bridge)
{
    UGPoint point = point_from_bridge(bridge);
    double radius = Bridge_receiveNumber(bridge);
    double fromAngle = Bridge_receiveNumber(bridge);
    double toAngle = Bridge_receiveNumber(bridge);
    __prepare_soft_paint();
    Replot_fillArc(_TARGET, _POINT, radius, fromAngle, toAngle);
    Bridge_returnEmpty(bridge);
}

void native_pencil_draw_ring(Bridge *bridge)
{
    UGPoint point = point_from_bridge(bridge);
    double radius = Bridge_receiveNumber(bridge);
    double thickness = Bridge_receiveNumber(bridge);
    __prepare_soft_paint();
    Replot_drawRing(_TARGET, _POINT, radius, thickness);
    Bridge_returnEmpty(bridge);
}

void native_pencil_fill_ring(Bridge *bridge)
{
    UGPoint point = point_from_bridge(bridge);
    double radius = Bridge_receiveNumber(bridge);
    double thickness = Bridge_receiveNumber(bridge);
    __prepare_soft_paint();
    Replot_fillRing(_TARGET, _POINT, radius, thickness);
    Bridge_returnEmpty(bridge);
}

void native_pencil_draw_polygon(Bridge *bridge)
{
    UGPoint point = point_from_bridge(bridge);
    int sides = Bridge_receiveNumber(bridge);
    double radius = Bridge_receiveNumber(bridge);
    __UG_PENCIL_CALL(draw_polygon, point, sides, radius, _ugRotation, _ugColor, _ugThickness) {
        Replot_drawPolygon(_TARGET, sides, _POINT, radius);
    }
    Bridge_returnEmpty(bridge);
}

void native_pencil_fill_polygon(Bridge *bridge)
{
    UGPoint point = point_from_bridge(bridge);
    int sides = Bridge_receiveNumber(bridge);
    double radius = Bridge_receiveNumber(bridge);
    __UG_PENCIL_CALL(fill_polygon, point, sides, radius, _ugRotation, _ugColor) {
        Replot_fillPolygon(_TARGET, sides, _POINT, radius);
    }
    Bridge_returnEmpty(bridge);
}

void native_pencil_draw_star(Bridge *bridge)
{
    UGPoint point = point_from_bridge(bridge);
    double sides = Bridge_receiveNumber(bridge);
    double radius1 = Bridge_receiveNumber(bridge);
    double radius2 = Bridge_receiveNumber(bridge);
    __prepare_soft_paint();
    Replot_drawStar(_TARGET, sides, _POINT, radius1, radius2);
    Bridge_returnEmpty(bridge);
}

void native_pencil_fill_star(Bridge *bridge)
{
    UGPoint point = point_from_bridge(bridge);
    double sides = Bridge_receiveNumber(bridge);
    double radius1 = Bridge_receiveNumber(bridge);
    double radius2 = Bridge_receiveNumber(bridge);
    __prepare_soft_paint();
    Replot_fillStar(_TARGET, sides, _POINT, radius1, radius2);
    Bridge_returnEmpty(bridge);
}

void native_pencil_print_text(Bridge *bridge)
{
    UGPoint point = point_from_bridge(bridge);
    CString text = Bridge_receiveString(bridge);
    int size = Bridge_receiveNumber(bridge);
    // 
    __prepare_soft_paint();
    Replot_printText(__eReplot, _POINT, size, text);
    Bridge_returnEmpty(bridge);
}

void native_pencil_draw_paper(Bridge *bridge)
{
    UGPoint point = point_from_bridge(bridge);
    ECanvas canvas = paper_from_bridge(bridge);
    UGSize size = size_from_bridge(bridge);
    //
    __prepare_soft_paint();
    Replot_drawCanvas(__eReplot, _POINT, _SIZE, canvas);
    Bridge_returnEmpty(bridge);
}

// load resource

void _pencil_release_image(Loadable *loadable) {
    UGImage *image = loadable->obj;
    loadable->obj = NULL;
    if (image) {
        void *data = delegate_unload_image(image);
        if (data) pct_free(data);
        pct_free(image->path);
        rimage_free(image->pxls);
        pct_free(image);
    }
}

void _pencil_release_font(Loadable *loadable) {
    UGFont *font = loadable->obj;
    loadable->obj = NULL;
    if (font) {
        void *data = delegate_unload_font(font);
        if (data) pct_free(data);
        pct_free(font->path);
        pct_free(font);
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
        Replot_setTexture(_TARGET, image->pxls, image->w, image->h);
        Replot_fillRect(_TARGET, _POINT, RSIZE(w, h));

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
#undef _TARGET

//////////////////////////////////////////////////////////
