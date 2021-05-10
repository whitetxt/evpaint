typedef struct {
	int width;
	int height;
	SDL_Texture *tex;
	SDL_Rect rect;
	Uint32 *pixels;
} Canvas;

extern Canvas *canvases;
extern size_t numCanvases;
extern size_t currentCanvas;

void renderCanvas(Canvas *canvas);
int saveCanvas(Canvas canvas, char *filename);
void saveCurrentCanvas();
int createCanvas(int width, int height);
void createCanvasPopup();
void setPixel(int x, int y);
void setLine(int prevx, int prevy, int x, int y);
Uint32 getColour();
int roundI(double number);