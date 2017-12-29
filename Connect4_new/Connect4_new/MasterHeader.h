#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include "SPFIARGame.h"

//SPFIARParser
char *my_strdup(const char *s);

//SPFIARGame
void spFiarGameSwitchPlayer(SPFiarGame* src);
bool checkColWinner(SPFiarGame* src, int lastCol, int lastRow, char lastMove);
bool checkRowWinner(SPFiarGame* src, int lastCol, int lastRow, char lastMove);
bool checkDiagUpWinner(SPFiarGame* src, int lastCol, int lastRow, char lastMove);
bool checkDiagDownWinner(SPFiarGame* src, int lastCol, int lastRow, char lastMove);
bool isGameBoardFull(SPFiarGame* src);
char getOtherPlayer(char player);