#include <stdbool.h>
#include "SDL_MainAux.h"



int SDL_main() {
	int lastHandledScreen = -1;
	curScreen = MAIN_WINDOW_INDEX;
	if (!initializeAllWindows()) {}
	while (true) {
		if (curScreen != lastHandledScreen) {
			if (lastHandledScreen != -1)
				SDL_HideWindow(chessWindowsArray[lastHandledScreen]->window);
			SDL_ShowWindow(chessWindowsArray[curScreen]->window);
			SDL_SetRenderDrawColor(chessWindowsArray[curScreen]->renderer, 255, 255, 255, 0);
			SDL_RenderClear(chessWindowsArray[curScreen]->renderer);
			SDL_RenderPresent(chessWindowsArray[curScreen]->renderer);
			lastHandledScreen = curScreen;
		}
	}
	return 0;
}