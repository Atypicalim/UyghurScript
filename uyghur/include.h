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
#include <unistd.h>
#include <sys/stat.h>
#include <limits.h>
#include <setjmp.h>

#include "utf8.h"
#include "tinyfiledialogs.h"

#include "../../c-pure-tools/tools.h"

#define CARGS_IMPLEMENTATION
#include "../../c-extra-tools/cargs/cargs.h"

#include "../../c-extra-tools/utf8iter/utf8iter.h"
#include "../../c-extra-tools/utf8iter/utf8iter.c"
#include "../../c-extra-tools/utf8string/utf8string.h"
#include "../../c-extra-tools/utf8string/utf8string.c"


#ifdef BUILDER_USE_RAYSAN
    #define RAUDIO_NO_IMLEMENT
    #define RAYGUI_IMPLEMENTATION
    #include "raylib.h"
    #include "raygui.h"
#endif

#ifdef BUILDER_USE_RILEY
    #define RSGL_IMPLEMENTATION
    #include "RSGL.h"
#endif


#define RA_NO_WIN_EXTRA
#include "../../c-extra-tools/raudio/raudio.h"


#endif
