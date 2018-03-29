#include <stdbool.h>
#include "SDL_MainAux.h"
#include "MainAux.h"



int SDL_main() {
	int lastHandledScreen = -1;
	g = ChessGameCreate(HISTORY_SIZE);
	if (g == NULL)
		exit(0);
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		printf("ERROR: unable to init SDL: %s\n",SDL_GetError());
		return 0;
	}
	curScreen = MAIN_WINDOW_INDEX;
	if (!initializeAllWindows()) {
		destroySDL();
		printf("ERROR: memory allocation error");
		exit(0);
	}
	while (true) {
		if (curScreen != lastHandledScreen) {
			if (lastHandledScreen != -1)
				SDL_HideWindow(chessWindowsArray[lastHandledScreen]->window);
			SDL_ShowWindow(chessWindowsArray[curScreen]->window);
			SDL_SetRenderDrawColor(chessWindowsArray[curScreen]->renderer, 255, 255, 255, 0);
			SDL_RenderClear(chessWindowsArray[curScreen]->renderer);
			drawWindowButtons(curScreen);
			SDL_RenderPresent(chessWindowsArray[curScreen]->renderer);
			lastHandledScreen = curScreen;
		}
	}
	return 0;
}