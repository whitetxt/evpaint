extern int width;
extern int height;

void handleEvents();
SDL_bool mouseCanvasCollision();
void printRect(SDL_Rect rect);
void setSliderValue(int i, SDL_Rect mouse);