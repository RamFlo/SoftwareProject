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

void buttonThatDoesNothing() {
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

void moveGameModeChoiceLine() {
	if (g->gameMode == 1)
		((Button*)(chessWindowsArray[SETTINGS_WINDOW_INDEX]->buttons[0]->data))->location.x = 240;
	else
		((Button*)(chessWindowsArray[SETTINGS_WINDOW_INDEX]->buttons[0]->data))->location.x = 350;
}

void gameModeButtonClick() {
	if (lastClickPoint.x >= 240 && lastClickPoint.x <= 340)
		g->gameMode = 1;
	else
		g->gameMode = 2;
	moveGameModeChoiceLine();
	shouldRenderSameScreenAgain = true;
}

void moveDiffLevelChoiceLine() {
	if (g->difficulty == 1) 
		((Button*)(chessWindowsArray[MAIN_WINDOW_INDEX]->buttons[1]->data))->location.x = 240;
	else if (g->difficulty == 2)
		((Button*)(chessWindowsArray[MAIN_WINDOW_INDEX]->buttons[1]->data))->location.x = 350;
	else if (g->difficulty == 3)
		((Button*)(chessWindowsArray[MAIN_WINDOW_INDEX]->buttons[1]->data))->location.x = 460;
	else if (g->difficulty == 4)
		((Button*)(chessWindowsArray[MAIN_WINDOW_INDEX]->buttons[1]->data))->location.x = 570;
	else
		((Button*)(chessWindowsArray[MAIN_WINDOW_INDEX]->buttons[1]->data))->location.x = 680;
}

void diffLevelButtonClick() {
	if (lastClickPoint.x >= 240 && lastClickPoint.x <= 340)
		g->difficulty = 1;
	else if (lastClickPoint.x >= 350 && lastClickPoint.x <= 450)
		g->difficulty = 2;
	else if (lastClickPoint.x >= 460 && lastClickPoint.x <=560)
		g->difficulty = 3;
	else if (lastClickPoint.x >= 570 && lastClickPoint.x <= 670)
		g->difficulty = 4;
	else 
		g->difficulty = 5;
	moveDiffLevelChoiceLine();
	shouldRenderSameScreenAgain = true;
}

void startButtonClick() {
	curScreen = BOARD_WINDOW_INDEX;
}

void moveuserColorChoiceLine() {
	if (g->userColor == 1) {
		((Button*)(chessWindowsArray[SETTINGS_WINDOW_INDEX]->buttons[2]->data))->location.x = 240;
	}
	else {
		((Button*)(chessWindowsArray[SETTINGS_WINDOW_INDEX]->buttons[2]->data))->location.x = 350;
	}
}

void userColorButtonClick() {
	if (lastClickPoint.x >= 240 && lastClickPoint.x <= 340)
		g->userColor = 1;
	else 
		g->userColor = 0;
	moveuserColorChoiceLine();
	shouldRenderSameScreenAgain = true;
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
	if (ChessGameLoad(g, loadedGamePath) == SUCCESS) {
		curScreen = SETTINGS_WINDOW_INDEX;
		moveGameModeChoiceLine();
		moveDiffLevelChoiceLine();
		moveUserColorChoiceLine();
	}
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
	SDL_Window* settingsWindow = SDL_CreateWindow("Settings Window", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_SHOWN);
	if (loadWindow == NULL)
		return NULL;
	SDL_Renderer* rend = SDL_CreateRenderer(settingsWindow, -1, SDL_RENDERER_SOFTWARE);
	if (rend == NULL) {
		free(settingsWindow);
		return NULL;
	}
	chessWindowsArray[SETTINGS_WINDOW_INDEX] = createChessWindow(settingsWindow, rend);
	
	//choice marks - done
	SDL_Rect gameModeChoiceRect = { .x = 240,.y = 113,.w = 100,.h = 3 };
	SDL_Rect diffLevelChoiceRect = { .x = 350,.y = 213,.w = 100,.h = 3 };
	SDL_Rect userColorChoiceRect = { .x = 240,.y = 313,.w = 100,.h = 3 };


	//back-done
	SDL_Rect backButtonRect = { .x = 20,.y = 20,.w = 80,.h = 20 };

	//left buttons-done
	SDL_Rect gameModeRect = { .x = 20,.y = 60,.w = 200,.h = 50 };
	SDL_Rect diffLevelRect = { .x = 20,.y = 160,.w = 200,.h = 50 };
	SDL_Rect userColorRect = { .x = 20,.y =260,.w = 200,.h = 50 };

	//Game Mode Buttons-done
	SDL_Rect onePlayerButtonRect = { .x = 240 ,.y = 85 ,.w = 100,.h = 25 };
	SDL_Rect twoPlayersButtonRect = { .x = 350 ,.y = 85 ,.w = 100,.h = 25 };

	//difficulty level buttons-done
	SDL_Rect diffAmateurButtonRect = { .x = 240,.y = 185,.w = 100,.h = 25 };
	SDL_Rect diffeasyButtonRect = { .x = 350 ,.y = 185 ,.w = 100,.h = 25 };
	SDL_Rect diffmoderateButtonRect = { .x = 460 ,.y = 185 ,.w = 100,.h = 25 };
	SDL_Rect diffhardButtonRect = { .x = 570 ,.y = 185 ,.w = 100,.h = 25 };
	SDL_Rect diffExpertButtonRect = { .x = 680 ,.y = 185 ,.w = 100,.h = 25 };
	
	//User Color Buttons-done
	SDL_Rect whiteButtonRect = { .x = 240 ,.y = 285 ,.w = 100,.h = 25 };
	SDL_Rect blackButtonRect = { .x = 350 ,.y = 285 ,.w = 100,.h = 25 };

	//start button-done
	SDL_Rect startButtonRect = { .x = 250,.y = 420,.w = 300,.h = 75 };
	
	//choice mark buttons
	chessWindowsArray[SETTINGS_WINDOW_INDEX]->buttons[0] = createButton(rend, "assets/backButton.bmp", gameModeChoiceRect, buttonThatDoesNothing);
	chessWindowsArray[SETTINGS_WINDOW_INDEX]->buttons[1] = createButton(rend, "assets/backButton.bmp", diffLevelChoiceRect, buttonThatDoesNothing);
	chessWindowsArray[SETTINGS_WINDOW_INDEX]->buttons[2] = createButton(rend, "assets/backButton.bmp", userColorChoiceRect, buttonThatDoesNothing);


	//back button-done
	chessWindowsArray[SETTINGS_WINDOW_INDEX]->buttons[3] = createButton(rend, "assets/backButton.bmp", backButtonRect, backButtonClick);

	//left side buttons
	chessWindowsArray[SETTINGS_WINDOW_INDEX]->buttons[4] = createButton(rend, "assets/backButton.bmp", gameModeRect, buttonThatDoesNothing);
	chessWindowsArray[SETTINGS_WINDOW_INDEX]->buttons[5] = createButton(rend, "assets/backButton.bmp", diffLevelRect, buttonThatDoesNothing);
	chessWindowsArray[SETTINGS_WINDOW_INDEX]->buttons[6] = createButton(rend, "assets/backButton.bmp", userColorRect, buttonThatDoesNothing);

	//game mode buttons
	chessWindowsArray[SETTINGS_WINDOW_INDEX]->buttons[7] = createButton(rend, "assets/backButton.bmp", onePlayerButtonRect, gameModeButtonClick);
	chessWindowsArray[SETTINGS_WINDOW_INDEX]->buttons[8] = createButton(rend, "assets/backButton.bmp", twoPlayersButtonRect, gameModeButtonClick);

	//diff level buttons
	chessWindowsArray[SETTINGS_WINDOW_INDEX]->buttons[9] = createButton(rend, "assets/backButton.bmp", diffAmateurButtonRect, diffLevelButtonClick);
	chessWindowsArray[SETTINGS_WINDOW_INDEX]->buttons[10] = createButton(rend, "assets/backButton.bmp", diffeasyButtonRect, diffLevelButtonClick);
	chessWindowsArray[SETTINGS_WINDOW_INDEX]->buttons[11] = createButton(rend, "assets/backButton.bmp", diffmoderateButtonRect, diffLevelButtonClick);
	chessWindowsArray[SETTINGS_WINDOW_INDEX]->buttons[12] = createButton(rend, "assets/backButton.bmp", diffhardButtonRect, diffLevelButtonClick);
	chessWindowsArray[SETTINGS_WINDOW_INDEX]->buttons[13] = createButton(rend, "assets/backButton.bmp", diffExpertButtonRect, diffLevelButtonClick);
	
	//user color buttons
	chessWindowsArray[SETTINGS_WINDOW_INDEX]->buttons[14] = createButton(rend, "assets/backButton.bmp", onePlayerButtonRect, userColorButtonClick);
	chessWindowsArray[SETTINGS_WINDOW_INDEX]->buttons[15] = createButton(rend, "assets/backButton.bmp", twoPlayersButtonRect, userColorButtonClick);

	//start button
	chessWindowsArray[SETTINGS_WINDOW_INDEX]->buttons[16] = createButton(rend, "assets/backButton.bmp", startButtonRect,startButtonClick);


	
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