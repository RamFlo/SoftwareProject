#include "SPFIARGame.h"
#include <stdlib.h>
#include <stdio.h>

/*
* Given an SPFiarGame object, this function switches the object's current player to the other player.
* @param src - A SPFiarGame object.
*/
void spFiarGameSwitchPlayer(SPFiarGame* src) {
	if (src != NULL) {
		if (src->currentPlayer == SP_FIAR_GAME_PLAYER_1_SYMBOL)
			src->currentPlayer = SP_FIAR_GAME_PLAYER_2_SYMBOL;
		else
			src->currentPlayer = SP_FIAR_GAME_PLAYER_1_SYMBOL;
	}
}

SPFiarGame* spFiarGameCreate(int historySize) {
	SPFiarGame* g;
	int i = 0,j=0;
	g = (SPFiarGame *)malloc(sizeof(SPFiarGame));
	if (g == NULL) {
		printf("Error: malloc has failed\n");
		return NULL;
	}	
	for (i = 0; i < SP_FIAR_GAME_N_ROWS; i++) {
		for (j = 0; j < SP_FIAR_GAME_N_COLUMNS; j++) {
			(g->gameBoard)[i][j] = '\0';
		}
	}
	for (i = 0; i < SP_FIAR_GAME_N_COLUMNS; i++)
		g->tops[i] = 0;
	g->currentPlayer = SP_FIAR_GAME_PLAYER_1_SYMBOL;
	g->history = spArrayListCreate(historySize);
	if (g->history == NULL)
		return NULL;
	return g;
}

SPFiarGame* spFiarGameCopy(SPFiarGame* src) {
	SPFiarGame *g;
	int i = 0, j = 0, k = 0;
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
	//free(g->history);
	spArrayListDestroy(g->history);
	g->history = p;
	return g;
}

void spFiarGameDestroy(SPFiarGame* src) {
	if (src != NULL) {
		spArrayListDestroy(src->history);
		free(src);
	}
}

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

bool spFiarGameIsValidMove(SPFiarGame* src, int col) {
	if (col < 0 || col > SP_FIAR_GAME_N_COLUMNS - 1 || src == NULL)
		return false;
	if ((src->tops)[col] == SP_FIAR_GAME_N_ROWS)
		return false;
	return true;
}

SP_FIAR_GAME_MESSAGE spFiarGameUndoPrevMove(SPFiarGame* src) {
	int lastCol = 0;
	if (src == NULL)
		return SP_ARRAY_LIST_INVALID_ARGUMENT;
	if (src->history->actualSize == 0) {
		printf("Error: cannot undo previous move!\n");
		return SP_FIAR_GAME_NO_HISTORY;
	}
	lastCol = spArrayListGetLast(src->history);
	spFiarGameSwitchPlayer(src);
	if (src->currentPlayer == SP_FIAR_GAME_PLAYER_1_SYMBOL)
		printf("Remove disc: remove user's disc at column %d\n", lastCol + 1);
	else
		printf("Remove disc: remove computer's disc at column %d\n", lastCol + 1);
	(src->tops)[lastCol]--;
	spArrayListRemoveLast(src->history);
	return SP_FIAR_GAME_SUCCESS;
}

SP_FIAR_GAME_MESSAGE spFiarGamePrintBoard(SPFiarGame* src) {
	int i = 0, j = 0;
	if (src == NULL)
		return SP_FIAR_GAME_INVALID_ARGUMENT;
	for (i = SP_FIAR_GAME_N_ROWS - 1; i >= 0; i--) {
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

char spFiarGameGetCurrentPlayer(SPFiarGame* src) {
	if (src == NULL)
		return SP_FIAR_GAME_EMPTY_ENTRY;
	return src->currentPlayer;
}

/*
* Given the previous player, the col and row of it's move and the current game status
* this function determines whether the last move created a "Column win situation".
* Namely, whether the last added disc completed a 4 in a column sequence.
* @param src - the current game status
* @param lastCol - the index of the last move's column, 0 based
* @param lastRow - the index of the last move's row, 0 based
* @param lastMove - the symbol of the player who played the last move
* @return
* a boolean representing whether there was a "Column Win" or not.
* Namely, True if the last move created 4 in a column, False otherwise.
*/
bool checkColWinner(SPFiarGame* src, int lastCol, int lastRow, char lastMove) {
	int curRow = lastRow;
	if (lastRow < SP_FIAR_GAME_SPAN - 1)
		return false;
	while (curRow > lastRow - SP_FIAR_GAME_SPAN && curRow >= 0) {
		if ((src->gameBoard)[curRow][lastCol] != lastMove)
			return false;
		curRow--;
	}
	return true;
}
/*
* Given the previous player, the col and row of it's move and the current game status
* this function determines whether the last move created a "Row win situation".
* Namely, whether the last added disc completed a 4 in a row sequence.
* @param src - the current game status
* @param lastCol - the index of the last move's column, 0 based
* @param lastRow - the index of the last move's row, 0 based
* @param lastMove - the symbol of the player who played the last move
* @return
* a boolean representing whether there was a "Row Win" or not.
* Namely, True if the last move created 4 in a row, False otherwise.
*/
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
/*
* Given the previous player, the col and row of it's move and the current game status
* this function determines whether the last move created a "Upword sloping diagonal win situation".
* Namely, whether the last added disc completed a 4 in a diagonal sequence, in which the diagonal is upword sloping.
* An example for an upword sloping diagonal - /.
* @param src - the current game status
* @param lastCol - the index of the last move's column, 0 based
* @param lastRow - the index of the last move's row, 0 based
* @param lastMove - the symbol of the player who played the last move
* @return
* a boolean representing whether there was a "Upword sloping diagonal win" or not.
* Namely, True if the last move created 4 in a Upword sloping diagonal, False otherwise.
*/
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

/*
* Given the previous player, the col and row of it's move and the current game status
* this function determines whether the last move created a "Downward sloping diagonal win situation".
* Namely, whether the last added disc completed a 4 in a diagonal sequence, in which the diagonal is downward sloping.
* An example for an downward sloping diagonal - \.
* @param src - the current game status
* @param lastCol - the index of the last move's column, 0 based
* @param lastRow - the index of the last move's row, 0 based
* @param lastMove - the symbol of the player who played the last move
* @return
* a boolean representing whether there was a "Downward sloping diagonal win" or not.
* Namely, True if the last move created 4 in a Downward sloping diagonal, False otherwise.
*/
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
/*
* Given a game status, this function checks whether the game board is full or not.
*@param src - current game status.
*@return
*True if the game board is full (no more moves can be made and all columns are full).
*False otherwise.
*/
bool isGameBoardFull(SPFiarGame* src) {
	int colNum = SP_FIAR_GAME_N_COLUMNS, rowNum = SP_FIAR_GAME_N_ROWS, i = 0;
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
	if (src->history->actualSize == 0)
		return '\0';
	lastCol = spArrayListGetLast(src->history);
	lastRow = (src->tops)[lastCol]-1;
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