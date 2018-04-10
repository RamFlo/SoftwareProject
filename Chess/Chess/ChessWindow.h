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

/**
* Returns pointer to a chessWindow
* with the assertion that all arguments are valid. If any of the arguments is
* invalid then NULL is returned.
* @param window - a pointer to a window
* @param renderer - a pointer to a renderer
* @return
* NULL on invalid args or memory error
* Otherwise, a pointer to the created chessWindow
*/
chessWindow* createChessWindow(SDL_Window* window, SDL_Renderer* renderer);

/**
* Destroys the chessWindow pointed by myChessWindow pointer (frees all of it's resources)
* @param myChessWindow - a pointer to a chessWindow
*/
void destroyChessWindow(chessWindow* myChessWindow);
#endif
