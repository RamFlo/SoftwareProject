#ifndef SPMAINAUX_H_
#define SPMAINAUX_H_
#define HISTORY_SIZE 20
#include "SPFIARParser.h"
#include "SPFIARGame.h"
#include <stdio.h>
#include "SPMinimax.h"
extern bool parserMemError;
void printWinner(SPFiarGame* curGame);
void endGame(SPFiarGame * game, bool isMemError);
char *my_strdup_main(const char *s);
int doUserCommand(SPFiarGame* curGame, int maxDepth);
SPCommand readCommand(SPFiarGame *curGame);
int getMaxDepth();
void computerTurn(SPFiarGame* curGame, int maxDepth);
#endif
