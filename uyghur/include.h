// uyghur

#ifndef H_UG_INCLUDE
#define H_UG_INCLUDE

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <ctype.h>
#include <stdarg.h>
#include <time.h>
#include <sys/stat.h>
#include <limits.h>
#include <setjmp.h>

#include "utf8.h"
#include "tinyfiledialogs.h"

#include "../../c-pure-tools/tools.h"

#define CARGS_IMPLEMENTATION
#include "../../c-xtra-tools/cargs/cargs.h"

#include "../../c-xtra-tools/utf8iter/utf8iter.h"
#include "../../c-xtra-tools/utf8iter/utf8iter.c"
#include "../../c-xtra-tools/utf8string/utf8string.h"
#include "../../c-xtra-tools/utf8string/utf8string.c"


#ifdef BUILDER_USE_RAYSAN
    #define REPLAY_NO_IMLEMENT
    #define REPLOT_NO_STB
    #define RAYGUI_IMPLEMENTATION
    #include "raylib.h"
    #include "raygui.h"
#endif

#ifdef BUILDER_USE_RILEY
    #define RSGL_IMPLEMENTATION
    #include "RSGL.h"
#endif

#ifndef BUILDER_USE_TIGR
#define TIGR_EXT_NO_DESKTOP
#endif
#undef __IOS__
#undef __ANDROID__
#define TIGR_EXT_NO_MOBILE
#include "../../c-xtra-tools/tigr4ext/tigr4ext.h"

#define REPLAY_WIN_NO_EXTRA
#include "../../c-replay-library/replay.h"

#define REPLOT_WIN_NO_EXTRA
#include "../../c-replot-library/replot.h"

#endif
