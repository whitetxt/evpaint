typedef struct {
	SDL_Texture *tex;
	SDL_Rect rect;
	char *name;
	char *text;
	void (*func)();
} Button;

typedef struct {
	SDL_Rect rect;
	char *name;
	int rotation;
	int length;
	double min;
	double max;
	double value;
	bool text;
	void (*func)(double val);
} Slider;

typedef struct {
	SDL_Texture *tex;
	SDL_Rect rect;
	char *name;
	bool active;
	void (*func)(bool val);
} Switch;

#define UI_TYPE_BUTTON 0
#define UI_TYPE_SLIDER 1
#define UI_TYPE_SWITCH 2

#define UI_SLIDER_VERTICAL 0
#define UI_SLIDER_HORIZONTAL 1

extern Button *Buttons;
extern size_t NumButtons;
extern Slider *Sliders;
extern size_t NumSliders;
extern Switch *Switches;
extern size_t NumSwitches;

extern int UIOffsetX;
extern int UIOffsetY;

extern int RIndex;
extern int GIndex;
extern int BIndex;
extern int ThickIndex;

extern SDL_Rect ColourPreview;

void initFont();
int createUIElement(char *name, int type);
void addButtonText(Button *button, char *text);
void setSliderMinMax(Slider *slider, float min, float max);
void setSliderRotation(Slider *slider, int rotation);
void setSliderLength(Slider *slider, int length);
void createUI();

void renderButton(Button *button, int *offset);
void renderSwitch();
void renderSlider(Slider *slider, int *offset);