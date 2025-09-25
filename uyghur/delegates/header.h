// callbacks

#ifndef UG_DELEGATE_HEADER
#define UG_DELEGATE_HEADER

#include "./delegates.h"

//////////////////////////////////////////////////////////

#ifdef USTAGE_USE_RAYSAN
#include "../delegates/raysan_lib.h"
#endif

#ifdef USTAGE_USE_RILEY
#include "../delegates/riley_lib.h"
#endif

#ifdef USTAGE_USE_TIGR
#include "../delegates/tigr_lib.h"
#endif

//////////////////////////////////////////////////////////

#if USTAGE_USE_SOFT

void *delegate_unload_image(UGImage *img) {
    return NULL;
}

void *delegate_unload_font(UGFont *fnt) {
    return NULL;
}

UGImage *delegate_load_image(UGImage *img)
{
    return img;
}

UGFont *delegate_load_font(UGFont *fnt)
{
    return fnt;
}

#endif

//////////////////////////////////////////////////////////

#ifdef UG_NO_STAGE

void deletage_paint_plot(int x, int y, int r, int g, int b, int a) {
}

#endif

//////////////////////////////////////////////////////////

#endif
