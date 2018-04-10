#include <stdbool.h>
#include "SDL_MainAux.h"
#include "MainAux.h"

/*
* This function is the main game's function for graphic mode.
* It created a new game and all the game program's windows using initializeAllWindows.
* It operates using an outer 'while' loop the polls the current event and acts accordignly (changing and rednering screens).
* In case of a memory error the function ends the game (frees all allocated memory).
* @return
* Undefined value
*/
int SDL_chessGame() {
	shouldRenderSameScreenAgain = true;
	SDL_Event e;
	int lastHandledScreen = 0;

	curGameSaved = true;
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
		while (SDL_PollEvent(&e)) {
			if (e.type == SDL_QUIT || e.window.event== SDL_WINDOWEVENT_CLOSE) {
				quitGameButtonClick();
				break;
			}
			sendEventToButtons(&e, curScreen);
			if (curScreen != lastHandledScreen || shouldRenderSameScreenAgain)
				break;
		}
		if (curScreen != lastHandledScreen || shouldRenderSameScreenAgain) {
			SwitchOrRenderScreen(lastHandledScreen);
			shouldRenderSameScreenAgain = false;
			lastHandledScreen = curScreen;
		}
	}
	return 0;
}