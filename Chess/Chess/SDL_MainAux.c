#include <stdio.h>
#include <stdbool.h>
#include "Button.h"
#include "ChessWindow.h"
#include "SDL_MainAux.h"
#include "MainAux.h"
#include "ChessGame.h"
#include <ctype.h>

int curScreen;
int curFirstSlotOnScreen;
int previousScreen;
int shouldDrawPiece[32];
int getMovesShouldDraw[256];
int legalMovesForPieceShouldDraw[64];
int movingPieceLocation;
bool shouldRenderSameScreenAgain;
bool curGameSaved;
bool shouldQuit;
bool shouldReturnToMainMenu;
ChessGame* g;
chessWindow* chessWindowsArray[NUM_OF_WINDOWS];
SDL_Window* settingsWindow = NULL;
SDL_Window* loadWindow = NULL;
SDL_Window* gameWindow = NULL;
SDL_Window* saveWindow = NULL;
SDL_Point lastClickPoint;
SDL_Event* curEvent;

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

bool isCheckmateOrDraw() {
	return (g->checkmated != '\0' || g->draw);
}

void resetGetMovesShouldDrawArray() {
	int i = 0;
	for (i = 0; i < 256; i++)
		getMovesShouldDraw[i] = 0;
}

void resetLegalMovesShouldDrawArray() {
	int i = 0;
	for (i = 0; i < 64; i++)
		legalMovesForPieceShouldDraw[i] = 0;
}

void drawAllWindowButtons(int windowIndex) {
	int i = 0;
	chessWindow* curWindow = chessWindowsArray[windowIndex];
	while (curWindow->buttons[i] != NULL) {
		curWindow->buttons[i]->draw(curWindow->buttons[i], curWindow->renderer);
		i++; 
	}
}

void boardSendEventToButtons(SDL_Event* e) {
	int i = 0;
	chessWindow* curWindow = chessWindowsArray[BOARD_WINDOW_INDEX];
	for (i = 0; i < 6; i++)
		curWindow->buttons[i]->handleEvent(curWindow->buttons[i], e);
	if (curGameSaved)
		curWindow->buttons[6]->handleEvent(curWindow->buttons[6], e);
	else
		curWindow->buttons[7]->handleEvent(curWindow->buttons[7], e);
	for (i = 8; i < 11; i++)
		curWindow->buttons[i]->handleEvent(curWindow->buttons[i], e);
	for (i = 11; i < 43; i++)
		if (shouldDrawPiece[i - 11] == 1)
			curWindow->buttons[i]->handleEvent(curWindow->buttons[i], e);
	for (i = 43; i < 299; i++)
		if (getMovesShouldDraw[i - 43] == 1)
			curWindow->buttons[i]->handleEvent(curWindow->buttons[i], e);
	for (i = 299; i < 363; i++)
		if (legalMovesForPieceShouldDraw[i - 299] == 1)
			curWindow->buttons[i]->handleEvent(curWindow->buttons[i], e);
}

void settingsSendEventToButtons(SDL_Event* e) {
	int i = 0, lastIndex = 0, curIndex = 0;
	int buttonIndexArray[] = { 0,3,4,7,8,16,1,2,5,6,9,10,11,12,13,14,15 };
	chessWindow* curWindow = chessWindowsArray[SETTINGS_WINDOW_INDEX];
	lastIndex = (g->gameMode == 1) ? 17 : 6;
	for (i = 0; i < lastIndex; i++) {
		curIndex = buttonIndexArray[i];
		curWindow->buttons[curIndex]->handleEvent(curWindow->buttons[curIndex], e);
	}
}

void saveOrLoadSendEventToButtons(SDL_Event* e, int windowIndex) {
	int i = 0, lastIndex = 0;
	chessWindow* curWindow = chessWindowsArray[windowIndex];
	lastIndex = (curFirstSlotOnScreen + NUM_OF_SCREEN_SLOTS) < NUM_OF_SAVE_SLOTS ? (curFirstSlotOnScreen + NUM_OF_SCREEN_SLOTS) : NUM_OF_SAVE_SLOTS;
	curWindow->buttons[0]->handleEvent(curWindow->buttons[0], e);
	curWindow->buttons[1]->handleEvent(curWindow->buttons[1], e);
	curWindow->buttons[2]->handleEvent(curWindow->buttons[2], e);
	for (i = curFirstSlotOnScreen + 3; i < lastIndex + 3; i++)
		curWindow->buttons[i]->handleEvent(curWindow->buttons[i], e);
}

void mainSendEventToButtons(SDL_Event* e) {
	Widget* curButton;
	int i = 0;
	chessWindow* curWindow = chessWindowsArray[MAIN_WINDOW_INDEX];
	while ((curButton = curWindow->buttons[i]) != NULL) {
		curButton->handleEvent(curButton, e);
		i++;
	}
}

void sendEventToButtons(SDL_Event* e, int windowIndex) {
	switch (windowIndex) {
	case MAIN_WINDOW_INDEX:
		mainSendEventToButtons(e);
		break;
	case BOARD_WINDOW_INDEX:
		boardSendEventToButtons(e);
		break;
	case LOAD_WINDOW_INDEX:
	case SAVE_WINDOW_INDEX:
		saveOrLoadSendEventToButtons(e,windowIndex);
		break;
	case SETTINGS_WINDOW_INDEX:
		settingsSendEventToButtons(e);
		break;
	}
}

//to be changed
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

void boardDrawWindowButtons() {
	int i = 0;
	chessWindow* curWindow = chessWindowsArray[BOARD_WINDOW_INDEX];
	for (i = 0; i < 6; i++)
		curWindow->buttons[i]->draw(curWindow->buttons[i], curWindow->renderer);
	if (curGameSaved)
		curWindow->buttons[6]->draw(curWindow->buttons[6], curWindow->renderer);
	else
		curWindow->buttons[7]->draw(curWindow->buttons[7], curWindow->renderer);
	for (i = 8; i < 11; i++)
		curWindow->buttons[i]->draw(curWindow->buttons[i], curWindow->renderer);
	for (i = 11; i < 43; i++)
		if (shouldDrawPiece[i - 11] == 1)
			curWindow->buttons[i]->draw(curWindow->buttons[i], curWindow->renderer);
	for (i = 43; i < 299; i++)
		if (getMovesShouldDraw[i - 43] == 1)
			curWindow->buttons[i]->draw(curWindow->buttons[i], curWindow->renderer);
	for (i = 299; i < 363; i++)
		if (legalMovesForPieceShouldDraw[i - 299] == 1)
			curWindow->buttons[i]->draw(curWindow->buttons[i], curWindow->renderer);
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

void settingsDrawWindowButtons() {
	int i = 0, lastIndex = 0,curIndex=0;
	int buttonIndexArray[] = { 0,3,4,7,8,16,1,2,5,6,9,10,11,12,13,14,15 };
	chessWindow* curWindow = chessWindowsArray[SETTINGS_WINDOW_INDEX];
	lastIndex = (g->gameMode==1) ? 17 : 6;
	for (i = 0; i < lastIndex; i++) {
		curIndex = buttonIndexArray[i];
		curWindow->buttons[curIndex]->draw(curWindow->buttons[curIndex], curWindow->renderer);
	}
}

void drawWindowButtons(int windowIndex) {
	switch (windowIndex) {
	case MAIN_WINDOW_INDEX:
		drawAllWindowButtons(windowIndex);
		break;
	case BOARD_WINDOW_INDEX:
		boardDrawWindowButtons();
		break;
	case LOAD_WINDOW_INDEX:
	case SAVE_WINDOW_INDEX:
		saveOrLoadDrawWindowsButton(windowIndex);
		break;
	case SETTINGS_WINDOW_INDEX:
		settingsDrawWindowButtons();
		break;
	}
}

void newGameButtonClick(void) {
	curScreen = SETTINGS_WINDOW_INDEX;
}

void loadGameButtonClick(void) {//subject to renaming
	resetGetMovesShouldDrawArray();
	resetLegalMovesShouldDrawArray();
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
	resetGetMovesShouldDrawArray();
	resetLegalMovesShouldDrawArray();
	shouldRenderSameScreenAgain = true;
	if (!curGameSaved && !isCheckmateOrDraw()) {
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
	if ((chessWindowsArray[MAIN_WINDOW_INDEX] = createChessWindow(mainWindow, rend))==NULL) {
		free(mainWindow);
		free(rend);
		return NULL;
	}
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
		((Button*)(chessWindowsArray[SETTINGS_WINDOW_INDEX]->buttons[1]->data))->location.x = 240;
	else if (g->difficulty == 2)
		((Button*)(chessWindowsArray[SETTINGS_WINDOW_INDEX]->buttons[1]->data))->location.x = 350;
	else if (g->difficulty == 3)
		((Button*)(chessWindowsArray[SETTINGS_WINDOW_INDEX]->buttons[1]->data))->location.x = 460;
	else if (g->difficulty == 4)
		((Button*)(chessWindowsArray[SETTINGS_WINDOW_INDEX]->buttons[1]->data))->location.x = 570;
	else
		((Button*)(chessWindowsArray[SETTINGS_WINDOW_INDEX]->buttons[1]->data))->location.x = 680;
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
	if (g->gameMode == 1 && ((g->userColor==0 && g->currentPlayer==WHITE_PLAYER)|| (g->userColor == 1 && g->currentPlayer == BLACK_PLAYER))) {
		computerTurn(g);
		updatePiecesRectsAccordingToBoard();
		showCheckCheckmateOrDrawMessage();
	}
}

void moveUserColorChoiceLine() {
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
	moveUserColorChoiceLine();
	shouldRenderSameScreenAgain = true;
}

void backButtonClick() {
	if (curScreen == SETTINGS_WINDOW_INDEX) {
		chessGameDefault(g);
		updatePiecesRectsAccordingToBoard();
		moveDiffLevelChoiceLine();
		moveGameModeChoiceLine();
		moveUserColorChoiceLine();
		curScreen = MAIN_WINDOW_INDEX;
	}
	else
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
		resetGetMovesShouldDrawArray();
		resetLegalMovesShouldDrawArray();
		updatePiecesRectsAccordingToBoard();
	}
}

void restartButtonClick() {
	chessGameReset(g);
	resetGetMovesShouldDrawArray();
	resetLegalMovesShouldDrawArray();
	updatePiecesRectsAccordingToBoard();
	shouldRenderSameScreenAgain = true;
}

void saveSlotButtonClick() {
	char savedGamePath[50];
	int i = 0;
	for (i = 0; i < 50; i++)
		savedGamePath[i] = '\0';
	sprintf(savedGamePath, "savedGames/%d.txt", calcClickedSlotNumber());
	if (ChessGameSave(g, savedGamePath) == SUCCESS) {
		curGameSaved = true;
		if (shouldQuit)
			destroySDL();
		else if (shouldReturnToMainMenu) {
			chessGameDefault(g);
			updatePiecesRectsAccordingToBoard();
			curScreen = MAIN_WINDOW_INDEX;
			shouldReturnToMainMenu = false;
		}	
	}
	SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "Save success", "game saved!", NULL);
}

void saveButtonClick() {
	if (!isCheckmateOrDraw()) {
		resetGetMovesShouldDrawArray();
		resetLegalMovesShouldDrawArray();
		curScreen = SAVE_WINDOW_INDEX;
	}
		
}

int calculatePositionOnBoardByPoint(int x, int y) {
	return ((y - 45) / 65) * 10 + ((x - 140) / 65);
}

void undoButtonClick() {
	resetGetMovesShouldDrawArray();
	resetLegalMovesShouldDrawArray();
	shouldRenderSameScreenAgain = true;
	if (g->checkmated == '\0' && !g->draw) {
		if (ChessGameUndoPrevMove(g) == SUCCESS)
			curGameSaved = false;
		ChessGameUndoPrevMove(g);
		updatePiecesRectsAccordingToBoard();
	}
}

void showCheckCheckmateOrDrawMessage() {
	if (g->checkmated == WHITE_PLAYER)
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "Checkmate", "Checkmate! black wins", NULL);
	else if (g->checkmated == BLACK_PLAYER)
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "Checkmate", "Checkmate! white wins", NULL);
	else if (g->draw)
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "Draw", "The game ends in a draw", NULL);
	else if (g->checked == WHITE_PLAYER)
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "Check", "white king is threatened", NULL);
	else if (g->checked == BLACK_PLAYER)
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "Check", "black king is threatened", NULL);
}

void legalMoveButtonClick() {
	int srcRow, srcCol, lastClicked, destRow, destCol;
	char rowChar, colChar;
	lastClicked = calculatePositionOnBoardByPoint(lastClickPoint.x, lastClickPoint.y);
	srcRow = movingPieceLocation / 10;
	srcCol = movingPieceLocation % 10;
	destRow = lastClicked / 10;
	destCol = lastClicked % 10;
	ChessGameSetMove(g, '8' - srcRow, 'A' + srcCol, '8' - destRow, 'A' + destCol);
	resetLegalMovesShouldDrawArray();
	curGameSaved = false;
	shouldRenderSameScreenAgain = true;
	updatePiecesRectsAccordingToBoard();
	showCheckCheckmateOrDrawMessage();
	if (g->gameMode == 1 && !isCheckmateOrDraw()) {
		computerTurn(g);
		updatePiecesRectsAccordingToBoard();
		showCheckCheckmateOrDrawMessage();
	}
}

void mainMenuButtonClick() {
	int chosenButtonID = 0;
	resetGetMovesShouldDrawArray();
	resetLegalMovesShouldDrawArray();
	if (!curGameSaved && !isCheckmateOrDraw()) {
		chosenButtonID = unsavedGameBeforeLeaving();
		if (chosenButtonID == 0) {
			curScreen = MAIN_WINDOW_INDEX;
			chessGameDefault(g);
			updatePiecesRectsAccordingToBoard();
		}
			
		else if (chosenButtonID == 1) {
			shouldReturnToMainMenu = true;
			curScreen = SAVE_WINDOW_INDEX;
		}
		//else if (buttonid == -1 || buttonid == 2) {closed window or canceled}
	}
	else {
		curScreen = MAIN_WINDOW_INDEX;
		chessGameDefault(g);
		updatePiecesRectsAccordingToBoard();
	}
		
}

void updateGetMovesShouldDrawArray(int row, int col) {
	int i = 0, j = 0, curRectIndex=0, modifier=0;
	char threateningPlayerColor = isupper(g->gameBoard[row][col]) ? WHITE_PLAYER : BLACK_PLAYER;
	resetGetMovesShouldDrawArray();
	for (i = 0; i < 8; i++) {
		for (j = 0; j < 8; j++) {
			if (isLegalMove(g, row, col, i, j) && !isKingCheckedAfterMove(g, row, col, i, j)) {
				modifier = 0;
				if (isThreatenedAfterMove(g, row, col, i, j))
					modifier += 1;
				if (isOppositeColorsSquares(g, i, j, row, col))
					modifier += 2;
				curRectIndex = (8 * i + j) * 4 + modifier;
				getMovesShouldDraw[curRectIndex] = 1;
			}
		}
	}
}

void updateLegalMovesShouldDrawArray(int row, int col) {
	int i = 0, j = 0, curRectIndex = 0, modifier = 0;
	char threateningPlayerColor = isupper(g->gameBoard[row][col]) ? WHITE_PLAYER : BLACK_PLAYER;
	resetLegalMovesShouldDrawArray();
	for (i = 0; i < 8; i++) {
		for (j = 0; j < 8; j++) {
			if (isLegalMove(g, row, col, i, j) && !isKingCheckedAfterMove(g, row, col, i, j)) {
				legalMovesForPieceShouldDraw[8*i+j] = 1;
			}
		}
	}
}

void chessPieceClick() {
	int clickPointSum = 0, row = 0, col = 0;
	clickPointSum = calculatePositionOnBoardByPoint(lastClickPoint.x, lastClickPoint.y);
	row = clickPointSum / 10;
	col = clickPointSum % 10;
	resetGetMovesShouldDrawArray();
	if (curEvent->button.button == SDL_BUTTON_RIGHT && !isCheckmateOrDraw()) {
		resetLegalMovesShouldDrawArray();
		updateGetMovesShouldDrawArray(row, col);
		shouldRenderSameScreenAgain = true;
	}
	else if (curEvent->button.button == SDL_BUTTON_LEFT && ((g->currentPlayer==WHITE_PLAYER && !isupper(g->gameBoard[row][col]))|| (g->currentPlayer == BLACK_PLAYER && isupper(g->gameBoard[row][col])))) {
		movingPieceLocation = 10 * row + col;
		updateLegalMovesShouldDrawArray(row, col);
		shouldRenderSameScreenAgain = true;
	}
}

void createBoardLegalMoveRectsAndButtons() { 
	SDL_Rect boardSquareRects[64], curRect = { .x = 140,.y = 45,.w = 65,.h = 65 };
	Widget* curButton;
	SDL_Renderer* rend = chessWindowsArray[BOARD_WINDOW_INDEX]->renderer;
	int i = 0, row = 0, column = 0, curRectIndex, curButtonIndex;
	for (row = 0; row < 8; row++) {
		for (column = 0; column < 8; column++) {
			curRectIndex = (8 * row + column);
			curRect.y = 45 + 65 * row;
			curRect.x = 140 + 65 * column;
			boardSquareRects[curRectIndex] = curRect;
		}
	}
	for (row = 0; row < 8; row++) {
		for (column = 0; column < 8; column++) {
			curRectIndex = (8 * row + column);
			curButtonIndex = curRectIndex + 299;
			chessWindowsArray[BOARD_WINDOW_INDEX]->buttons[curButtonIndex] = createButton(rend, "assets/SquareBG_BoardWindow_moveSquare.bmp", boardSquareRects[curRectIndex], legalMoveButtonClick);
		}
	}
}

void createBoardSquaresRectsAndButtons() { //finish after pieces
	SDL_Rect boardSquareRects[256], curRect= { .x = 140,.y = 45,.w = 65,.h = 65 };
	Widget* curButton;
	SDL_Renderer* rend = chessWindowsArray[BOARD_WINDOW_INDEX]->renderer;
	int i = 0, row = 0, column = 0, curRectIndex, curButtonIndex;
	for (row = 0; row < 8; row++) {
		for (column = 0; column < 8; column++) {
			for (i = 0; i < 4; i++) {
				curRectIndex = (8 * row + column)*4 + i;
				curRect.y = 45 + 65 * row;
				curRect.x = 140 + 65 * column;
				boardSquareRects[curRectIndex] = curRect;
			}
		}
	}
	for (row = 0; row < 8; row++) {
		for (column = 0; column < 8; column++) {
			for (i = 0; i < 4; i++) {
				curRectIndex = (8 * row + column)*4 + i;
				curButtonIndex = curRectIndex + 43;
				switch (i) //i changes the path of the picture
				{
				case 0:
					chessWindowsArray[BOARD_WINDOW_INDEX]->buttons[curButtonIndex] = createButton(rend, "assets/SquareBG_BoardWindow_standardSquare.bmp", boardSquareRects[curRectIndex], buttonThatDoesNothing);
					break;
				case 1:
					chessWindowsArray[BOARD_WINDOW_INDEX]->buttons[curButtonIndex] = createButton(rend, "assets/SquareBG_BoardWindow_threatenedSquare.bmp", boardSquareRects[curRectIndex], buttonThatDoesNothing);
					break;
				case 2:
					chessWindowsArray[BOARD_WINDOW_INDEX]->buttons[curButtonIndex] = createButton(rend, "assets/SquareBG_BoardWindow_captureSquare.bmp", boardSquareRects[curRectIndex], buttonThatDoesNothing);
					break;
				case 3:
					chessWindowsArray[BOARD_WINDOW_INDEX]->buttons[curButtonIndex] = createButton(rend, "assets/SquareBG_BoardWindow_captureAndThreatenedSquare.bmp", boardSquareRects[curRectIndex], buttonThatDoesNothing);
					break;
				}
			}
		}
	}
}

SDL_Point calculatePointOfBoardPosition(int row, int col) {
	SDL_Point res;
	res.x = 140 + col * 65;
	res.y = 45 + row * 65;
	return res;
}

void updatePiecesRectsAccordingToBoard() {
	SDL_Point curPoint = { .x = 0,.y = 0 };
	chessWindow* boardWindow = chessWindowsArray[BOARD_WINDOW_INDEX];
	int i = 0, j = 0;
	int whiteRookCurIndex = 11, whiteKnightCurIndex = 12, whiteBishopCurIndex = 13, whiteQueenCurIndex = 14, whiteKingCurIndex = 15, whitePawnCurIndex = 19;
	int blackRookCurIndex = 27, blackKnightCurIndex = 28, blackBishopCurIndex = 29, blackQueenCurIndex = 30, blackKingCurIndex = 31, blackPawnCurIndex = 35;
	for (i = 0; i < 32; i++)
		shouldDrawPiece[i] = 0;
	for (i = 0; i < 8; i++) {
		for (j = 0; j < 8; j++) {
			if (g->gameBoard[i][j] != '\0') {
				curPoint = calculatePointOfBoardPosition(i, j);
				switch (g->gameBoard[i][j]) {
				case WHITE_ROOK:
					((Button*)boardWindow->buttons[whiteRookCurIndex]->data)->location.x = curPoint.x;
					((Button*)boardWindow->buttons[whiteRookCurIndex]->data)->location.y = curPoint.y;
					shouldDrawPiece[whiteRookCurIndex - 11] = 1;
					whiteRookCurIndex = 18;
					break;
				case WHITE_KNIGHT:
					((Button*)boardWindow->buttons[whiteKnightCurIndex]->data)->location.x = curPoint.x;
					((Button*)boardWindow->buttons[whiteKnightCurIndex]->data)->location.y = curPoint.y;
					shouldDrawPiece[whiteKnightCurIndex - 11] = 1;
					whiteKnightCurIndex = 17;
					break;
				case WHITE_BISHOP:
					((Button*)boardWindow->buttons[whiteBishopCurIndex]->data)->location.x = curPoint.x;
					((Button*)boardWindow->buttons[whiteBishopCurIndex]->data)->location.y = curPoint.y;
					shouldDrawPiece[whiteBishopCurIndex - 11] = 1;
					whiteBishopCurIndex = 16;
					break;
				case WHITE_QUEEN:
					((Button*)boardWindow->buttons[whiteQueenCurIndex]->data)->location.x = curPoint.x;
					((Button*)boardWindow->buttons[whiteQueenCurIndex]->data)->location.y = curPoint.y;
					shouldDrawPiece[whiteQueenCurIndex - 11] = 1;
					break;
				case WHITE_KING:
					((Button*)boardWindow->buttons[whiteKingCurIndex]->data)->location.x = curPoint.x;
					((Button*)boardWindow->buttons[whiteKingCurIndex]->data)->location.y = curPoint.y;
					shouldDrawPiece[whiteKingCurIndex - 11] = 1;
					break;
				case WHITE_PAWN:
					((Button*)boardWindow->buttons[whitePawnCurIndex]->data)->location.x = curPoint.x;
					((Button*)boardWindow->buttons[whitePawnCurIndex]->data)->location.y = curPoint.y;
					shouldDrawPiece[whitePawnCurIndex - 11] = 1;
					whitePawnCurIndex++;
					break;
				case BLACK_ROOK:
					((Button*)boardWindow->buttons[blackRookCurIndex]->data)->location.x = curPoint.x;
					((Button*)boardWindow->buttons[blackRookCurIndex]->data)->location.y = curPoint.y;
					shouldDrawPiece[blackRookCurIndex - 11] = 1;
					blackRookCurIndex = 34;
					break;
				case BLACK_KNIGHT:
					((Button*)boardWindow->buttons[blackKnightCurIndex]->data)->location.x = curPoint.x;
					((Button*)boardWindow->buttons[blackKnightCurIndex]->data)->location.y = curPoint.y;
					shouldDrawPiece[blackKnightCurIndex - 11] = 1;
					blackKnightCurIndex = 33;
					break;
				case BLACK_BISHOP:
					((Button*)boardWindow->buttons[blackBishopCurIndex]->data)->location.x = curPoint.x;
					((Button*)boardWindow->buttons[blackBishopCurIndex]->data)->location.y = curPoint.y;
					shouldDrawPiece[blackBishopCurIndex - 11] = 1;
					blackBishopCurIndex = 32;
					break;
				case BLACK_QUEEN:
					((Button*)boardWindow->buttons[blackQueenCurIndex]->data)->location.x = curPoint.x;
					((Button*)boardWindow->buttons[blackQueenCurIndex]->data)->location.y = curPoint.y;
					shouldDrawPiece[blackQueenCurIndex - 11] = 1;
					break;
				case BLACK_KING:
					((Button*)boardWindow->buttons[blackKingCurIndex]->data)->location.x = curPoint.x;
					((Button*)boardWindow->buttons[blackKingCurIndex]->data)->location.y = curPoint.y;
					shouldDrawPiece[blackKingCurIndex - 11] = 1;
					break;
				case BLACK_PAWN:
					((Button*)boardWindow->buttons[blackPawnCurIndex]->data)->location.x = curPoint.x;
					((Button*)boardWindow->buttons[blackPawnCurIndex]->data)->location.y = curPoint.y;
					shouldDrawPiece[blackPawnCurIndex - 11] = 1;
					blackPawnCurIndex++;
					break;
				}

			}
		}
	}
}

void createPiecesRectsAndButtons() { //currently with functions that do nothing
	SDL_Rect boardPiecesRects[32], curRect = { .x = 0,.y = 0,.w = 65,.h = 65 };
	SDL_Renderer* rend = chessWindowsArray[BOARD_WINDOW_INDEX]->renderer;
	chessWindow* boardWindow = chessWindowsArray[BOARD_WINDOW_INDEX];
	Widget* curWidget;
	int whiteRookCurIndex = 11, whiteKnightCurIndex = 12, whiteBishopCurIndex = 13, whiteQueenCurIndex = 14, whiteKingCurIndex = 15, whitePawnCurIndex = 19;
	int blackRookCurIndex = 27, blackKnightCurIndex = 28, blackBishopCurIndex = 29, blackQueenCurIndex = 30, blackKingCurIndex = 31, blackPawnCurIndex = 35;
	int i = 0;
	for (i = 0; i < 8; i++) {
		boardWindow->buttons[whitePawnCurIndex] = createButton(rend, "assets/pieces/whitePawn.bmp", curRect, chessPieceClick);
		boardWindow->buttons[blackPawnCurIndex] = createButton(rend, "assets/pieces/blackPawn.bmp", curRect, chessPieceClick);
		whitePawnCurIndex++;
		blackPawnCurIndex++;
	}
	for (i = 0; i < 2; i++) {
		curWidget = createButton(rend, "assets/pieces/whiteRook.bmp", curRect, chessPieceClick);
		boardWindow->buttons[whiteRookCurIndex] = curWidget;
		//boardWindow->buttons[whiteRookCurIndex] = createButton(rend, "assets/pieces/whiteRook.bmp", curRect, chessPieceClick);
		boardWindow->buttons[blackRookCurIndex] = createButton(rend, "assets/pieces/blackRook.bmp", curRect, chessPieceClick);
		whiteRookCurIndex = 18;
		blackRookCurIndex = 34;
		boardWindow->buttons[whiteKnightCurIndex] = createButton(rend, "assets/pieces/whiteKnight.bmp", curRect, chessPieceClick);
		boardWindow->buttons[blackKnightCurIndex] = createButton(rend, "assets/pieces/blackKnight.bmp", curRect, chessPieceClick);
		whiteKnightCurIndex = 17;
		blackKnightCurIndex = 33;
		boardWindow->buttons[whiteBishopCurIndex] = createButton(rend, "assets/pieces/whiteBishop.bmp", curRect, chessPieceClick);
		boardWindow->buttons[blackBishopCurIndex] = createButton(rend, "assets/pieces/blackBishop.bmp", curRect, chessPieceClick);
		whiteBishopCurIndex = 16;
		blackBishopCurIndex = 32;
	}
	boardWindow->buttons[whiteQueenCurIndex] = createButton(rend, "assets/pieces/whiteQueen.bmp", curRect, chessPieceClick);
	boardWindow->buttons[blackQueenCurIndex] = createButton(rend, "assets/pieces/blackQueen.bmp", curRect, chessPieceClick);
	boardWindow->buttons[whiteKingCurIndex] = createButton(rend, "assets/pieces/whiteKing.bmp", curRect, chessPieceClick);
	boardWindow->buttons[blackKingCurIndex] = createButton(rend, "assets/pieces/blackKing.bmp", curRect, chessPieceClick);
}


chessWindow* createBoardWindow() {
	int i = 0;
	SDL_Window* boardWindow = SDL_CreateWindow("Board Window", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_SHOWN);
	if (boardWindow == NULL)
		return NULL;
	SDL_Renderer* rend = SDL_CreateRenderer(boardWindow, -1, SDL_RENDERER_SOFTWARE);
	if (rend == NULL) {
		free(boardWindow);
		return NULL;
	}
	if (SDL_SetRenderDrawBlendMode(rend, SDL_BLENDMODE_BLEND) < 0) {
		free(boardWindow);
		free(rend);
		return NULL;
	}
		
	if ((chessWindowsArray[BOARD_WINDOW_INDEX] = createChessWindow(boardWindow, rend)) == NULL) {
		free(boardWindow);
		free(rend);
		return NULL;
	}
	//upper menu button rects - done
	SDL_Rect restartButtonRect = { .x = 20,.y = 10,.w = 100,.h = 25 };
	SDL_Rect saveButtonRect = { .x = 130,.y = 10,.w = 100,.h = 25 };
	SDL_Rect loadButtonRect = { .x = 240,.y = 10,.w = 100,.h = 25 };
	SDL_Rect undoButtonRect = { .x = 350,.y = 10,.w = 100,.h = 25 };
	SDL_Rect mainMenuButtonRect = { .x = 460,.y = 10,.w = 100,.h = 25 };
	SDL_Rect quitButtonRect = { .x = 570,.y = 10,.w = 100,.h = 25 };
	//saved or unsaved game rects - done
	SDL_Rect gameIsSavedRect = { .x = 680,.y = 10,.w = 100,.h = 25 };
	SDL_Rect gameIsUnsavedRect = { .x = 680,.y = 10,.w = 100,.h = 25 };
	//board rect - done
	SDL_Rect gameboardRect = { .x = 140,.y = 45,.w = 520,.h = 520 };
	//numbers column and letters row
	SDL_Rect lettersRowRect = { .x = 140,.y = 565,.w = 520,.h = 25 };
	SDL_Rect numbersColumnRect = { .x = 115,.y = 45,.w = 25,.h = 520 }; //waiting for Ram's approval
	chessWindowsArray[BOARD_WINDOW_INDEX]->buttons[0] = createButton(rend, "assets/boardWindow_restartButton.bmp", restartButtonRect, restartButtonClick);
	chessWindowsArray[BOARD_WINDOW_INDEX]->buttons[1] = createButton(rend, "assets/boardWindow_saveButton.bmp", saveButtonRect, saveButtonClick);
	chessWindowsArray[BOARD_WINDOW_INDEX]->buttons[2] = createButton(rend, "assets/boardWindow_loadButton.bmp", loadButtonRect, loadGameButtonClick);
	chessWindowsArray[BOARD_WINDOW_INDEX]->buttons[3] = createButton(rend, "assets/boardWindow_undoButton.bmp", undoButtonRect, undoButtonClick);
	chessWindowsArray[BOARD_WINDOW_INDEX]->buttons[4] = createButton(rend, "assets/boardWindow_mainMenuButton.bmp", mainMenuButtonRect, mainMenuButtonClick);
	chessWindowsArray[BOARD_WINDOW_INDEX]->buttons[5] = createButton(rend, "assets/boardWindow_quitButton.bmp", quitButtonRect, quitGameButtonClick);
	chessWindowsArray[BOARD_WINDOW_INDEX]->buttons[6] = createButton(rend, "assets/boardWindow_gameSaved.bmp", gameIsSavedRect, buttonThatDoesNothing);
	chessWindowsArray[BOARD_WINDOW_INDEX]->buttons[7] = createButton(rend, "assets/boardWindow_gameNotSaved.bmp", gameIsUnsavedRect, buttonThatDoesNothing);
	chessWindowsArray[BOARD_WINDOW_INDEX]->buttons[8] = createButton(rend, "assets/boardWindow_board.bmp", gameboardRect, buttonThatDoesNothing);
	chessWindowsArray[BOARD_WINDOW_INDEX]->buttons[9] = createButton(rend, "assets/boardWindow_boardBottomLegend.bmp", lettersRowRect, buttonThatDoesNothing);
	chessWindowsArray[BOARD_WINDOW_INDEX]->buttons[10] = createButton(rend, "assets/boardWindow_boardLeftLegend.bmp", numbersColumnRect, buttonThatDoesNothing);

	//pieces buttons
	createPiecesRectsAndButtons();
	updatePiecesRectsAccordingToBoard();
	//square colors rects
	createBoardSquaresRectsAndButtons();

	createBoardLegalMoveRectsAndButtons();

	for (i = 0; i < 363; i++) {
		if (chessWindowsArray[BOARD_WINDOW_INDEX]->buttons[i] == NULL)
			return NULL;
	}
	SDL_HideWindow(chessWindowsArray[BOARD_WINDOW_INDEX]->window);
	return chessWindowsArray[BOARD_WINDOW_INDEX];
}

chessWindow* createSaveWindow() {
	SDL_Rect slotsRects[5];
	char curSlotImagePath[50];
	int i = 0, curPos = 0;
	for (i = 0; i < 50; i++)
		curSlotImagePath[i] = '\0';
	SDL_Window* saveWindow = SDL_CreateWindow("Save Window", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_SHOWN);
	if (saveWindow == NULL)
		return NULL;
	SDL_Renderer* rend = SDL_CreateRenderer(saveWindow, -1, SDL_RENDERER_SOFTWARE);
	if (rend == NULL) {
		free(saveWindow);
		return NULL;
	}
	if ((chessWindowsArray[SAVE_WINDOW_INDEX] = createChessWindow(saveWindow, rend)) == NULL) {
		free(saveWindow);
		free(rend);
		return NULL;
	}
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
	chessWindowsArray[SAVE_WINDOW_INDEX]->buttons[0] = leftArrowButton;
	chessWindowsArray[SAVE_WINDOW_INDEX]->buttons[1] = rightArrowButton;
	chessWindowsArray[SAVE_WINDOW_INDEX]->buttons[2] = backButton;
	for (i = 3; i < 3 + NUM_OF_SAVE_SLOTS; i++) {
		curPos = (i - 3) % NUM_OF_SCREEN_SLOTS;
		sprintf(curSlotImagePath, "assets/load_saveWindowSlot%d.bmp", i - 2);
		chessWindowsArray[SAVE_WINDOW_INDEX]->buttons[i] = createButton(rend, curSlotImagePath, slotsRects[curPos], saveSlotButtonClick);
	}
	for (i = 0; i < 3 + NUM_OF_SAVE_SLOTS; i++) {
		if (chessWindowsArray[SAVE_WINDOW_INDEX]->buttons[i] == NULL)
			return NULL;
	}
	SDL_HideWindow(chessWindowsArray[SAVE_WINDOW_INDEX]->window);
	return chessWindowsArray[SAVE_WINDOW_INDEX];
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
	if ((chessWindowsArray[LOAD_WINDOW_INDEX] = createChessWindow(loadWindow, rend))==NULL) {
		free(loadWindow);
		free(rend);
		return NULL;
	}
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
	int i = 0;
	SDL_Window* settingsWindow = SDL_CreateWindow("Settings Window", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_SHOWN);
	if (settingsWindow == NULL)
		return NULL;
	SDL_Renderer* rend = SDL_CreateRenderer(settingsWindow, -1, SDL_RENDERER_SOFTWARE);
	if (rend == NULL) {
		free(settingsWindow);
		return NULL;
	}
	if((chessWindowsArray[SETTINGS_WINDOW_INDEX] = createChessWindow(settingsWindow, rend))==NULL) {
		free(settingsWindow);
		free(rend);
		return NULL;
	}
	
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
	chessWindowsArray[SETTINGS_WINDOW_INDEX]->buttons[0] = createButton(rend, "assets/settingsWindow_choiceMark.bmp", gameModeChoiceRect, buttonThatDoesNothing);
	chessWindowsArray[SETTINGS_WINDOW_INDEX]->buttons[1] = createButton(rend, "assets/settingsWindow_choiceMark.bmp", diffLevelChoiceRect, buttonThatDoesNothing);
	chessWindowsArray[SETTINGS_WINDOW_INDEX]->buttons[2] = createButton(rend, "assets/settingsWindow_choiceMark.bmp", userColorChoiceRect, buttonThatDoesNothing);


	//back button-done
	chessWindowsArray[SETTINGS_WINDOW_INDEX]->buttons[3] = createButton(rend, "assets/backButton.bmp", backButtonRect, backButtonClick);

	//left side buttons
	chessWindowsArray[SETTINGS_WINDOW_INDEX]->buttons[4] = createButton(rend, "assets/settingsWindow_gameMode.bmp", gameModeRect, buttonThatDoesNothing);
	chessWindowsArray[SETTINGS_WINDOW_INDEX]->buttons[5] = createButton(rend, "assets/settingsWindow_diffLevel.bmp", diffLevelRect, buttonThatDoesNothing);
	chessWindowsArray[SETTINGS_WINDOW_INDEX]->buttons[6] = createButton(rend, "assets/settingsWindow_userColor.bmp", userColorRect, buttonThatDoesNothing);

	//game mode buttons
	chessWindowsArray[SETTINGS_WINDOW_INDEX]->buttons[7] = createButton(rend, "assets/settingsWindow_1player.bmp", onePlayerButtonRect, gameModeButtonClick);
	chessWindowsArray[SETTINGS_WINDOW_INDEX]->buttons[8] = createButton(rend, "assets/settingsWindow_2player.bmp", twoPlayersButtonRect, gameModeButtonClick);

	//diff level buttons
	chessWindowsArray[SETTINGS_WINDOW_INDEX]->buttons[9] = createButton(rend, "assets/settingsWindow_amateur.bmp", diffAmateurButtonRect, diffLevelButtonClick);
	chessWindowsArray[SETTINGS_WINDOW_INDEX]->buttons[10] = createButton(rend, "assets/settingsWindow_easy.bmp", diffeasyButtonRect, diffLevelButtonClick);
	chessWindowsArray[SETTINGS_WINDOW_INDEX]->buttons[11] = createButton(rend, "assets/settingsWindow_moderate.bmp", diffmoderateButtonRect, diffLevelButtonClick);
	chessWindowsArray[SETTINGS_WINDOW_INDEX]->buttons[12] = createButton(rend, "assets/settingsWindow_hard.bmp", diffhardButtonRect, diffLevelButtonClick);
	chessWindowsArray[SETTINGS_WINDOW_INDEX]->buttons[13] = createButton(rend, "assets/settingsWindow_expert.bmp", diffExpertButtonRect, diffLevelButtonClick);
	
	//user color buttons
	chessWindowsArray[SETTINGS_WINDOW_INDEX]->buttons[14] = createButton(rend, "assets/settingsWindow_white.bmp", whiteButtonRect, userColorButtonClick);
	chessWindowsArray[SETTINGS_WINDOW_INDEX]->buttons[15] = createButton(rend, "assets/settingsWindow_black.bmp", blackButtonRect, userColorButtonClick);

	//start button
	chessWindowsArray[SETTINGS_WINDOW_INDEX]->buttons[16] = createButton(rend, "assets/settingsWindow_start.bmp", startButtonRect,startButtonClick);


	for (i = 0; i < 17; i++) {
		if (chessWindowsArray[SETTINGS_WINDOW_INDEX]->buttons[i] == NULL)
			return NULL;
	}
	SDL_HideWindow(chessWindowsArray[SETTINGS_WINDOW_INDEX]->window);
	return chessWindowsArray[SETTINGS_WINDOW_INDEX];
}

bool initializeAllWindows() {
	if (createMainWindow() == NULL || createLoadWindow()==NULL|| createSettingsWindow() == NULL|| createBoardWindow()==NULL ||createSaveWindow()==NULL)
		return false;
	return true;
}