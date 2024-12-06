// paper

#include "./externals.h"
#include "../delegates/header.h"

/////////////////////////////////////////////////

void native_paper_create(Bridge *bridge)
{
    int w = Bridge_receiveNumber(bridge);
    int h = Bridge_receiveNumber(bridge);
    EPaper paper = EPaper_new(w, h);
    Loadable *loadable = Loadable_newStuf(paper, ALIAS_paper, "~", __release_paper);
    pencil_focus_to(UG_PENCIL_FOCUS_PAPER, paper);
    Bridge_returnValue(bridge, loadable);
}

void native_paper_copy(Bridge *bridge)
{
    EPaper paper = paper_from_bridge(bridge);
    UGPoint point = point_from_bridge(bridge);
    UGSize size = size_from_bridge(bridge);
    UGRect _rect = UG_RECT_GENERATE(&point, &size);
    EPaper _paper = pntr_image_from_image(paper, _rect.x, _rect.y, _rect.w, _rect.h);
    Loadable *loadable = Loadable_newStuf(_paper, ALIAS_paper, "~", __release_paper);
    Bridge_returnValue(bridge, loadable);
}

void native_paper_crop(Bridge *bridge)
{
    EPaper paper = paper_from_bridge(bridge);
    UGPoint point = point_from_bridge(bridge);
    UGSize size = size_from_bridge(bridge);
    UGRect _rect = UG_RECT_GENERATE(&point, &size);
    pntr_image_crop(paper, _rect.x, _rect.y, _rect.w, _rect.h);
    Bridge_returnEmpty(bridge);
}

void native_paper_read(Bridge *bridge)
{
    CString path = Bridge_receiveString(bridge);
    EPaper paper = pntr_load_image(path);
    Loadable *loadable = Loadable_newStuf(paper, ALIAS_paper, path, __release_paper);
    pencil_focus_to(UG_PENCIL_FOCUS_PAPER, paper);
    Bridge_returnValue(bridge, loadable);
}

void native_paper_write(Bridge *bridge)
{
    EPaper paper = paper_from_bridge(bridge);
    CString path = Bridge_receiveString(bridge);
    pntr_save_image(paper, path);
    Bridge_returnEmpty(bridge);
}
