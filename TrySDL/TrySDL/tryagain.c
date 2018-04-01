#include <SDL.h>
#include <SDL_video.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>

int main() {
	//shouldRenderSameScreenAgain = true;
	SDL_Event e;
	int lastHandledScreen = 0;
	//curGameSaved = true;
	//g = ChessGameCreate(HISTORY_SIZE);
	//if (g == NULL)
	//	exit(0);
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		printf("ERROR: unable to init SDL: %s\n", SDL_GetError());
		return 0;
	}
	//curScreen = MAIN_WINDOW_INDEX;
	//if (!initializeAllWindows()) {
	//	destroySDL();
	//	printf("ERROR: memory allocation error");
	//	exit(0);
	//}
	while (1==1) {
		while (SDL_PollEvent(&e)) {
			if (e.type == SDL_QUIT) {//DOESN'T WORK!
				exit(0);
				break;
			}
		}

	}
	return 0;
}