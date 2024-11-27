// baord

#include "./externals.h"
#include "../delegates/header.h"

/////////////////////////////////////////////////

UGColor _ugColor = (UGColor){200, 255, 200, 255};
int _ugRotation = 0;
int _ugThickness = 1;

/////////////////////////////////////////////////

#define _COLOR (RColor){_ugColor.r, _ugColor.g, _ugColor.b, _ugColor.a}
#define _VECTOR (RPoint){vector.x, vector.y}
#define _CENTER (RPoint){center.x, center.y}
#define _SIZE (RSize){size.w, size.h}
#define _POINT (RPoint){point.x, point.y}
#define _POINT1 (RPoint){point1.x, point1.y}
#define _POINT2 (RPoint){point2.x, point2.y}
#define _POINT3 (RPoint){point3.x, point3.y}
#define _RECT (RRect){rect.x, rect.y, rect.w, rect.h}
#define _RECT2P (RPoint){rect.x, rect.y}
#define _RECT2S (RSize){rect.w, rect.h}

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

#define __UG_PENCIL_CALL(name, ...) if(__ugPencilFocus == UG_PENCIL_FOCUS_STAGE) { \
    delegate_pencil_##name(__VA_ARGS__); \
    } else if (__ugPencilFocus != UG_PENCIL_FOCUS_PAPER) { \
    tools_error("invalid state for pencil"); \
    } else \

void __pencil_draw_line(UGPoint point1, UGPoint point2) {
    __UG_PENCIL_CALL(draw_line, point1, point2, _ugThickness, _ugColor) {
        Replot_drawLine(_PAPER, _POINT1, _POINT2, 1);
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

void native_pencil_facelize(Bridge *bridge) {
    Loadable *paper = Bridge_receiveValue(bridge, UG_TYPE_NON);
    if (Loadable_isStuf(paper) && paper->linka == __release_paper) {
        pencil_focus_to(UG_PENCIL_FOCUS_PAPER);
        __ugPencilTarget = paper->obj;
    } else {
        pencil_focus_to(UG_PENCIL_FOCUS_NONE);
    }
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
    __UG_PENCIL_CALL(customize, _ugColor, _ugRotation) {
        Replot_setColor(_PAPER, _COLOR);
        Replot_setRotation(_PAPER, _ROTATION);
    }
    Bridge_returnEmpty(bridge);
}

void native_pencil_draw_pixel(Bridge *bridge)
{
    UGPoint point = point_from_bridge(bridge);
    __UG_PENCIL_CALL(draw_pixel, point, _ugColor) {
        Replot_drawPoint(_PAPER, _POINT, 1);
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
        Replot_drawTriangle(_PAPER, _POINT1, _POINT2, _POINT3);
    }
    Bridge_returnEmpty(bridge);
}

void native_pencil_fill_triangle(Bridge *bridge)
{
    UGPoint point1 = point_from_bridge(bridge);
    UGPoint point2 = point_from_bridge(bridge);
    UGPoint point3 = point_from_bridge(bridge);
    __UG_PENCIL_CALL(fill_triangle, point1, point2, point3, _ugColor) {
        Replot_fillTriangle(_PAPER, _POINT1, _POINT2, _POINT3);
    }
    Bridge_returnEmpty(bridge);
}

void native_pencil_draw_rectangle(Bridge *bridge)
{
    UGPoint point = point_from_bridge(bridge);
    UGSize size = size_from_bridge(bridge);
    __UG_PENCIL_CALL(draw_rectangle, point, size, _ugColor, _ugThickness) {
        Replot_drawRect(_PAPER, _POINT, _SIZE);
    }
    Bridge_returnEmpty(bridge);
}

void native_pencil_fill_rectangle(Bridge *bridge)
{
    UGPoint point = point_from_bridge(bridge);
    UGSize size = size_from_bridge(bridge);
    __UG_PENCIL_CALL(fill_rectangle, point, size, _ugColor) {
        Replot_fillRect(_PAPER, _POINT, _SIZE);
    }
    Bridge_returnEmpty(bridge);
}

void native_pencil_draw_circle(Bridge *bridge)
{
    UGPoint point = point_from_bridge(bridge);
    double radius = Bridge_receiveNumber(bridge);
    __UG_PENCIL_CALL(draw_circle, point, radius, _ugColor, _ugThickness) {
        Replot_drawCircle(_PAPER, _POINT, radius);
    }
    Bridge_returnEmpty(bridge);
}

void native_pencil_fill_circle(Bridge *bridge)
{
    UGPoint point = point_from_bridge(bridge);
    double radius = Bridge_receiveNumber(bridge);
    __UG_PENCIL_CALL(fill_circle, point, radius, _ugColor) {
        Replot_fillCircle(_PAPER, _POINT, radius);
    }
    Bridge_returnEmpty(bridge);
}

void native_pencil_draw_polygon(Bridge *bridge)
{
    UGPoint point = point_from_bridge(bridge);
    int sides = Bridge_receiveNumber(bridge);
    double radius = Bridge_receiveNumber(bridge);
    __UG_PENCIL_CALL(draw_polygon, point, sides, radius, _ugRotation, _ugColor, _ugThickness) {
        Replot_fillPolygon(_PAPER, _POINT, radius, sides);
    }
    Bridge_returnEmpty(bridge);
}

void native_pencil_fill_polygon(Bridge *bridge)
{
    UGPoint point = point_from_bridge(bridge);
    int sides = Bridge_receiveNumber(bridge);
    double radius = Bridge_receiveNumber(bridge);
    __UG_PENCIL_CALL(fill_polygon, point, sides, radius, _ugRotation, _ugColor) {
        Replot_fillPolygon(_PAPER, _POINT, radius, sides);
    }
    Bridge_returnEmpty(bridge);
}

#define _UG_PRINTER_W 500
#define _UG_PRINTER_H 100
Replot *_ugPrinter = NULL;
UGPoint _ugPrintCenter;
UGPoint _ugPrintP;
UGColor _ugPrintC;
void __ug_pencel_print(int x, int y, int r, int g, int b, int a) {
    _ugPrintP.x = _ugPrintCenter.x + x - _UG_PRINTER_W / 2;
    _ugPrintP.y = _ugPrintCenter.y + y - _UG_PRINTER_H / 2;
    _ugPrintC.r = r;
    _ugPrintC.g = g;
    _ugPrintC.b = b;
    _ugPrintC.a = a;
    __UG_PENCIL_CALL(draw_pixel, _ugPrintP, _ugPrintC) {}
}

void native_pencil_print_text(Bridge *bridge)
{
    UGPoint point = point_from_bridge(bridge);
    CString text = Bridge_receiveString(bridge);
    double size = Bridge_receiveNumber(bridge);
    // 
    if (__ugPencilFocus == UG_PENCIL_FOCUS_PAPER) {
        Replot_printText(_PAPER, _POINT, size, text);
    } else {
        if (!_ugPrinter) {
            _ugPrinter = Replot_new(_UG_PRINTER_W, _UG_PRINTER_H);
            _ugPrinter->plot = __ug_pencel_print;
        } else {
            _ugPrintCenter.x = point.x;
            _ugPrintCenter.y = point.y;
            Replot_printText(_ugPrinter, RPOINT(_UG_PRINTER_W / 2, _UG_PRINTER_H / 2), size, text);
        }
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

// font

void native_pencil_draw_font(Bridge *bridge)
{
    UGFont *font = font_from_bridge(bridge);
    float size = Bridge_receiveNumber(bridge);
    CString text = Bridge_receiveString(bridge);
    UGPoint point = point_from_bridge(bridge);
    __UG_PENCIL_CALL(draw_font, font, text, size, _ugColor, point) {
        tools_error("font not supported for paper");
    }
    Bridge_returnEmpty(bridge);
}

void native_pencil_measure_font(Bridge *bridge)
{
    UGFont *font = font_from_bridge(bridge);
    float size = Bridge_receiveNumber(bridge);
    CString text = Bridge_receiveString(bridge);
    int width = 0;
    __UG_PENCIL_CALL(measure_font, font, text, size, &width) {
        tools_error("font not supported for paper");
    }
    Bridge_returnNumber(bridge, width);
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
        Replot_setColor(_PAPER, _COLOR);
        Replot_setImage(_PAPER, image->path);
        Replot_drawRect(_PAPER, _POINT, RSIZE(w, h));
    }
    Bridge_returnEmpty(bridge);
}

/////////////////////////////////////////////////

#undef _COLOR
#undef _VECTOR
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
