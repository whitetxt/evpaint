#include "include/main.h"

int initSDL() {
	if (!SDL_WasInit(SDL_INIT_EVERYTHING) && SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		error("Failed to initialise SDL", ERROR_TYPE_SDL);
		return 1;
	}
	return 0;
}

int initTTF() {
	if (!TTF_WasInit() && TTF_Init() != 0) {
		error("Failed to initialise SDL_TTF", ERROR_TYPE_TTF);
		return 1;
	}
	return 0;
}

int initImage() {
	int flags = IMG_INIT_JPG | IMG_INIT_PNG | IMG_INIT_TIF | IMG_INIT_WEBP;
	int initted = IMG_Init(flags);
	if ((initted & flags) != flags) {
		error("Failed to initialise SDL_Image", ERROR_TYPE_IMAGE);
		return 1;
	}
	return 0;
}

int quitSDL() {
	SDL_Quit();
	return 0;
}

int quitImage() {
	IMG_Quit();
	return 0;
}

int quitTTF() {
	TTF_Quit();
	return 0;
}