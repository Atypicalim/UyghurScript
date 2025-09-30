// baord

#include "../others/header.h"
#include "./externals.h"
#include "../delegates/header.h"

/////////////////////////////////////////////////

#define _COLOR RCOLOR(gPenColor.r, gPenColor.g, gPenColor.b, gPenColor.a)
#define _CENTER RPOINT(center.x, center.y)
#define _SIZE RSIZE(size.w, size.h)
#define _POINT RPOINT(point.x, point.y)
#define _POINT1 RPOINT(point1.x, point1.y)
#define _POINT2 RPOINT(point2.x, point2.y)
#define _POINT3 RPOINT(point3.x, point3.y)
#define _RECT RRect(rect.x, rect.y, rect.w, rect.h)
#define _RECT2P RPOINT(rect.x, rect.y)
#define _RECT2S RSIZE(rect.w, rect.h)

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
#define __UG_PENCIL_CALL(name, ...) if (gPenFocus == UG_PENCIL_FOCUS_NONE) { \
    tools_error("invalid state for pencil"); \
} else if(gPenFocus == UG_PENCIL_FOCUS_STAGE) { \
    __eReplot->plot = NULL; \
}
#else
#define __UG_PENCIL_CALL(name, ...) if(gPenFocus == UG_PENCIL_FOCUS_NONE) { \
    tools_error("invalid state for pencil"); \
} else if (gPenFocus == UG_PENCIL_FOCUS_STAGE) { \
    delegate_pencil_##name(__VA_ARGS__); \
} else 
#endif

/////////////////////////////////////////////////

void __pencil_draw_line(UGPoint point1, UGPoint point2) {
    __UG_PENCIL_CALL(draw_line, point1, point2) {
        Replot_drawLine(gPenTarget, _POINT1, _POINT2, 1);
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

/////////////////////////////////////////////////

void __pencil_apply_styles() {
    __UG_PENCIL_CALL(customize, gPenColor, gPenRotation) {
        Replot_setColor(gPenTarget, _COLOR);
        Replot_setRotation(gPenTarget, gPenRotation);
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
    gPenColor = color;
    // 
    if (Bridge_nextType(bridge) != UG_TYPE_NON) {
        int rotation = Bridge_receiveNumber(bridge);
        gPenRotation = rotation;
    }
    //
    if (Bridge_nextType(bridge) != UG_TYPE_NON) {
        float scale = Bridge_receiveNumber(bridge);
        gPenScale = scale;
    }
    //
    if (Bridge_nextType(bridge) != UG_TYPE_NON) {
        int thickness = Bridge_receiveNumber(bridge);
        gPenThickness = thickness;
    }
    //
    __pencil_apply_styles();
    Bridge_returnEmpty(bridge);
}

/////////////////////////////////////////////////

void __pencil_prepare_paint() {
    bool _needPlot = !USTAGE_USE_SOFT && gPenFocus == UG_PENCIL_FOCUS_STAGE;
    __eReplot->plot = _needPlot ? deletage_paint_plot : NULL;
}

/////////////////////////////////////////////////

void native_pencil_draw_pixel(Bridge *bridge)
{
    UGPoint point = point_from_bridge(bridge);
    __UG_PENCIL_CALL(draw_pixel, point) {
        Replot_drawPoint(gPenTarget, _POINT, 1);
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
    __pencil_apply_styles();
    Bridge_returnEmpty(bridge);
}

void native_pencil_draw_triangle(Bridge *bridge)
{
    UGPoint point1 = point_from_bridge(bridge);
    UGPoint point2 = point_from_bridge(bridge);
    UGPoint point3 = point_from_bridge(bridge);
    __UG_PENCIL_CALL(draw_triangle, point1, point2, point3) {
        Replot_drawTriangle(gPenTarget, _POINT1, _POINT2, _POINT3);
    }
    __pencil_apply_styles();
    Bridge_returnEmpty(bridge);
}

void native_pencil_fill_triangle(Bridge *bridge)
{
    UGPoint point1 = point_from_bridge(bridge);
    UGPoint point2 = point_from_bridge(bridge);
    UGPoint point3 = point_from_bridge(bridge);
    __UG_PENCIL_CALL(fill_triangle, point1, point2, point3) {
        Replot_fillTriangle(gPenTarget, _POINT1, _POINT2, _POINT3);
    }
    __pencil_apply_styles();
    Bridge_returnEmpty(bridge);
}

void native_pencil_draw_rectangle(Bridge *bridge)
{
    UGPoint point = point_from_bridge(bridge);
    UGSize size = size_from_bridge(bridge);
    __UG_PENCIL_CALL(draw_rectangle, point, size) {
        Replot_drawRect(gPenTarget, _POINT, _SIZE);
    }
    __pencil_apply_styles();
    Bridge_returnEmpty(bridge);
}

void native_pencil_fill_rectangle(Bridge *bridge)
{
    UGPoint point = point_from_bridge(bridge);
    UGSize size = size_from_bridge(bridge);
    __UG_PENCIL_CALL(fill_rectangle, point, size) {
        Replot_fillRect(gPenTarget, _POINT, _SIZE);
    }
    __pencil_apply_styles();
    Bridge_returnEmpty(bridge);
}

void native_pencil_draw_circle(Bridge *bridge)
{
    UGPoint point = point_from_bridge(bridge);
    double radius = Bridge_receiveNumber(bridge);
    __UG_PENCIL_CALL(draw_circle, point, radius) {
        Replot_drawCircle(gPenTarget, _POINT, radius);
    }
    __pencil_apply_styles();
    Bridge_returnEmpty(bridge);
}

void native_pencil_fill_circle(Bridge *bridge)
{
    UGPoint point = point_from_bridge(bridge);
    double radius = Bridge_receiveNumber(bridge);
    __UG_PENCIL_CALL(fill_circle, point, radius) {
        Replot_fillCircle(gPenTarget, _POINT, radius);
    }
    __pencil_apply_styles();
    Bridge_returnEmpty(bridge);
}

void native_pencil_draw_arc(Bridge *bridge)
{
    UGPoint point = point_from_bridge(bridge);
    double radius = Bridge_receiveNumber(bridge);
    double fromAngle = Bridge_receiveNumber(bridge);
    double toAngle = Bridge_receiveNumber(bridge);
    __pencil_prepare_paint();
    Replot_drawArc(gPenTarget, _POINT, radius, fromAngle, toAngle);
    __pencil_apply_styles();
    Bridge_returnEmpty(bridge);
}

void native_pencil_fill_arc(Bridge *bridge)
{
    UGPoint point = point_from_bridge(bridge);
    double radius = Bridge_receiveNumber(bridge);
    double fromAngle = Bridge_receiveNumber(bridge);
    double toAngle = Bridge_receiveNumber(bridge);
    __pencil_prepare_paint();
    Replot_fillArc(gPenTarget, _POINT, radius, fromAngle, toAngle);
    __pencil_apply_styles();
    Bridge_returnEmpty(bridge);
}

void native_pencil_draw_ring(Bridge *bridge)
{
    UGPoint point = point_from_bridge(bridge);
    double radius = Bridge_receiveNumber(bridge);
    double thickness = Bridge_receiveNumber(bridge);
    __pencil_prepare_paint();
    Replot_drawRing(gPenTarget, _POINT, radius, thickness);
    __pencil_apply_styles();
    Bridge_returnEmpty(bridge);
}

void native_pencil_fill_ring(Bridge *bridge)
{
    UGPoint point = point_from_bridge(bridge);
    double radius = Bridge_receiveNumber(bridge);
    double thickness = Bridge_receiveNumber(bridge);
    __pencil_prepare_paint();
    Replot_fillRing(gPenTarget, _POINT, radius, thickness);
    __pencil_apply_styles();
    Bridge_returnEmpty(bridge);
}

void native_pencil_draw_polygon(Bridge *bridge)
{
    UGPoint point = point_from_bridge(bridge);
    int sides = Bridge_receiveNumber(bridge);
    double radius = Bridge_receiveNumber(bridge);
    __UG_PENCIL_CALL(draw_polygon, point, sides, radius) {
        Replot_drawPolygon(gPenTarget, sides, _POINT, radius);
    }
    __pencil_apply_styles();
    Bridge_returnEmpty(bridge);
}

void native_pencil_fill_polygon(Bridge *bridge)
{
    UGPoint point = point_from_bridge(bridge);
    int sides = Bridge_receiveNumber(bridge);
    double radius = Bridge_receiveNumber(bridge);
    __UG_PENCIL_CALL(fill_polygon, point, sides, radius) {
        Replot_fillPolygon(gPenTarget, sides, _POINT, radius);
    }
    __pencil_apply_styles();
    Bridge_returnEmpty(bridge);
}

void native_pencil_draw_star(Bridge *bridge)
{
    UGPoint point = point_from_bridge(bridge);
    double sides = Bridge_receiveNumber(bridge);
    double radius1 = Bridge_receiveNumber(bridge);
    double radius2 = Bridge_receiveNumber(bridge);
    __pencil_prepare_paint();
    Replot_drawStar(gPenTarget, sides, _POINT, radius1, radius2);
    __pencil_apply_styles();
    Bridge_returnEmpty(bridge);
}

void native_pencil_fill_star(Bridge *bridge)
{
    UGPoint point = point_from_bridge(bridge);
    double sides = Bridge_receiveNumber(bridge);
    double radius1 = Bridge_receiveNumber(bridge);
    double radius2 = Bridge_receiveNumber(bridge);
    __pencil_prepare_paint();
    Replot_fillStar(gPenTarget, sides, _POINT, radius1, radius2);
    __pencil_apply_styles();
    Bridge_returnEmpty(bridge);
}

void native_pencil_print_text(Bridge *bridge)
{
    UGPoint point = point_from_bridge(bridge);
    CString text = Bridge_receiveString(bridge);
    int size = Bridge_receiveNumber(bridge);
    // 
    __pencil_prepare_paint();
    Replot_printText(__eReplot, _POINT, size, text);
    __pencil_apply_styles();
    Bridge_returnEmpty(bridge);
}

void native_pencil_draw_paper(Bridge *bridge)
{
    UGPoint point = point_from_bridge(bridge);
    ECanvas canvas = paper_from_bridge(bridge);
    UGSize size = size_from_bridge(bridge);
    //
    __pencil_prepare_paint();
    Replot_drawCanvas(__eReplot, _POINT, _SIZE, canvas);
    __pencil_apply_styles();
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
    UGImage *image;
    if (Bridge_nextType(bridge) == UG_TYPE_NUM) {
        UGPoint point = point_from_bridge(bridge);
        UGSize size = size_from_bridge(bridge);
        image = UG_NEW_IMAGE(path, point.x, point.y, size.w, size.h);
    } else {
        image = UG_NEW_IMAGE(path, 0, 0, -1, -1);
    }
    delegate_load_image(image);
    Loadable *loadable = Loadable_newStuf(image, ALIAS_texture, path, _pencil_release_image);
    Bridge_returnValue(bridge, loadable);
}

void native_pencil_load_font(Bridge *bridge)
{
    CString path = Bridge_receiveString(bridge);
    UGFont *font = UG_NEW_FONT(path);
    delegate_load_font(font);
    Loadable *loadable = Loadable_newStuf(font, ALIAS_texture, path, _pencil_release_font);
    Bridge_returnValue(bridge, loadable);
}

// font

void native_pencil_draw_font(Bridge *bridge)
{
    UGPoint point = point_from_bridge(bridge);
    UGFont *font = font_from_bridge(bridge);
    float size = Bridge_receiveNumber(bridge);
    CString text = Bridge_receiveString(bridge);
    UGColor color = color_from_bridge(bridge);
    //
    __UG_PENCIL_CALL(draw_font, point, font, color, text, size) {
        // font not supported
    }
    __pencil_apply_styles();
    Bridge_returnEmpty(bridge);
}

// image

void native_pencil_draw_image(Bridge *bridge)
{
    UGPoint point = point_from_bridge(bridge);
    UGImage *image = image_from_bridge(bridge);
    double scale = Bridge_receiveNumberWithDefault(bridge, gPenScale);
    UGColor color = color_from_bridge(bridge);
    //
    float xAnchor = 0.5f;
    float yAnchor = 0.5f;
    __UG_PENCIL_CALL(draw_image, point, image, color, xAnchor, yAnchor, scale) {
        int w = image->w * scale;
        int h = image->h * scale;
        int x = point.x - (xAnchor - 0.5) * w;
        int y = point.y - (yAnchor - 0.5) * h;
        Replot_setTexture(gPenTarget, image->pxls, image->w, image->h);
        Replot_fillRect(gPenTarget, RPOINT(x, y), RSIZE(w, h));
        Replot_setImage(gPenTarget, NULL);
    }
    __pencil_apply_styles();
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

//////////////////////////////////////////////////////////
