#include <stdio.h>
#include <stdbool.h>
#include "Button.h"
#include "ChessWindow.h"
#include "SDL_MainAux.h"
#include "MainAux.h"

int curScreen;
int curFirstSlotOnScreen;
int previousScreen;
bool shouldRenderSameScreenAgain;
bool curGameSaved;
bool shouldQuit;
ChessGame* g;
chessWindow* chessWindowsArray[NUM_OF_WINDOWS];
SDL_Window* settingsWindow = NULL;
SDL_Window* loadWindow = NULL;
SDL_Window* gameWindow = NULL;
SDL_Window* saveWindow = NULL;
SDL_Point lastClickPoint;

void destroySDL() {
	int i = 0;
	if (g != NULL)
		ChessGameDestroy(g);
	for (i = 0; i < NUM_OF_WINDOWS; i++) {
		if (chessWindowsArray[i] != NULL)
			destroyChessWindow(chessWindowsArray[i]);
	}
	SDL_Quit();
	exit(0);
}

void drawAllWindowButtons(int windowIndex) {
	int i = 0;
	chessWindow* curWindow = chessWindowsArray[windowIndex];
	while (curWindow->buttons[i] != NULL) {
		curWindow->buttons[i]->draw(curWindow->buttons[i], curWindow->renderer);
		i++; 
	}
}

void sendEventToButtons(SDL_Event* e, int windowIndex) {
	Widget* curButton;
	int i = 0,lastIndex=0;
	chessWindow* curWindow = chessWindowsArray[windowIndex];
	lastIndex = (curFirstSlotOnScreen + NUM_OF_SCREEN_SLOTS) < NUM_OF_SAVE_SLOTS ? (curFirstSlotOnScreen + NUM_OF_SCREEN_SLOTS) : NUM_OF_SAVE_SLOTS;
	if (windowIndex == LOAD_WINDOW_INDEX || windowIndex == SAVE_WINDOW_INDEX) {
		curWindow->buttons[0]->handleEvent(curWindow->buttons[0], e);
		curWindow->buttons[1]->handleEvent(curWindow->buttons[1], e);
		curWindow->buttons[2]->handleEvent(curWindow->buttons[2], e);
		for (i = curFirstSlotOnScreen + 3; i < lastIndex + 3; i++)
			curWindow->buttons[i]->handleEvent(curWindow->buttons[i], e);
	}
	else {
		while ((curButton = curWindow->buttons[i]) != NULL) {
			curButton->handleEvent(curButton, e);
			i++;
		}
	}
}

void SwitchOrRenderScreen(int lastHandledScreen) {
	if (curScreen != lastHandledScreen) {
		previousScreen = lastHandledScreen;
		curFirstSlotOnScreen = 0;
		SDL_HideWindow(chessWindowsArray[lastHandledScreen]->window);
	}
	SDL_ShowWindow(chessWindowsArray[curScreen]->window);
	SDL_SetRenderDrawColor(chessWindowsArray[curScreen]->renderer, 255, 255, 255, 0);
	SDL_RenderClear(chessWindowsArray[curScreen]->renderer);
	drawWindowButtons(curScreen);
	SDL_RenderPresent(chessWindowsArray[curScreen]->renderer);
}

void saveOrLoadDrawWindowsButton(int windowIndex) {
	int i = 0,lastIndex=0;
	chessWindow* curWindow = chessWindowsArray[windowIndex];
	lastIndex = (curFirstSlotOnScreen + NUM_OF_SCREEN_SLOTS) < NUM_OF_SAVE_SLOTS ? (curFirstSlotOnScreen + NUM_OF_SCREEN_SLOTS) : NUM_OF_SAVE_SLOTS;
	curWindow->buttons[0]->draw(curWindow->buttons[0], curWindow->renderer);
	curWindow->buttons[1]->draw(curWindow->buttons[1], curWindow->renderer);
	curWindow->buttons[2]->draw(curWindow->buttons[2], curWindow->renderer);
	for (i = curFirstSlotOnScreen + 3; i < lastIndex + 3; i++)
		curWindow->buttons[i]->draw(curWindow->buttons[i], curWindow->renderer);
}

void drawWindowButtons(int windowIndex) {
	switch (windowIndex) {
	case MAIN_WINDOW_INDEX:
	case SETTINGS_WINDOW_INDEX:
	case BOARD_WINDOW_INDEX:
		drawAllWindowButtons(windowIndex);
		break;
	case LOAD_WINDOW_INDEX:
	case SAVE_WINDOW_INDEX:
		saveOrLoadDrawWindowsButton(windowIndex);
		break;
	}
}

void newGameButtonClick(void) {
	//SDL_HideWindow(mainWindow);
	//showSettingsWindow();
}

void loadGameButtonClick(void) {//subject to renaming
	curScreen = LOAD_WINDOW_INDEX;
}

int unsavedGameBeforeLeaving() {
	const SDL_MessageBoxButtonData buttons[] = {
		{ 0, 0, "no" },
	{ SDL_MESSAGEBOX_BUTTON_RETURNKEY_DEFAULT, 1, "yes" },
	{ SDL_MESSAGEBOX_BUTTON_ESCAPEKEY_DEFAULT, 2, "cancel" },
	};
	const SDL_MessageBoxData messageboxdata = {
		SDL_MESSAGEBOX_INFORMATION,
		NULL,
		"Would you like to save your game first?",
		"The current game is unsaved. Would you like to save it?",
		SDL_arraysize(buttons),
		buttons,
	};
	int buttonid;
	if (SDL_ShowMessageBox(&messageboxdata, &buttonid) < 0) {
		printf("ERROR: unable to display message box: %s\n", SDL_GetError());
		destroySDL();
	}
	return buttonid;
}

void quitGameButtonClick() {
	int chosenButtonID = 0;
	if (!curGameSaved) {
		chosenButtonID = unsavedGameBeforeLeaving();
		if (chosenButtonID == 0)
			destroySDL();
		else if (chosenButtonID == 1) {
			shouldQuit = true;
			curScreen = SAVE_WINDOW_INDEX;
		}
		//else if (buttonid == -1 || buttonid == 2) {closed window or canceled}
	}
	else 
		destroySDL();
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
	Widget* newGameButton = createButton(rend, "assets/mainWindow_newGameButton.bmp", newGameButtonRect, newGameButtonClick);
	Widget* loadGameButton = createButton(rend, "assets/mainWindow_loadGameButton.bmp", loadGameButtonRect, loadGameButtonClick);
	Widget* quitGameButton = createButton(rend, "assets/mainWindow_quitGameButton.bmp", quitGameButtonRect, quitGameButtonClick);
	chessWindowsArray[MAIN_WINDOW_INDEX]->buttons[0] = newGameButton;
	chessWindowsArray[MAIN_WINDOW_INDEX]->buttons[1] = loadGameButton;
	chessWindowsArray[MAIN_WINDOW_INDEX]->buttons[2] = quitGameButton;
	if (newGameButton == NULL || loadGameButton == NULL || quitGameButton == NULL)
		return NULL;
	SDL_HideWindow(chessWindowsArray[MAIN_WINDOW_INDEX]->window);
	return chessWindowsArray[MAIN_WINDOW_INDEX];
}

void backButtonClick() {
	curScreen = previousScreen;
}

void leftArrowButtonClick() {
	if (curFirstSlotOnScreen > 0) {
		shouldRenderSameScreenAgain = true;
		curFirstSlotOnScreen -= NUM_OF_SCREEN_SLOTS;
	}
}

void rightArrowButtonClick() {
	if (curFirstSlotOnScreen + NUM_OF_SCREEN_SLOTS < NUM_OF_SAVE_SLOTS) {
		shouldRenderSameScreenAgain = true;
		curFirstSlotOnScreen += NUM_OF_SCREEN_SLOTS;
	}	
}

//1-Indexed
int calcClickedSlotNumber() {
	return (lastClickPoint.y - 75) / 100 + 1 + curFirstSlotOnScreen;
}

void loadSlotButtonClick() {
	char loadedGamePath[50];
	int i = 0;
	for (i = 0; i < 50; i++)
		loadedGamePath[i] = '\0';
	sprintf(loadedGamePath, "savedGames/%d.txt", calcClickedSlotNumber());
	if (ChessGameLoad(g, loadedGamePath) == SUCCESS)
		curScreen = SETTINGS_WINDOW_INDEX;
}

chessWindow* createLoadWindow() {
	SDL_Rect slotsRects[5];
	char curSlotImagePath[50];
	int i = 0,curPos=0;
	for (i = 0; i < 50; i++)
		curSlotImagePath[i] = '\0';
	SDL_Window* loadWindow = SDL_CreateWindow("Load Window", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_SHOWN);
	if (loadWindow == NULL)
		return NULL;
	SDL_Renderer* rend = SDL_CreateRenderer(loadWindow, -1, SDL_RENDERER_SOFTWARE);
	if (rend == NULL) {
		free(loadWindow);
		return NULL;
	}
	chessWindowsArray[LOAD_WINDOW_INDEX] = createChessWindow(loadWindow, rend);
	SDL_Rect leftArrowRect = { .x = 125,.y = 275,.w = 50,.h = 50 };
	SDL_Rect rightArrowRect = { .x = 625,.y = 275,.w = 50,.h = 50 };
	SDL_Rect backButtonRect = { .x = 20,.y = 20,.w = 80,.h = 20 };
	SDL_Rect slotRect1 = { .x = 300,.y = 75,.w = 200,.h = 50 };
	SDL_Rect slotRect2 = { .x = 300,.y = 175,.w = 200,.h = 50 };
	SDL_Rect slotRect3 = { .x = 300,.y = 275,.w = 200,.h = 50 };
	SDL_Rect slotRect4 = { .x = 300,.y = 375,.w = 200,.h = 50 };
	SDL_Rect slotRect5 = { .x = 300,.y = 475,.w = 200,.h = 50 };
	slotsRects[0] = slotRect1;
	slotsRects[1] = slotRect2;
	slotsRects[2] = slotRect3;
	slotsRects[3] = slotRect4;
	slotsRects[4] = slotRect5;
	Widget* leftArrowButton = createButton(rend, "assets/load_saveWindow_leftArrow.bmp", leftArrowRect, leftArrowButtonClick);
	Widget* rightArrowButton = createButton(rend, "assets/load_saveWindow_rightArrow.bmp", rightArrowRect, rightArrowButtonClick);
	Widget* backButton = createButton(rend, "assets/backButton.bmp", backButtonRect, backButtonClick);
	chessWindowsArray[LOAD_WINDOW_INDEX]->buttons[0] = leftArrowButton;
	chessWindowsArray[LOAD_WINDOW_INDEX]->buttons[1] = rightArrowButton;
	chessWindowsArray[LOAD_WINDOW_INDEX]->buttons[2] = backButton;
	for (i = 3; i < 3 + NUM_OF_SAVE_SLOTS; i++) {
		curPos = (i - 3) % NUM_OF_SCREEN_SLOTS;
		sprintf(curSlotImagePath, "assets/load_saveWindowSlot%d.bmp", i - 2);
		chessWindowsArray[LOAD_WINDOW_INDEX]->buttons[i]= createButton(rend, curSlotImagePath, slotsRects[curPos], loadSlotButtonClick);
	}
	for (i = 0; i < 3 + NUM_OF_SAVE_SLOTS; i++) {
		if (chessWindowsArray[LOAD_WINDOW_INDEX]->buttons[i] == NULL)
			return NULL;
	}
	SDL_HideWindow(chessWindowsArray[LOAD_WINDOW_INDEX]->window);
	return chessWindowsArray[LOAD_WINDOW_INDEX];
}

chessWindow* createSettingsWindow() {
	SDL_Rect slotsRects[5];
	char curSlotImagePath[50];
	int i = 0, curPos = 0;
	for (i = 0; i < 50; i++)
		curSlotImagePath[i] = '\0';
	SDL_Window* loadWindow = SDL_CreateWindow("Load Window", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_SHOWN);
	if (loadWindow == NULL)
		return NULL;
	SDL_Renderer* rend = SDL_CreateRenderer(loadWindow, -1, SDL_RENDERER_SOFTWARE);
	if (rend == NULL) {
		free(loadWindow);
		return NULL;
	}
	chessWindowsArray[LOAD_WINDOW_INDEX] = createChessWindow(loadWindow, rend);
	SDL_Rect leftArrowRect = { .x = 125,.y = 275,.w = 50,.h = 50 };
	SDL_Rect rightArrowRect = { .x = 625,.y = 275,.w = 50,.h = 50 };
	SDL_Rect backButtonRect = { .x = 20,.y = 20,.w = 80,.h = 20 };
	SDL_Rect slotRect1 = { .x = 300,.y = 75,.w = 200,.h = 50 };
	SDL_Rect slotRect2 = { .x = 300,.y = 175,.w = 200,.h = 50 };
	SDL_Rect slotRect3 = { .x = 300,.y = 275,.w = 200,.h = 50 };
	SDL_Rect slotRect4 = { .x = 300,.y = 375,.w = 200,.h = 50 };
	SDL_Rect slotRect5 = { .x = 300,.y = 475,.w = 200,.h = 50 };
	slotsRects[0] = slotRect1;
	slotsRects[1] = slotRect2;
	slotsRects[2] = slotRect3;
	slotsRects[3] = slotRect4;
	slotsRects[4] = slotRect5;
	Widget* leftArrowButton = createButton(rend, "assets/load_saveWindow_leftArrow.bmp", leftArrowRect, leftArrowButtonClick);
	Widget* rightArrowButton = createButton(rend, "assets/load_saveWindow_rightArrow.bmp", rightArrowRect, rightArrowButtonClick);
	Widget* backButton = createButton(rend, "assets/backButton.bmp", backButtonRect, backButtonClick);
	chessWindowsArray[LOAD_WINDOW_INDEX]->buttons[0] = leftArrowButton;
	chessWindowsArray[LOAD_WINDOW_INDEX]->buttons[1] = rightArrowButton;
	chessWindowsArray[LOAD_WINDOW_INDEX]->buttons[2] = backButton;
	for (i = 3; i < 3 + NUM_OF_SAVE_SLOTS; i++) {
		curPos = (i - 3) % NUM_OF_SCREEN_SLOTS;
		sprintf(curSlotImagePath, "assets/load_saveWindowSlot%d.bmp", i - 2);
		chessWindowsArray[LOAD_WINDOW_INDEX]->buttons[i] = createButton(rend, curSlotImagePath, slotsRects[curPos], loadSlotButtonClick);
	}
	for (i = 0; i < 3 + NUM_OF_SAVE_SLOTS; i++) {
		if (chessWindowsArray[LOAD_WINDOW_INDEX]->buttons[i] == NULL)
			return NULL;
	}
	SDL_HideWindow(chessWindowsArray[LOAD_WINDOW_INDEX]->window);
	return chessWindowsArray[LOAD_WINDOW_INDEX];
}

bool initializeAllWindows() {
	if (createMainWindow() == NULL || createLoadWindow()==NULL)
		return false;
	return true;
}