#include "include/main.h"

Canvas *canvases = NULL;
size_t numCanvases = 0;
size_t currentCanvas = 0;

void renderCanvas(Canvas *canvas) {
	SDL_UpdateTexture(canvas->tex, NULL, canvas->pixels, canvas->width * sizeof(uint32_t));
	canvas->rect.x = (width - UIOffsetX) / 2 - canvas->rect.w / 2;
	canvas->rect.y = (height - UIOffsetY) / 2 - canvas->rect.h / 2;
	SDL_RenderCopy(renderer, canvas->tex, NULL, &canvas->rect);
}

int saveCanvas(Canvas canvas, char *filename) {
	/*
	Saves the canvas parameter to the file
	specified.
	Returns 0 if success, 1 if failure.
	*/
	SDL_Texture *target = SDL_GetRenderTarget(renderer);
	if (SDL_SetRenderTarget(renderer, canvas.tex) != 0) {
		return 1;
	}
	SDL_Surface *surface = SDL_CreateRGBSurface(0, canvas.width, canvas.height, 32, 0, 0, 0, 0);
	if (!surface) {
		return 1;
	}
	if (SDL_RenderReadPixels(renderer, NULL, surface->format->format, surface->pixels, surface->pitch) != 0) {
		return 1;
	}
	char *ext = strrchr(filename, '.');
	if (!ext) {
		return 1;
	}
	if (strcmp(ext, ".jpg") == 0) {
		IMG_SaveJPG(surface, filename, 100);
	} else if (strcmp(ext, ".bmp") == 0) {
		SDL_SaveBMP(surface, filename);
	} else if (strcmp(ext, ".png") == 0)  {
		IMG_SavePNG(surface, filename);
	}
	SDL_FreeSurface(surface);
	if (SDL_SetRenderTarget(renderer, target) != 0) {
		return 1;
	}
	return 0;
}

void saveCurrentCanvas() {
	printf("Saving picture.\n");
	char const * lFilterPatterns[3] = {"*.jpg", "*.png", "*.bmp"};
	char *fn = tinyfd_saveFileDialog("Save location", "out.png", 3, lFilterPatterns, NULL);
	saveCanvas(canvases[currentCanvas], fn);
	printf("Canvas saved.\n");
}

int createCanvas(int Cwidth, int Cheight) {
	/*
	Creates a brand new white canvas
	and adds it to the canvases array
	and sets the currentCanvas to it
	Returns the index.
	*/
	if (Cwidth < 1 || Cheight < 1) {
		Cwidth = 300;
		Cheight = 300;
	}
	canvases = realloc(canvases, ++numCanvases * sizeof(*canvases));
	int canvasNum = numCanvases - 1;
	currentCanvas = numCanvases - 1;
	canvases[currentCanvas].pixels = malloc(Cwidth * Cheight * sizeof(Uint32));
	memset(canvases[currentCanvas].pixels, 255, Cwidth * Cheight * sizeof(Uint32));
	canvases[currentCanvas].tex = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_TARGET, Cwidth, Cheight);
	canvases[currentCanvas].width = Cwidth;
	canvases[currentCanvas].height = Cheight;
	canvases[currentCanvas].rect.w = Cwidth;
	canvases[currentCanvas].rect.h = Cheight;
	canvases[currentCanvas].rect.x = UIOffsetX;
	canvases[currentCanvas].rect.y = UIOffsetY;
	canvasPreviewNum += 1;
	return canvasNum;
}

void createCanvasPopup() {
	char *width = tinyfd_inputBox("New canvas width", "Please enter the width for the new canvas", "");
	if (!width || atoi(width) < 0) {
		tinyfd_messageBox("Invalid width", "Invalid width entered.", "ok", "error", 1);
		return;
	}
	char *height = tinyfd_inputBox("New canvas width", "Please enter the width for the new canvas", "");
	if (!height || atoi(height) < 0) {
		tinyfd_messageBox("Invalid height", "Invalid height entered.", "ok", "error", 1);
		return;
	}
	int w;
	int h;
	if (sscanf(width, "%d", &w) != 1) {
		tinyfd_messageBox("Invalid width", "Invalid width entered.", "ok", "error", 1);
		return;
	}
	if (sscanf(height, "%d", &h) != 1) {
		tinyfd_messageBox("Invalid height", "Invalid height entered.", "ok", "error", 1);
		return;
	}
	createCanvas(w, h);
}

void setPixel(int x, int y) {
	/*
	Sets the pixel at the target X and Y
	Sets it on the current canvas.
	*/
	x -= canvases[currentCanvas].rect.x;
	y -= canvases[currentCanvas].rect.y;
	if (x > canvases[currentCanvas].width ||
		x < 0 ||
		y > canvases[currentCanvas].height ||
		y < 0) {
		return;
	}
	Uint32 pixelValue = getColour();
	canvases[currentCanvas].pixels[y * canvases[currentCanvas].height + x] = pixelValue;
}

void setLine(int x1, int y1, int x2, int y2) {
	/*
	Old algorithm method, bad.
	if (x1 == x2 || (x1 - x2) == 0) {
		int currentY = y1;
		if (y1 > y2) {
			while (currentY > y2) {
				currentY--;
				setPixel(x1, currentY);
			}
		} else {
			while (currentY < y2) {
				currentY++;
				setPixel(x1, currentY);
			}
		}
		return;
	}
	double grad = (y1 - y2)/(x1 - x2);
	double yint = y1 - grad * x1;
	int currentX = x1;
	if (x1 > x2) {
		while (currentX > x2) {
			currentX--;
			setPixel(currentX, roundI(grad * currentX + yint));
		}
	} else {
		while (currentX < x2) {
			currentX++;
			setPixel(currentX, roundI(grad * currentX + yint));
		}
	}*/

	// Blitting method, good.
	SDL_Texture *target = SDL_GetRenderTarget(renderer);
	if (SDL_SetRenderTarget(renderer, canvases[currentCanvas].tex) != 0) {
		return;
	}
	x1 -= canvases[currentCanvas].rect.x;
	y1 -= canvases[currentCanvas].rect.y;
	x2 -= canvases[currentCanvas].rect.x;
	y2 -= canvases[currentCanvas].rect.y;
	SDL_SetRenderDrawColor(renderer, (int)Sliders[RIndex].value, (int)Sliders[GIndex].value, (int)Sliders[BIndex].value, 255);
	SDL_Surface *surface = SDL_CreateRGBSurface(0, canvases[currentCanvas].width, canvases[currentCanvas].height, 32, 0, 0, 0, 0);
	if (!surface) {
		return;
	}
	thickLineRGBA(renderer, x1, y1, x2, y2, (int)Sliders[ThickIndex].value, (int)Sliders[RIndex].value, (int)Sliders[GIndex].value, (int)Sliders[BIndex].value, 255);
	if (SDL_RenderReadPixels(renderer, NULL, 0, canvases[currentCanvas].pixels, surface->pitch) != 0) {
		return;
	}
	SDL_UpdateTexture(canvases[currentCanvas].tex, NULL, canvases[currentCanvas].pixels, canvases[currentCanvas].width * sizeof(uint32_t));
	if (SDL_SetRenderTarget(renderer, target) != 0) {
		return;
	}
}

int roundI(double number) {
	return (number >= 0) ? (int)(number + 0.5) : (int)(number - 0.5);
}

Uint32 getColour() {
	Uint32 pixelValue = 255 << 24;
	int R = Sliders[RIndex].value;
	int G = Sliders[GIndex].value;
	int B = Sliders[BIndex].value;
	pixelValue = pixelValue | R << 16;
	pixelValue = pixelValue | G << 8;
	pixelValue = pixelValue | B;
	return pixelValue;
}