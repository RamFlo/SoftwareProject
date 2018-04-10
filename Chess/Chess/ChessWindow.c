#include "ChessWindow.h"

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
chessWindow* createChessWindow(SDL_Window* window, SDL_Renderer* renderer) {
	chessWindow* newChessWindow;
	int i;
	newChessWindow = (chessWindow*)malloc(sizeof(chessWindow));
	if (newChessWindow == NULL)
		return NULL;
	newChessWindow->window = window;
	newChessWindow->renderer = renderer;
	for (i = 0; i < MAX_BUTTON_AMOUNT; i++)
		newChessWindow->buttons[i] = NULL;
	return newChessWindow;
}

/**
* Destroys the chessWindow pointed by myChessWindow pointer (frees all of it's resources)
* @param myChessWindow - a pointer to a chessWindow
*/
void destroyChessWindow(chessWindow* myChessWindow) {
	int i;
	for (i = 0; i < MAX_BUTTON_AMOUNT; i++) {
		if (myChessWindow->buttons[i] != NULL) {
			//if (i >= 11 && i<=41)
			//	continue;
			myChessWindow->buttons[i]->destroy(myChessWindow->buttons[i]);
		}
	}
	SDL_DestroyRenderer(myChessWindow->renderer);
	SDL_DestroyWindow(myChessWindow->window);
	free(myChessWindow);
}

