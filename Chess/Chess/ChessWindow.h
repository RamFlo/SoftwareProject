#ifndef CHESSWINDOW_H_
#define CHESSWINDOW_H_
#include "Button.h"
#include <SDL.h>
#include <SDL_video.h>
#define MAX_BUTTON_AMOUNT 400

typedef struct sdl_window_t {
	Widget* buttons[MAX_BUTTON_AMOUNT];
	SDL_Renderer* renderer;
	SDL_Window* window;
	//int curSize;
} chessWindow;

chessWindow* createChessWindow(SDL_Window* window, SDL_Renderer* renderer);

void destroyChessWindow(chessWindow* myChessWindow);
#endif
