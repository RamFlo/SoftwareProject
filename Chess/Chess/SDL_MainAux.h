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
extern chessWindow* chessWindowsArray[NUM_OF_WINDOWS];
extern int shouldDrawPiece[32];
extern int getMovesShouldDraw[256];
extern int legalMovesForPieceShouldDraw[64];
extern int curScreen;
extern int curFirstSlotOnScreen;
extern int previousScreen;
extern int movingPieceLocation;
extern bool curGameSaved;
extern bool shouldQuit;
extern bool shouldReturnToMainMenu;
extern ChessGame* g;
extern SDL_Point lastClickPoint;
extern bool shouldRenderSameScreenAgain;
extern SDL_Event* curEvent;
bool initializeAllWindows();
void destroySDL();
void drawWindowButtons(int windowIndex);
void SwitchOrRenderScreen(int lastHandledScreen);
void sendEventToButtons(SDL_Event* e, int windowIndex);
void quitGameButtonClick();
void updatePiecesRectsAccordingToBoard();
#endif