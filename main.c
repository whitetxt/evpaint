#include "include/main.h"

SDL_Window *window;
SDL_Renderer *renderer;
bool active = true;
SDL_Rect *canvasPreviews = NULL;
size_t canvasPreviewNum = 0;

void error_caller(char *err, int type, const char *file, const int line)
{
    switch (type)
    {
    case ERROR_TYPE_SDL:
        printf("Error [%s:%d]: %s%s\n", file, line, err, SDL_GetError());
        break;
    case ERROR_TYPE_IMAGE:
        printf("Error [%s:%d]: %s%s\n", file, line, err, IMG_GetError());
        break;
    case ERROR_TYPE_TTF:
        printf("Error [%s:%d]: %s%s\n", file, line, err, TTF_GetError());
        break;
    case ERROR_TYPE_NORMAL:
    default:
        printf("Error [%s:%d]: %s", file, line, err);
        break;
    }
}

void warn_caller(char *err, const char *file, const int line)
{
    printf("Warning [%s:%d]: %s", file, line, err);
}

double render()
{
    // Returns rendertime
    clock_t t;
    t = clock();
    canvasPreviews = realloc(canvasPreviews, canvasPreviewNum * sizeof(*canvasPreviews));
    SDL_SetRenderDrawColor(renderer, 90, 90, 90, 255);
    SDL_RenderClear(renderer);
    SDL_Rect tmp = {0, 0, width, 70};
    SDL_SetRenderDrawColor(renderer, 150, 150, 150, 255);
    SDL_RenderFillRect(renderer, &tmp);
    SDL_Rect tmp2 = {0, 70, width, height > 750 ? height / 10 : 75};
    UIOffsetY = height > 750 ? height / 10 : 75;
    SDL_SetRenderDrawColor(renderer, 125, 125, 125, 255);
    SDL_RenderFillRect(renderer, &tmp2);
    int off = 0;
    for (size_t i = 0; i < numCanvases; i++)
    {
        SDL_UpdateTexture(canvases[i].tex, NULL, canvases[i].pixels, canvases[i].width * sizeof(uint32_t));
        canvasPreviews[i].x = 10 + off;
        canvasPreviews[i].y = 10;
        canvasPreviews[i].w = 50;
        canvasPreviews[i].h = 50;
        off += 60;
        SDL_RenderCopy(renderer, canvases[i].tex, NULL, &canvasPreviews[i]);
    }
    renderCanvas(&canvases[currentCanvas]);
    int offset = 0;
    UIOffsetY += 70;
    UIOffsetY *= 1.4;
    for (size_t i = 0; i < NumButtons; i++)
    {
        renderButton(&Buttons[i], &offset);
    }
    for (size_t i = 0; i < NumSliders; i++)
    {
        renderSlider(&Sliders[i], &offset);
    }
    for (size_t i = 0; i < NumSwitches; i++)
    {
        renderSwitch(Switches[i], i);
    }
    SDL_SetRenderDrawColor(renderer, (int)Sliders[RIndex].value, (int)Sliders[GIndex].value, (int)Sliders[BIndex].value, 255);
    SDL_RenderFillRect(renderer, &ColourPreview);
    SDL_SetRenderDrawColor(renderer, 90, 90, 90, 255);
    SDL_RenderPresent(renderer);
    t = clock() - t;
    double time = ((double)t) / CLOCKS_PER_SEC;
    return time;
}

int initAll()
{
    if (initSDL() != 0)
    {
        return 1;
    }
    if (initTTF() != 0)
    {
        return 1;
    }
    if (initImage() != 0)
    {
        return 1;
    }
    return 0;
}

void quitAll()
{
    quitTTF();
    quitImage();
    quitSDL();
}

void quit()
{
    active = false;
}

int main(int argc, char **argv)
{
    (void)argc;
    (void)argv;
    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "best");

    printf("Started.\n");
    if (initAll() != 0)
    {
        error("Couldn't initialise.\n", ERROR_TYPE_NORMAL);
        return 1;
    }

    Uint32 winFlags = SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI;
    width = 1600;
    height = 900;
    UIOffsetY = height > 750 ? height / 10 : 75;
    window = SDL_CreateWindow("Paint but good", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, winFlags);

    if (!window)
    {
        error("Failed to create window.\n", ERROR_TYPE_SDL);
        quitAll();
        return 1;
    }

    Uint32 rendFlags = SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_ACCELERATED;
    renderer = SDL_CreateRenderer(window, -1, rendFlags);
    initFont();
    createUI();
    createCanvas(0, 0);
    while (active)
    {
        handleEvents();
        render();
    }
    SDL_DestroyRenderer(renderer);
    quitAll();
    return 0;
}