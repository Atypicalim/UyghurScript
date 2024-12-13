// paper

#include "./externals.h"
#include "../delegates/header.h"

/////////////////////////////////////////////////

void native_paper_create(Bridge *bridge)
{
    int w = Bridge_receiveNumber(bridge);
    int h = Bridge_receiveNumber(bridge);
    ECanvas canvas = Replot_new(w, h);
    Loadable *loadable = Loadable_newStuf(canvas, ALIAS_paper, "~", __release_paper);
    pencil_focus_to(UG_PENCIL_FOCUS_PAPER, canvas);
    Bridge_returnValue(bridge, loadable);
}

void native_paper_copy(Bridge *bridge)
{
    ECanvas canvas = paper_from_bridge(bridge);
    UGPoint point = point_from_bridge(bridge);
    UGSize size = size_from_bridge(bridge);
    UGRect _rect = UG_RECT_GENERATE(&point, &size);
    ECanvas _canvas = Replot_copy(canvas, RPOINT(_rect.x, _rect.y), RSIZE(_rect.w, _rect.h));
    Loadable *loadable = Loadable_newStuf(_canvas, ALIAS_paper, "~", __release_paper);
    Bridge_returnValue(bridge, loadable);
}

void native_paper_crop(Bridge *bridge)
{
    ECanvas canvas = paper_from_bridge(bridge);
    UGPoint point = point_from_bridge(bridge);
    UGSize size = size_from_bridge(bridge);
    UGRect _rect = UG_RECT_GENERATE(&point, &size);
    Replot_crop(canvas, RPOINT(_rect.x, _rect.y), RSIZE(_rect.w, _rect.h));
    Bridge_returnEmpty(bridge);
}

void native_paper_flip(Bridge *bridge)
{
    ECanvas canvas = paper_from_bridge(bridge);
    bool xFlip = Bridge_receiveBoolean(bridge);
    bool yFlip = Bridge_receiveBoolean(bridge);
    Replot_flip(canvas, xFlip, yFlip);
    Bridge_returnEmpty(bridge);
}

void native_paper_invert(Bridge *bridge)
{
    ECanvas canvas = paper_from_bridge(bridge);
    Replot_invert(canvas);
    Bridge_returnEmpty(bridge);
}

void native_paper_tint(Bridge *bridge)
{
    ECanvas canvas = paper_from_bridge(bridge);
    UGColor color = color_from_bridge(bridge);
    float intensity = Bridge_receiveNumber(bridge);
    Replot_tint(canvas, RCOLOR(color.r, color.g, color.b, color.a), intensity);
    Bridge_returnEmpty(bridge);
}

void native_paper_fade(Bridge *bridge)
{
    ECanvas canvas = paper_from_bridge(bridge);
    float intensity = Bridge_receiveNumber(bridge);
    Replot_fade(canvas, intensity);
    Bridge_returnEmpty(bridge);
}

void native_paper_blur(Bridge *bridge)
{
    ECanvas canvas = paper_from_bridge(bridge);
    int radius = Bridge_receiveNumber(bridge);
    Replot_blur(canvas, radius);
    Bridge_returnEmpty(bridge);
}

void native_paper_read(Bridge *bridge)
{
    CString path = Bridge_receiveString(bridge);
    ECanvas canvas = Replot_read(path);
    Loadable *loadable = Loadable_newStuf(canvas, ALIAS_paper, path, __release_paper);
    pencil_focus_to(UG_PENCIL_FOCUS_PAPER, canvas);
    Bridge_returnValue(bridge, loadable);
}

void native_paper_write(Bridge *bridge)
{
    ECanvas canvas = paper_from_bridge(bridge);
    CString path = Bridge_receiveString(bridge);
    Replot_write(canvas, path);
    Bridge_returnEmpty(bridge);
}
