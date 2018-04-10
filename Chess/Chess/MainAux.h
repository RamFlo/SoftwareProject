#ifndef MAINAUX_H_
#define MAINAUX_H_
#define HISTORY_SIZE 30
#include "ChessParser.h"
#include "ChessGame.h"
#include <stdio.h>
#include "Minimax.h"


//Starts the settings state - gets a command from the user, makes sure it's a legal command that's executable on the settings state according to the instructions,
//then executes it accordingly
void settingsState(ChessGame* g);

//Computes the best move using Minimax, executes the move and prints it
void computerTurn(ChessGame* g);

//Executes a human player's turn:
//prints the relevant messages, gets a command and executes it according to it's specified effect.
void playerTurn(ChessGame* g);

//if it's a human player's turn - executes a player turn function. Else - executes a computer turn.
void gameState(ChessGame* g);
#endif
