#include "include/main.h"

Button *Buttons = NULL;
size_t NumButtons = 0;
Slider *Sliders = NULL;
size_t NumSliders = 0;
Switch *Switches = NULL;
size_t NumSwitches = 0;

int RIndex;
int GIndex;
int BIndex;
int ThickIndex;

SDL_Rect ColourPreview;

TTF_Font *Font = NULL;
TTF_Font *SliderLabel = NULL;

int UIOffsetX = 0;
int UIOffsetY = 0;

void initFont() {
	Font = TTF_OpenFont("fonts/aller.ttf", 24);
	if (!Font) {
		error("Failed to open font.\n", ERROR_TYPE_TTF);
	}
	SliderLabel = TTF_OpenFont("fonts/aller.ttf", 12);
	if (!SliderLabel) {
		error("Failed to open font for slider labels.\n", ERROR_TYPE_TTF);
	}
}

int createUIElement(char *name, int type) {
	/*
		Creates a new UI element and adds it to the relevant UI array.
		Returns the index in the relevant UI array.
	*/
	switch (type) {
		case UI_TYPE_BUTTON:
			Buttons = realloc(Buttons, ++NumButtons * sizeof(*Buttons));
			Buttons[NumButtons - 1].name = strdup(name);
			Buttons[NumButtons - 1].func = NULL;
			return NumButtons - 1;
		case UI_TYPE_SLIDER:
			Sliders = realloc(Sliders, ++NumSliders * sizeof(*Sliders));
			Sliders[NumSliders - 1].name = strdup(name);
			Sliders[NumSliders - 1].func = NULL;
			Sliders[NumSliders - 1].min = 0;
			Sliders[NumSliders - 1].max = 100;
			Sliders[NumSliders - 1].value = Sliders[NumSliders - 1].min;
			return NumSliders - 1;
			// Implement sliders
		case UI_TYPE_SWITCH:
			warn("Switch UI elements have not been implemented.\nThis element will not be created.\n");
			break;
			// Implement switches
		default:
			warn("Unknown UI element type passed in\nPlease use the defined options.\n");
			return -1;
	}
	return -1;
}

void addButtonText(Button *button, char *text) {
	SDL_Color Colour = {255, 255, 255, 255};
	SDL_Surface *TextSurface = TTF_RenderText_Blended(Font, text, Colour);
	if (!TextSurface) {
		warn("Failed to set button text.\nThe button will be blank.\n");
		return;
	}
	SDL_Rect tmpRect;
	// Create the texture if the text was make correctly.
	SDL_Texture *tmpTex = SDL_CreateTextureFromSurface(renderer, TextSurface);
	SDL_FreeSurface(TextSurface);
	// Prepare the SDL_Rect
	tmpRect.x = 0;
	tmpRect.y = 0;
	SDL_QueryTexture(tmpTex, NULL, NULL, &tmpRect.w, &tmpRect.h);
	button->rect = tmpRect;
	button->tex = tmpTex;
}

void setSliderMinMax(Slider *slider, float min, float max) {
	if (min > max) {
		warn("Slider minimum was attempted to be set higher than the max.\n");
		return;
	}
	slider->min = min;
	slider->max = max;
	slider->value = min;
}

void setSliderRotation(Slider *slider, int rotation) {
	if (rotation != UI_SLIDER_HORIZONTAL && rotation != UI_SLIDER_VERTICAL) {
		warn("Incorrect rotation attempted to be set on slider.\n");
		return;
	}
	slider->rotation = rotation;
}

void setSliderLength(Slider *slider, int length) {
	if (length < 1) {
		warn("OOB length attempted to be set for slider.\n");
		return;
	}
	switch (slider->rotation) {
		case UI_SLIDER_VERTICAL:
			slider->rect.h = length;
			slider->rect.w = 25;
			break;
		case UI_SLIDER_HORIZONTAL:
			slider->rect.w = length;
			slider->rect.h = 25;
			break;
		default:
			warn("Slider has an incorrect rotation. Length will not be set.\n");
			break;
	}
}

void addSliderText(Slider *slider) {
	slider->text = true;
}

void createUI() {
	if (!Font) {
		warn("Button font has not been initialised.\nThe UI cannot be created.\n");
		return;
	}
	int testI = createUIElement("new", UI_TYPE_BUTTON);
	addButtonText(&Buttons[testI], "New");
	Buttons[testI].func = &createCanvasPopup;
	testI = createUIElement("save", UI_TYPE_BUTTON);
	addButtonText(&Buttons[testI], "Save");
	Buttons[testI].func = &saveCurrentCanvas;
	testI = createUIElement("quit", UI_TYPE_BUTTON);
	addButtonText(&Buttons[testI], "Quit");
	Buttons[testI].func = &quit;
	testI = createUIElement("R", UI_TYPE_SLIDER);
	setSliderMinMax(&Sliders[testI], 0, 255);
	setSliderRotation(&Sliders[testI], UI_SLIDER_VERTICAL);
	setSliderLength(&Sliders[testI], 50);
	addSliderText(&Sliders[testI]);
	RIndex = testI;
	testI = createUIElement("G", UI_TYPE_SLIDER);
	setSliderMinMax(&Sliders[testI], 0, 255);
	setSliderRotation(&Sliders[testI], UI_SLIDER_VERTICAL);
	setSliderLength(&Sliders[testI], 50);
	addSliderText(&Sliders[testI]);
	GIndex = testI;
	testI = createUIElement("B", UI_TYPE_SLIDER);
	setSliderMinMax(&Sliders[testI], 0, 255);
	setSliderRotation(&Sliders[testI], UI_SLIDER_VERTICAL);
	setSliderLength(&Sliders[testI], 50);
	addSliderText(&Sliders[testI]);
	BIndex = testI;
	testI = createUIElement("Thickness", UI_TYPE_SLIDER);
	setSliderMinMax(&Sliders[testI], 1, 25);
	setSliderRotation(&Sliders[testI], UI_SLIDER_VERTICAL);
	setSliderLength(&Sliders[testI], 50);
	addSliderText(&Sliders[testI]);
	ThickIndex = testI;
}

void renderButton(Button *button, int *offset) {
	button->rect.x = *offset + 10;
	button->rect.y = UIOffsetY / 2 - button->rect.h / 2;
	SDL_Rect tmp = button->rect;
	tmp.x -= 2;
	tmp.y -= 2;
	tmp.h += 4;
	tmp.w += 4;
	*offset += button->rect.w + 4 + 10;
	SDL_SetRenderDrawColor(renderer, 90, 90, 90, 255);
	SDL_RenderFillRect(renderer, &tmp);
	SDL_RenderCopy(renderer, button->tex, NULL, &button->rect);
}

void renderSwitch() {

}

void renderSlider(Slider *slider, int *offset) {
	slider->rect.x = *offset + 10;
	slider->rect.y = UIOffsetY / 2 - slider->rect.h / 2;
	SDL_Rect tmpRect;
	if (slider->text) {
		slider->rect.y -= 6;
		SDL_Color Colour = {255, 255, 255, 255};
		char text[64];
		sprintf(text, "%.1f", slider->value);
		SDL_Surface *TextSurface = TTF_RenderText_Blended(SliderLabel, text, Colour);
		if (!TextSurface) {
			warn("Failed to set button text.\nThe button will be blank.\n");
			return;
		}
		// Create the texture if the text was make correctly.
		SDL_Texture *tmpTex = SDL_CreateTextureFromSurface(renderer, TextSurface);
		SDL_FreeSurface(TextSurface);
		// Prepare the rendering rect
		SDL_QueryTexture(tmpTex, NULL, NULL, &tmpRect.w, &tmpRect.h);
		// Prepare the SDL_Rect
		tmpRect.x = slider->rect.x + (slider->rect.w / 2) - (tmpRect.w / 2);
		tmpRect.y = slider->rect.y + slider->rect.h + 2;
		SDL_RenderCopy(renderer, tmpTex, NULL, &tmpRect);
		SDL_DestroyTexture(tmpTex);
	}
	if (strcmp(slider->name, "R") == 0) {
		ColourPreview.y = tmpRect.y + tmpRect.h + 1;
		ColourPreview.x = tmpRect.x + 2;
		ColourPreview.h = 3;
		ColourPreview.w += slider->rect.w;
		slider->name = strdup("R2");
	} else if (strcmp(slider->name, "G") == 0) {
		ColourPreview.w += slider->rect.w + 10;
		slider->name = strdup("G2");
	} else if (strcmp(slider->name, "B") == 0) {
		ColourPreview.w += slider->rect.w;
		slider->name = strdup("B2");
	}
	double percentage = (slider->value - slider->min) / (slider->max - slider->min);
	SDL_Rect tmp;
	switch (slider->rotation) {
		case UI_SLIDER_HORIZONTAL:
			tmp.x = 2 + slider->rect.x + (percentage * slider->rect.w);
			tmp.w = 2;
			tmp.h = slider->rect.h;
			tmp.y = slider->rect.y;
			break;
		case UI_SLIDER_VERTICAL:
			tmp.x = slider->rect.x;
			tmp.w = slider->rect.w;
			tmp.h = 2;
			tmp.y = slider->rect.y + slider->rect.h - (percentage * slider->rect.h);
			break;
	}
	*offset += slider->rect.w + 10;
	SDL_SetRenderDrawColor(renderer, 90, 90, 90, 255);
	SDL_RenderDrawRect(renderer, &slider->rect);
	SDL_RenderFillRect(renderer, &tmp);
}