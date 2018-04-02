#include <stdbool.h>
#include "SDL_MainAux.h"
#include "MainAux.h"

int SDL_chessGame() {
	shouldRenderSameScreenAgain = true;
	SDL_Event e;
	int lastHandledScreen = 0;
	curGameSaved = true;
	g = ChessGameCreate(HISTORY_SIZE);
	if (g == NULL) {
		exit(0);
	}	
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		printf("ERROR: unable to init SDL: %s\n",SDL_GetError());
		ChessGameDestroy(g);
		return 0;
	}
	programWindow = SDL_CreateWindow("Chess Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_SHOWN);
	if (programWindow == NULL)
		exit(0);
	curScreen = MAIN_WINDOW_INDEX;
	if (!initializeAllWindows()) {
		destroySDL();
		free(programWindow);
		ChessGameDestroy(g);
		printf("ERROR: memory allocation error");
		exit(0);
	}
	while (true) {
		while (SDL_PollEvent(&e)) {
			if (e.type == SDL_QUIT) {//DOESN'T WORK!
				quitGameButtonClick();
				break;
			}
			sendEventToButtons(&e, curScreen);
			if (curScreen != lastHandledScreen || shouldRenderSameScreenAgain)
				break;
		}
		if (curScreen != lastHandledScreen || shouldRenderSameScreenAgain) {//check if needed
			SwitchOrRenderScreen(lastHandledScreen);
			shouldRenderSameScreenAgain = false;
			lastHandledScreen = curScreen;
		}
	}
	return 0;
}