#include <SDL.h>
#include <SDL_video.h>
#include <stdio.h>
#include "Button.h"


SDL_Window* mainWindow = NULL;
SDL_Window* settingsWindow = NULL;
SDL_Window* loadWindow = NULL;
SDL_Window* gameWindow = NULL;
SDL_Window* saveWindow = NULL;

void destroyAllWindows() {
	if (mainWindow != NULL)
		SDL_DestroyWindow(mainWindow);
	if (settingsWindow != NULL)
		SDL_DestroyWindow(settingsWindow);
	if (loadWindow != NULL)
		SDL_DestroyWindow(loadWindow);
	if (gameWindow != NULL)
		SDL_DestroyWindow(gameWindow);
	if (saveWindow != NULL)
		SDL_DestroyWindow(saveWindow);
}

void exitOnNullObject(char* nullObjectType) {
	printf("ERROR: unable to create %s: %s\n", nullObjectType,SDL_GetError());
	destroyAllWindows();
	SDL_Quit();
	exit(0);
}


void newGameButtonClick(void) {
	SDL_HideWindow(mainWindow);
	showSettingsWindow();
}

void loadGameButtonClick(void) {
	SDL_HideWindow(mainWindow);
	showLoadWindow();
}

void quitGameButtonClick(void) {

}

void showMainWindow() {
	if (mainWindow != NULL) {
		SDL_ShowWindow(mainWindow);
		return;
	}
	mainWindow = SDL_CreateWindow("Main Window", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_SHOWN);
	if (mainWindow == NULL)
		exitOnNullObject("window");
	SDL_Renderer* rend = SDL_CreateRenderer(mainWindow, -1, SDL_RENDERER_SOFTWARE);
	if (rend == NULL)
		exitOnNullObject("renderer");
	SDL_Rect newGameButtonRect = { .x = 200,.y = 100,.w = 400,.h = 100 };
	SDL_Rect loadGameButtonRect = { .x = 200,.y = 250,.w = 400,.h = 100 };
	SDL_Rect quitGameButtonRect = { .x = 200,.y = 400,.w = 400,.h = 100 };
	Widget* newGameButton = createButton(rend, "assets\mainWindow_newGameButton.bmp", newGameButtonRect, newGameButtonClick);
	Widget* loadGameButton = createButton(rend, "assets\mainWindow_loadGameButton.bmp", loadGameButtonRect, loadGameButtonClick);
	Widget* quitGameButton = createButton(rend, "assets\mainWindow_quitGameButton.bmp", quitGameButtonRect, quitGameButtonClick);
}

void showSettingsWindow() {

}

void showLoadWindow() {

}