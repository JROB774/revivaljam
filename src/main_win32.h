#ifndef MAIN_WIN32_H
#define MAIN_WIN32_H

#define PLATFORM_WIN32

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <time.h>
#include <assert.h>
#include <math.h>
#include <string.h>

// Defining these as blank is necessary in order to get SDL2 working with TCC.
// We also had to modify some typedefs in SDL_config.h, we will look into
// making that part more portable across compiler's in the future. However,
// for now it is fine as it is as we are the only ones compiling this source.
#ifdef __TINYC__
#define SDLCALL
#define DECLSPEC
#endif

#define SDL_MAIN_HANDLED 1
#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>

// Headers
#include "assets.h"
#include "utility.h"
#include "logger.h"
#include "ftimer.h"
#include "render.h"
#include "window.h"
#include "input.h"
#include "audio.h"
#include "settings.h"
#include "score.h"
#include "player.h"
#include "effect.h"
#include "entity.h"
#include "application.h"

// Sources
#include "utility.c"
#include "logger.c"
#include "ftimer.c"
#include "render.c"
#include "window.c"
#include "input.c"
#include "audio.c"
#include "settings.c"
#include "score.c"
#include "player.c"
#include "effect.c"
#include "entity.c"
#include "application.c"

#endif /* MAIN_WIN32_H */
