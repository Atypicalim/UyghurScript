// callbacks

#ifndef UG_DELEGATE_HEADER
#define UG_DELEGATE_HEADER

#include "./define.h"

void stage_on_show();

void stage_on_hide();

void stage_on_focus();

void stage_on_resize();

void stage_on_drop();

#ifdef BUILDER_USE_RAYSAN
#include "../delegates/raylib_lib.h"
#endif

#ifdef BUILDER_USE_RILEY
#include "../delegates/riley_lib.h"
#endif

#endif
