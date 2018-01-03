#ifndef SPMAINAUX_H_
#define SPMAINAUX_H_
#define HISTORY_SIZE 20
#include "SPFIARParser.h"
#include "SPFIARGame.h"
#include <stdio.h>
#include "SPMinimax.h"

//a boolean value, indicating whether or not a memory error occurred while parsing the command
extern bool parserMemError;

/*
* Given a game state, this function prints the game's winner (or tie).
* If there's no winner (or tie), this function does nothing.
*
* @param currentGame - The current game state
*/
void printWinner(SPFiarGame* curGame);

/*
* Given a game state, this function ends the current game by destroying the game state using
* spFiarGameDestroy. If the game has ended due to a memory error, it also prints the line
* "Exiting...". Else, it only frees the memory allocated to the game.
*
* @param game - The current game state
* @param isMemError - a boolean value that indicates whether or not the game ended due to a memory error.
*/
void endGame(SPFiarGame * game, bool isMemError);

/*
* Given a const char sequence's address, this function allocates a non-const char space of the same
* size and copies the given const char sequence into the non-const char space.
*
* @param s - The address of a const char sequence to copy
* @return
* NULL if a memory allocation error occurred
* On success, the function returns a pointer to the non-const char sequence which is
* a copy of the char sequence residing at address s.
*/
char *my_strdup_main(const char *s);

/*
* Given a game state and maxDepth, this function reads a command given by the user (from stdin),
* and parses it using spParserPraseLine.
* If the command isn't legal (or if the command couldn't be done),
* a corresponding message is printed and the function calls itself in order to read a new command.
* If the command was SP_QUIT, the game is ended using endGame.
* If the command was SP_RESTART, the function returns -1.
* In case of a memory error, the function ends the game using endGame.
* In any other case, the function executes the given command and returns 0.
* @param currentGame - The current game state
* @param maxDepth - The maximum depth of the miniMax algorithm
* @return
* 0 if the command was read and executed successfully and wasn't SP_RESTART or SP_QUIT
* -1 if the command was SP_RESTART
* Undefined value if the game was ended
* doUserCommand(curGame,maxDepth) if the command isn't legal (or if the command couldn't be done)
*/
int doUserCommand(SPFiarGame* curGame, int maxDepth);

/*
* Given a game state, this function reads a command given by the user (from stdin),
* and parses it using spParserPraseLine, and returns SPCommand that was parsed.
* In case of a memory error, the function ends the game using endGame.

* @param currentGame - The current game state
* @return
* SPCommand cmd if the command was read and parsed.
* Undefined value if the game was ended due to a memory error
*/
SPCommand readCommand(SPFiarGame *curGame);

/*
* Given a game state, this function reads the maxDepth from stdin,
* and parses it using spParserPraseLine, and returns SPCommand that was parsed.
* In case of a memory error, the function ends the game using endGame.
* In case of an invalid value, the function calls itelf to read another value.

* @return
* -1 if the 'quit' command was read instead of maxDepth value
* -2 if a memory allocation error occurred
* getMaxDepth(), if an invalid maxDepth was read
* else, the value of maxDepth that was read from stdin is returned.
*/
int getMaxDepth();

/*
* Given a game state and maxDepth, this function executes the computer's turn:
* it calls spMinimaxSuggestMove in order to compute the move to be executed.
* In case of a memory error, the function ends the game using endGame.
* Else, the function executes said move and prints it.
*/
void computerTurn(SPFiarGame* curGame, int maxDepth);
#endif
