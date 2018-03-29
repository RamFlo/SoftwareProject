#include "ChessWindow.h"

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

void destroyChessWindow(chessWindow* myChessWindow) {
	int i;
	for (i = 0; i < MAX_BUTTON_AMOUNT; i++) {
		if (myChessWindow->buttons[i]!=NULL)
			myChessWindow->buttons[i]->destroy(myChessWindow->buttons[i]);
	}
	SDL_DestroyRenderer(myChessWindow->renderer);
	SDL_DestroyWindow(myChessWindow->window);
	free(myChessWindow);
}

