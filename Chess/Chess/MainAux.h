#ifndef MAINAUX_H_
#define MAINAUX_H_
#define HISTORY_SIZE 30
#include "ChessParser.h"
#include "ChessGame.h"
#include <stdio.h>
#include "Minimax.h"



void settingsState(ChessGame* g);

void computerTurn(ChessGame* g);

void playerTurn(ChessGame* g);

void gameState(ChessGame* g);
#endif
