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

#define MD5_IMPLEMENTATION
#include "../../c-pure-tools/test.h"

#define CARGS_IMPLEMENTATION
#include "../../c-xtra-tools/cargs/cargs.h"

#include "../../c-xtra-tools/utf8iter/utf8iter.h"
#include "../../c-xtra-tools/utf8iter/utf8iter.c"
#include "../../c-xtra-tools/utf8string/utf8string.h"
#include "../../c-xtra-tools/utf8string/utf8string.c"

#ifndef USTAGE_NONE_RAYSAN
    #define REPLAY_NO_IMLEMENT
    #define RAYGUI_IMPLEMENTATION
    #include "../../my-build-tools/build/c_libs/raylib/raylib-4.0.0_win64_mingw-w64/include/raylib.h"
    #include "../../my-build-tools/build/c_libs/raylib/raylib-4.0.0_win64_mingw-w64/include/raygui.h"
#endif

#ifndef USTAGE_NONE_RILEY
    #define RILEY_EXT_NO_WIDGETS
    #include "../../c-xtra-tools/riley4ext/riley4ext.h"
#endif

#ifdef USTAGE_USE_TIGR
#define USTAGE_USE_SOFT true
#else
#define USTAGE_USE_SOFT false
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
