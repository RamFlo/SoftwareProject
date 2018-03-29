#include "ChessWindow.h"
#define NUM_OF_WINDOWS 5

#define MAIN_WINDOW_INDEX 0
#define SETTINGS_WINDOW_INDEX 1
#define LOAD_WINDOW_INDEX 2
#define SAVE_WINDOW_INDEX 3
#define BOARD_WINDOW_INDEX 4
extern chessWindow* chessWindowsArray[NUM_OF_WINDOWS];
int curScreen;
bool initializeAllWindows();
