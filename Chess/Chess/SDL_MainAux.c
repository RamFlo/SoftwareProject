#include <SDL.h>
#include <SDL_video.h>
#include <stdio.h>
#include <stdbool.h>
#include "Button.h"
#include "ChessWindow.h"
#include "SDL_MainAux.h"



SDL_Window* settingsWindow = NULL;
SDL_Window* loadWindow = NULL;
SDL_Window* gameWindow = NULL;
SDL_Window* saveWindow = NULL;



void newGameButtonClick(void) {
	//SDL_HideWindow(mainWindow);
	//showSettingsWindow();
}

void loadGameButtonClick(void) {
	//SDL_HideWindow(mainWindow);
	//showLoadWindow();
}

void quitGameButtonClick(void) {

}

chessWindow* createMainWindow() {
	SDL_Window* mainWindow = SDL_CreateWindow("Main Window", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_SHOWN);
	if (mainWindow == NULL)
		return NULL;
	SDL_Renderer* rend = SDL_CreateRenderer(mainWindow, -1, SDL_RENDERER_SOFTWARE);
	if (rend == NULL) {
		free(mainWindow);
		return NULL;
	}
	chessWindowsArray[MAIN_WINDOW_INDEX] = createChessWindow(mainWindow, rend);
	
	SDL_Rect newGameButtonRect = { .x = 200,.y = 100,.w = 400,.h = 100 };
	SDL_Rect loadGameButtonRect = { .x = 200,.y = 250,.w = 400,.h = 100 };
	SDL_Rect quitGameButtonRect = { .x = 200,.y = 400,.w = 400,.h = 100 };

	chessWindowsArray[MAIN_WINDOW_INDEX]->rectangles[0] = newGameButtonRect;
	chessWindowsArray[MAIN_WINDOW_INDEX]->rectangles[1] = loadGameButtonRect;
	chessWindowsArray[MAIN_WINDOW_INDEX]->rectangles[2] = quitGameButtonRect;

	Widget* newGameButton = createButton(rend, "assets\\mainWindow_newGameButton.bmp", newGameButtonRect, newGameButtonClick);
	Widget* loadGameButton = createButton(rend, "assets\\mainWindow_loadGameButton.bmp", loadGameButtonRect, loadGameButtonClick);
	Widget* quitGameButton = createButton(rend, "assets\\mainWindow_quitGameButton.bmp", quitGameButtonRect, quitGameButtonClick);

	chessWindowsArray[MAIN_WINDOW_INDEX]->buttons[0] = newGameButton;
	chessWindowsArray[MAIN_WINDOW_INDEX]->buttons[1] = loadGameButton;
	chessWindowsArray[MAIN_WINDOW_INDEX]->buttons[2] = quitGameButton;

	SDL_HideWindow(chessWindowsArray[MAIN_WINDOW_INDEX]->window);

	return chessWindowsArray[MAIN_WINDOW_INDEX];
}

bool initializeAllWindows() {
	createMainWindow();
	return true;
}






















void destroyAllWindows() {
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





void showSettingsWindow() {

}

void showLoadWindow() {

}