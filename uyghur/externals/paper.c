// paper

#include "./externals.h"
#include "../delegates/header.h"

/////////////////////////////////////////////////

#define RPaper Replot*

RPaper paper_from_bridge(Bridge *bridge)
{
    Loadable *loadable = Bridge_receiveValue(bridge, UG_TYPE_STF);
    RPaper paper = loadable->obj;
    return paper;
}

void __release_paper(Loadable *loadable) {
    if (loadable->obj) {
        RPaper paper = loadable->obj;
        if (paper == __ugPencilTarget) {
            pencil_focus_to(UG_PENCIL_FOCUS_NONE);
        }
        Replot_free(paper);
        loadable->obj = NULL;
    }
}

/////////////////////////////////////////////////

void native_paper_create(Bridge *bridge)
{
    int w = Bridge_receiveNumber(bridge);
    int h = Bridge_receiveNumber(bridge);
    RPaper paper = Replot_new(w, h);
    Loadable *loadable = Loadable_newStuf(paper, ALIAS_paper, "~", __release_paper);
    pencil_focus_to(UG_PENCIL_FOCUS_PAPER);
    __ugPencilTarget = paper;
    Bridge_returnValue(bridge, loadable);
}

void native_paper_copy(Bridge *bridge)
{
    RPaper paper = paper_from_bridge(bridge);
    UGPoint point = point_from_bridge(bridge);
    UGSize size = size_from_bridge(bridge);
    RPaper _rplt = Replot_copy(paper, RPOINT(point.x, point.y), RSIZE(size.w, size.h));
    Loadable *loadable = Loadable_newStuf(_rplt, ALIAS_paper, "~", __release_paper);
    Bridge_returnValue(bridge, loadable);
}

void native_paper_crop(Bridge *bridge)
{
    RPaper paper = paper_from_bridge(bridge);
    UGPoint point = point_from_bridge(bridge);
    UGSize size = size_from_bridge(bridge);
    Replot_crop(paper, RPOINT(point.x, point.y), RSIZE(size.w, size.h));
    Bridge_returnEmpty(bridge);
}

void native_paper_read(Bridge *bridge)
{
    CString path = Bridge_receiveString(bridge);
    RPaper paper = Replot_read(path);
    Loadable *loadable = Loadable_newStuf(paper, ALIAS_paper, path, __release_paper);
    pencil_focus_to(UG_PENCIL_FOCUS_PAPER);
    __ugPencilTarget = paper;
    Bridge_returnValue(bridge, loadable);
}

void native_paper_write(Bridge *bridge)
{
    RPaper paper = paper_from_bridge(bridge);
    CString path = Bridge_receiveString(bridge);
    Replot_write(paper, path);
    Bridge_returnEmpty(bridge);
}
