#ifndef SPFIARGAME_H_
#define SPFIARGAME_H_
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include "SPArrayList.h"
#include "SPFIARGame.h"

/**
* SPFIARGame Summary:
*
* A container that represents a classic connect-4 game, a two players 6 by 7
* board game (rows X columns). The container supports the following functions.
*
* spFiarGameCreate           - Creates a new game board
* spFiarGameCopy             - Copies a game board
* spFiarGameDestroy          - Frees all memory resources associated with a game
* spFiarGameSetMove          - Sets a move on a game board
* spFiarGameIsValidMove      - Checks if a move is valid
* spFiarGameUndoPrevMove     - Undoes previous move made by the last player
* spFiarGamePrintBoard       - Prints the current board
* spFiarGameGetCurrentPlayer - Returns the current player
*
*/

//Definitions
#define SP_FIAR_GAME_SPAN 4
#define SP_FIAR_GAME_N_ROWS 6
#define SP_FIAR_GAME_N_COLUMNS 7
#define SP_FIAR_GAME_PLAYER_1_SYMBOL 'X'
#define SP_FIAR_GAME_PLAYER_2_SYMBOL 'O'
#define SP_FIAR_GAME_TIE_SYMBOL '-'
#define SP_FIAR_GAME_EMPTY_ENTRY ' '

typedef struct sp_fiar_game_t {
	char gameBoard[SP_FIAR_GAME_N_ROWS][SP_FIAR_GAME_N_COLUMNS];
	int tops[SP_FIAR_GAME_N_COLUMNS];
	char currentPlayer;
	SPArrayList* history;
} SPFiarGame;

/**
* Type used for returning error codes from game functions
*/
typedef enum sp_fiar_game_message_t {
	SP_FIAR_GAME_INVALID_MOVE,
	SP_FIAR_GAME_INVALID_ARGUMENT,
	SP_FIAR_GAME_NO_HISTORY,
	SP_FIAR_GAME_SUCCESS,
	//You may add any message you like
} SP_FIAR_GAME_MESSAGE;


void spFiarGameSwitchPlayer(SPFiarGame* src) {
	if (src != NULL) {
		if (src->currentPlayer == SP_FIAR_GAME_PLAYER_1_SYMBOL)
			src->currentPlayer = SP_FIAR_GAME_PLAYER_2_SYMBOL;
		else
			src->currentPlayer = SP_FIAR_GAME_PLAYER_1_SYMBOL;
	}
}
/**
* Creates a new game with a specified history size. The history size is a
* parameter which specifies the number of previous moves to store. If the number
* of moves played so far exceeds this parameter, then first moves stored will
* be discarded in order for new moves to be stored.
*
* @historySize - The total number of moves to undo,
*                a player can undo at most historySizeMoves turns.
* @return
* NULL if either a memory allocation failure occurs or historySize <= 0.
* Otherwise, a new game instant is returned.
*/
SPFiarGame* spFiarGameCreate(int historySize) {
	SPFiarGame* g;
	int i = 0;
	g = (SPFiarGame *)malloc(sizeof(SPFiarGame));
	if (g==NULL)
		return NULL;
	for (i = 0; i < SP_FIAR_GAME_N_COLUMNS; i++)
		g->tops[i] = 0;
	g->currentPlayer = SP_FIAR_GAME_PLAYER_1_SYMBOL;
	g->history = spArrayListCreate(historySize);
	if (g->history == NULL)
		return NULL;
	return g;
}

/**
*	Creates a copy of a given game.
*	The new copy has the same status as the src game.
*
*	@param src - the source game which will be copied
*	@return
*	NULL if either src is NULL or a memory allocation failure occurred.
*	Otherwise, an new copy of the source game is returned.
*
*/
SPFiarGame* spFiarGameCopy(SPFiarGame* src) {
	SPFiarGame *g;
	int i = 0, j = 0, k=0;
	SPArrayList* p;
	if (src == NULL)
		return NULL;
	g = spFiarGameCreate(src->history->maxSize);
	if (g == NULL)
		return NULL;
	g->currentPlayer = src->currentPlayer;
	for (i = 0; i < SP_FIAR_GAME_N_ROWS; i++) {
		for (j = 0; j < SP_FIAR_GAME_N_COLUMNS; j++) {
			(g->gameBoard)[i][j] = (src->gameBoard)[i][j];
		}
	}
	p = spArrayListCopy(src->history);
	for (k = 0; k < SP_FIAR_GAME_N_COLUMNS; k++) {
		(g->tops)[k] = (src->tops)[k];
	}
	g->history = p;
	return g;
}

/**
* Frees all memory allocation associated with a given game. If src==NULL
* the function does nothing.
*
* @param src - the source game
*/
void spFiarGameDestroy(SPFiarGame* src) {
	if (src != NULL) {
		spArrayListDestroy(src->history);
		free(src);
	}
}

/**
* Sets the next move in a given game by specifying column index. The
* columns are 0-based and in the range [0,SP_FIAR_GAME_N_COLUMNS -1].
*
* @param src - The target game
* @param col - The target column, the columns are 0-based
* @return
* SP_FIAR_GAME_INVALID_ARGUMENT - if src is NULL or col is out-of-range
* SP_FIAR_GAME_INVALID_MOVE - if the given column is full.
* SP_FIAR_GAME_SUCCESS - otherwise
*/
SP_FIAR_GAME_MESSAGE spFiarGameSetMove(SPFiarGame* src, int col) {
	if (src == NULL || col >= SP_FIAR_GAME_N_COLUMNS || col < 0)
		return SP_FIAR_GAME_INVALID_ARGUMENT;
	if (src->tops[col] == SP_FIAR_GAME_N_ROWS)
		return SP_FIAR_GAME_INVALID_MOVE;
	src->gameBoard[src->tops[col]][col] = src->currentPlayer;
	src->tops[col]++;
	spFiarGameSwitchPlayer(src);
	if (spArrayListAddLast(src->history, col) != SP_ARRAY_LIST_SUCCESS) {
		spArrayListRemoveFirst(src->history);
		spArrayListAddLast(src->history, col);
	}
	return SP_FIAR_GAME_SUCCESS;
}

/**
* Checks if a disk can be put in the specified column.
*
* @param src - The source game
* @param col - The specified column
* @return
* true  - if the a disc can be put in the target column
* false - otherwise.
*/
bool spFiarGameIsValidMove(SPFiarGame* src, int col) {
	if (col < 0 || col > SP_FIAR_GAME_N_COLUMNS -1 || src == NULL)
		return false;
	if ((src->tops)[col] == SP_FIAR_GAME_N_ROWS)
		return false;
	return true;
}

/**
* Removes a disc that was put in the previous move and changes the current
* player's turn. If the user invoked this command more than historySize times
* in a row then an error occurs.
*
* @param src - The source game
* @return
* SP_FIAR_GAME_INVALID_ARGUMENT - if src == NULL
* SP_FIAR_GAME_NO_HISTORY       - if the user invoked this function more then
*                                 historySize in a row.
* SP_FIAR_GAME_SUCCESS          - on success. The last disc that was put on the
*                                 board is removed and the current player is changed
*/
SP_FIAR_GAME_MESSAGE spFiarGameUndoPrevMove(SPFiarGame* src) {
	int lastCol = 0, lastRow = 0;
	if (src == NULL) 
		return SP_ARRAY_LIST_INVALID_ARGUMENT;
	if (src->history->actualSize == 0)
		return SP_FIAR_GAME_NO_HISTORY;
	spFiarGameSwitchPlayer(src);
	lastCol = spArrayListGetLast(src->history);
	(src->tops)[lastCol]--;
	spArrayListRemoveLast(src->history);
	return SP_FIAR_GAME_SUCCESS;
}

/**
* On success, the function prints the board game. If an error occurs, then the
* function does nothing. The characters 'X' and 'O' are used to represent
* the discs of player 1 and player 2, respectively.
*
* @param src - the target game
* @return
* SP_FIAR_GAME_INVALID_ARGUMENT - if src==NULL
* SP_FIAR_GAME_SUCCESS - otherwise
*
*/
SP_FIAR_GAME_MESSAGE spFiarGamePrintBoard(SPFiarGame* src) {
	int i = 0, j = 0;
	if (src == NULL)
		return SP_FIAR_GAME_INVALID_ARGUMENT;
	for (i = SP_FIAR_GAME_N_ROWS-1; i >=0 ; i--) {
		printf("|");
		for (j = 0; j < SP_FIAR_GAME_N_COLUMNS; j++) {
			if (src->tops[j] > i)
				printf(" %c", src->gameBoard[i][j]);
			else
				printf("  ");
		}
		printf(" |\n");
	}
	for (i = 0; i < 2 * SP_FIAR_GAME_N_COLUMNS + 3; i++)
		printf("-");
	printf("\n  ");
	for (i = 1; i <= SP_FIAR_GAME_N_COLUMNS; i++)
		printf("%d ", i);
	printf(" \n");
	return SP_FIAR_GAME_SUCCESS;
}

/**
* Returns the current player of the specified game.
* @param src - the source game
* @return
* SP_FIAR_GAME_PLAYER_1_SYMBOL - if it's player one's turn
* SP_FIAR_GAME_PLAYER_2_SYMBOL - if it's player two's turn
* SP_FIAR_GAME_EMPTY_ENTRY     - otherwise
*/
char spFiarGameGetCurrentPlayer(SPFiarGame* src) {
	if (src == NULL)
		return SP_FIAR_GAME_EMPTY_ENTRY;
	return src->currentPlayer;
}




/**
* Checks if there's a winner in the specified game status. The function returns either
* SP_FIAR_GAME_PLAYER_1_SYMBOL or SP_FIAR_GAME_PLAYER_2_SYMBOL in case there's a winner, where
* the value returned is the symbol of the winner. If the game is over and there's a tie
* then the value SP_FIAR_GAME_TIE_SYMBOL is returned. in any other case the null characters
* is returned.
* @param src - the source game
* @return
* SP_FIAR_GAME_PLAYER_1_SYMBOL - if player 1 won
* SP_FIAR_GAME_PLAYER_2_SYMBOL - if player 2 won
* SP_FIAR_GAME_TIE_SYMBOL - If the game is over and there's a tie
* null character - otherwise
*/

bool checkColWinner(SPFiarGame* src, int lastCol, int lastRow, char lastMove) {
	int i = 0, curChecked = lastRow - 1, curRow= lastRow;
	while (curRow > lastRow - SP_FIAR_GAME_SPAN && curRow >= 0) {
		if ((src->gameBoard)[curRow][lastCol] != lastMove)
			return false;
		curRow--;
	}
		return true;
}

bool checkRowWinner(SPFiarGame* src, int lastCol, int lastRow, char lastMove) {
	int i = 0, counter = 1;
	for (i = lastCol + 1; i < SP_FIAR_GAME_N_COLUMNS; i++) {
		if (lastRow < src->tops[i] && src->gameBoard[lastRow][i] == lastMove)
			counter++;
		else
			break;
	}
	for (i = lastCol - 1; i >= 0; i--) {
		if (lastRow < src->tops[i] && src->gameBoard[lastRow][i] == lastMove)
			counter++;
		else
			break;
	}
	return counter >= SP_FIAR_GAME_SPAN;
}

bool checkDiagUpWinner(SPFiarGame* src, int lastCol, int lastRow, char lastMove) {
	int i = 0, lastRowCopy = lastRow, counter = 1;
	for (i = lastCol + 1; i < SP_FIAR_GAME_N_COLUMNS; i++) {
		if (++lastRowCopy < src->tops[i] && src->gameBoard[lastRowCopy][i] == lastMove)
			counter++;
		else
			break;
	}
	lastRowCopy = lastRow;
	for (i = lastCol - 1; i >= 0; i--) {
		if (--lastRowCopy >= 0 && lastRowCopy < src->tops[i] && src->gameBoard[lastRowCopy][i] == lastMove)
			counter++;
		else
			break;
	}
	return counter >= SP_FIAR_GAME_SPAN;
}

bool checkDiagDownWinner(SPFiarGame* src, int lastCol, int lastRow, char lastMove) {
	int i = 0, lastRowCopy = lastRow, counter = 1;
	for (i = lastCol - 1; i >= 0; i--) {
		if (++lastRowCopy < src->tops[i] && src->gameBoard[lastRowCopy][i] == lastMove)
			counter++;
		else
			break;
	}
	lastRowCopy = lastRow;
	for (i = lastCol + 1; i < SP_FIAR_GAME_N_COLUMNS; i++) {
		if (--lastRowCopy >= 0 && lastRowCopy < src->tops[i] && src->gameBoard[lastRowCopy][i] == lastMove)
			counter++;
		else
			break;
	}
	return counter >= SP_FIAR_GAME_SPAN;
}

bool isGameBoardFull(SPFiarGame* src) {
	int colNum = SP_FIAR_GAME_N_COLUMNS, rowNum = SP_FIAR_GAME_N_ROWS, i=0;
	for (i = 0; i < colNum; i++) {
		if (src->tops[i] != rowNum)
			return false;
	}
	return true;
}
char spFiarCheckWinner(SPFiarGame* src) {
	int lastCol = 0, lastRow = 0;
	char lastMove = 'a';
	bool rowWinner = true, colWinner = true, diagWinner = true;
	if (src == NULL)
		return '\0';
	lastCol = spArrayListGetLast(src->history);
	lastRow = (src->tops)[lastCol];
	lastMove = (src->gameBoard)[lastRow][lastCol];
	rowWinner = checkRowWinner(src, lastCol, lastRow, lastMove);
	colWinner = checkColWinner(src, lastCol, lastRow, lastMove);
	diagWinner = checkDiagUpWinner(src, lastCol, lastRow, lastMove) || checkDiagDownWinner(src, lastCol, lastRow, lastMove);
	if (rowWinner || colWinner || diagWinner)
		return lastMove;
	else if (isGameBoardFull(src))
		return SP_FIAR_GAME_TIE_SYMBOL;
	return '\0';
}
char getOtherPlayer(char player) {
	if (player == SP_FIAR_GAME_PLAYER_1_SYMBOL)
		return SP_FIAR_GAME_PLAYER_2_SYMBOL;
	return SP_FIAR_GAME_PLAYER_1_SYMBOL;
}
#endif
