#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include "../SDL2_gfxPrimitives.h"
#include "tinyfiledialogs.h"
#include "init.h"
#include "paint.h"
#include "events.h"
#include "ui.h"


extern SDL_Window *window;
extern SDL_Renderer *renderer;
extern bool active;

extern SDL_Rect *canvasPreviews;
extern size_t canvasPreviewNum;

int initAll();
void quitAll();
void quit();

#define error(error, type) error_caller(error, type, __FILE__, __LINE__)
#define warn(error) warn_caller(error, __FILE__, __LINE__)
#define ERROR_TYPE_NORMAL 0
#define ERROR_TYPE_SDL 1
#define ERROR_TYPE_IMAGE 2
#define ERROR_TYPE_TTF 3
void error_caller(char *err, int type, const char *file, const int line);
void warn_caller(char *err, const char *file, const int line);