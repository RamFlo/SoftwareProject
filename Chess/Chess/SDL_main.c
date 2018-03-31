#include <stdbool.h>
#include "SDL_MainAux.h"
#include "MainAux.h"



int SDL_main() {
	shouldRenderSameScreenAgain = true;
	SDL_Event e;
	int lastHandledScreen = 0;
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
			sendEventToButtons(&e, curScreen);
			if (curScreen != lastHandledScreen || shouldRenderSameScreenAgain)
				break;
			//button->handleEvent(button, &e);
			//switch (e.type) {
			//case SDL_QUIT:
			//	done = 1;
			//	break;
			//case SDL_KEYDOWN:
			//	if (e.key.keysym.sym == SDLK_ESCAPE)
			//		done = 1;
			//	break;
			}
		if (curScreen != lastHandledScreen || shouldRenderSameScreenAgain) {//check if needed
			SwitchOrRenderScreen(lastHandledScreen);
			shouldRenderSameScreenAgain = false;
			lastHandledScreen = curScreen;
		}
	}
	return 0;
}