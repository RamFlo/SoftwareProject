#ifndef CHESSWINDOW_H_
#define CHESSWINDOW_H_
#include "Button.h"
#include <SDL.h>
#include <SDL_video.h>
#define MAX_BUTTON_AMOUNT 50

typedef struct sdl_window_t {
	Widget* buttons[MAX_BUTTON_AMOUNT];
	SDL_Renderer* renderer;
	SDL_Rect rectangles[MAX_BUTTON_AMOUNT];
	SDL_Window* window;
	//int curSize;
} chessWindow;

chessWindow* createChessWindow(SDL_Window* window, SDL_Renderer* renderer);

void destroyChessWindow(chessWindow* myChessWindow);
#endif
