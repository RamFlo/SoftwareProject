#ifndef SDL_MAINAUX_H_
#define SDL_MAINAUX_H_
#include "ChessWindow.h"
#include <SDL.h>
#include <SDL_video.h>
#include "ChessGame.h"

#define NUM_OF_WINDOWS 5
#define MAIN_WINDOW_INDEX 0
#define SETTINGS_WINDOW_INDEX 1
#define LOAD_WINDOW_INDEX 2
#define SAVE_WINDOW_INDEX 3
#define BOARD_WINDOW_INDEX 4
#define NUM_OF_SAVE_SLOTS 12
#define NUM_OF_SCREEN_SLOTS 5

//an array of the program's windows
extern chessWindow* chessWindowsArray[NUM_OF_WINDOWS];

//an array specifying whether or not a piece should be drawn on the board (0-no, 1-yes)
extern int shouldDrawPiece[32];

//an array specifying whether or not a get moves square should be drawn on the board (0-no, 1-yes)
extern int getMovesShouldDraw[256];

//an array specifying whether or not a move option should be drawn on the board (0-no, 1-yes)
extern int legalMovesForPieceShouldDraw[64];

//an integer saving the current screen's index
extern int curScreen;

//an integer saving the save or load first slot index that should be shown on screen
extern int curFirstSlotOnScreen;

//an integer saving the previous screen's index
extern int previousScreen;

//an integer saving the chosen piece's location
extern int movingPieceLocation;

//a boolean specifying whether or not the current game is saved
extern bool curGameSaved;

//a boolean specifying whether or not the user intends to quit immediately after saving
extern bool shouldQuit;

//a boolean specifying whether or not the user intends to return to main menu immediately after saving
extern bool shouldReturnToMainMenu;

//a ChessGame pointer to the current game
extern ChessGame* g;

//the last click point on the screen
extern SDL_Point lastClickPoint;

//a boolean specifying whether or not the current screen should be rendered again
extern bool shouldRenderSameScreenAgain;

//a SDL_Event pointer to the current event
extern SDL_Event* curEvent;

/**
*  Initializes all of the program's needed windows.
*  @return
*  true, if the windows all initialied successfully
*  false, otherwise
*/
bool initializeAllWindows();

/**
*  Destroys the game's program and the game itself, and invokes SDL_Quit()
*/
void destroySDL();

/**
*  Draws all of the buttons of the window according to windowIndex
*  @param windowIndex - the index of the window
*/
void drawWindowButtons(int windowIndex);

/**
*  Switches the current program's screen and renders the new screen or re-renders the current screen
*  @param lastHandledScreen - the last seen screen index
*/
void SwitchOrRenderScreen(int lastHandledScreen);

/**
*  Sends the current event pointer 'e' to all of the buttons of the window according to windowIndex
*  @param e - a pointer to an event
*  @param windowIndex - the index of the window
*/
void sendEventToButtons(SDL_Event* e, int windowIndex);

/**
*  Click action for Quit button in the main and board windows. Also called upon clicking the X button.
*  Checks for unsaved game the acts accordingly.
*/
void quitGameButtonClick();

/**
*  Calculates and updates the positions of the pieces's buttons that currently exist on the board.
*/
void updatePiecesRectsAccordingToBoard();

/**
*  Called at the end of every turn. Shows a message upon check, checkmate or draw situations
*/
void showCheckCheckmateOrDrawMessage();
#endif