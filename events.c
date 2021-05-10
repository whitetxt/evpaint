#include "include/main.h"

bool mouseDown = false;
int mouseX = 0;
int mouseY = 0;
int width;
int height;
SDL_Rect mouse = {0, 0, 4, 4};
SDL_Rect out;
SDL_bool result;

int prevMouseX = 0;
int prevMouseY = 0;

void handleEvents() {
	SDL_Event ev;
	while (SDL_PollEvent(&ev)) {
		switch (ev.type) {
			case SDL_QUIT: // If the window is closed.
				active = false;
				break;
			case SDL_MOUSEMOTION: // When the mouse is moved
				prevMouseX = mouseX;
				prevMouseY = mouseY;
				mouseX = ev.motion.x;
				mouseY = ev.motion.y;
				mouseCanvasCollision();
				if (mouseDown) {
					if (result == SDL_TRUE)
						setLine(prevMouseX, prevMouseY, mouseX, mouseY);
					for (size_t i = 0; i < NumSliders; i++) {
						mouse.h = 1;
						mouse.w = 1;
						result = SDL_IntersectRect(&Sliders[i].rect, &mouse, &out);
						if (result == SDL_TRUE) {
							setSliderValue(i, mouse);
						}
					}
				}
				break;
			case SDL_MOUSEBUTTONDOWN: // When the LMB is pressed
				mouseDown = true;
				prevMouseX = mouseX;
				prevMouseY = mouseY;
				mouseX = ev.motion.x;
				mouseY = ev.motion.y;
				mouseCanvasCollision();
				if (result == SDL_TRUE)
					setPixel(mouseX, mouseY);
				for (size_t i = 0; i < NumButtons; i++) {
					result = SDL_IntersectRect(&Buttons[i].rect, &mouse, &out);
					if (result == SDL_TRUE && Buttons[i].func != NULL) {
						Buttons[i].func();
					}
				}
				for (size_t i = 0; i < NumSliders; i++) {
					mouse.h = 1;
					mouse.w = 1;
					result = SDL_IntersectRect(&Sliders[i].rect, &mouse, &out);
					if (result == SDL_TRUE) {
						setSliderValue(i, mouse);
					}
				}
				for (size_t i = 0; i < canvasPreviewNum; i++) {
					mouse.h = 1;
					mouse.w = 1;
					result = SDL_IntersectRect(&canvasPreviews[i], &mouse, &out);
					if (result == SDL_TRUE) {
						currentCanvas = i;
					}
				}
				break;
			case SDL_MOUSEBUTTONUP: // When the LMB is released
				mouseDown = false;
				prevMouseX = mouseX;
				prevMouseY = mouseY;
				mouseX = ev.motion.x;
				mouseY = ev.motion.y;
				mouseCanvasCollision();
				if (result == SDL_TRUE)
					setPixel(mouseX, mouseY);
				break;
			case SDL_KEYDOWN: // When a key is pressed
				switch (ev.key.keysym.sym) {
					case SDLK_s:
						printf("Saving image to 'out.png'\n");
						if (saveCanvas(canvases[currentCanvas], "out.png") != 0) {
							error("Couldn't save image.\n", ERROR_TYPE_SDL);
						}
						break;
				}
				break;
			case SDL_WINDOWEVENT: // When the window changed
				switch (ev.window.event) {
					case SDL_WINDOWEVENT_RESIZED:
						width = ev.window.data1;
						height = ev.window.data2;
						break;
				}
				break;
			default:
				break;
		}
	}
}

SDL_bool mouseCanvasCollision() {
	mouse.x = mouseX;
	mouse.y = mouseY;
	result = SDL_IntersectRect(&canvases[currentCanvas].rect, &mouse, &out);
	return result;
}

void printRect(SDL_Rect rect) {
	printf("RectX: %d\nRectY: %d\nRectW: %d\nRectH: %d\n", rect.x, rect.y, rect.w, rect.h);
}

void setSliderValue(int i, SDL_Rect mouse) {
	switch (Sliders[i].rotation) {
		case UI_SLIDER_VERTICAL:
			mouse.y -= Sliders[i].rect.y;
			Sliders[i].value = Sliders[i].max - ((Sliders[i].max - Sliders[i].min) * ((double)mouse.y / Sliders[i].rect.h));
			break;
		case UI_SLIDER_HORIZONTAL:
			mouse.x -= Sliders[i].rect.x;
			Sliders[i].value = Sliders[i].min + ((Sliders[i].max - Sliders[i].min) * ((double)mouse.x / Sliders[i].rect.w));
			break;
	}
}