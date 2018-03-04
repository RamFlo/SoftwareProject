#include "ChessGame.h"
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>


void ChessGameSwitchPlayer(ChessGame* src) {
	if (src != NULL) {
		if (src->currentPlayer == WHITE_PLAYER)
			src->currentPlayer = BLACK_PLAYER;
		else
			src->currentPlayer = WHITE_PLAYER;
	}
}

void addWhitePieces(ChessGame* g) {
	int i = 0;
	for (i = 0; i < 8; i++) {
		g->gameBoard[6][i] = WHITE_PAWN;
		if(i==0||i==7)
			g->gameBoard[7][i] = WHITE_ROOK;
		if(i == 1 || i == 6)
			g->gameBoard[7][i] = WHITE_KNIGHT;
		if (i == 2 || i == 5)
			g->gameBoard[7][i] = WHITE_BISHOP;
	}
	g->gameBoard[7][3] = WHITE_QUEEN;
	g->gameBoard[7][4] = WHITE_KING;
}

void addBlackPieces(ChessGame* g) {
	int i = 0;
	for (i = 0; i < 8; i++) {
		g->gameBoard[1][i] = BLACK_PAWN;
		if (i == 0 || i == 7)
			g->gameBoard[0][i] = BLACK_ROOK;
		if (i == 1 || i == 6)
			g->gameBoard[0][i] = BLACK_KNIGHT;
		if (i == 2 || i == 5)
			g->gameBoard[0][i] = BLACK_BISHOP;
	}
	g->gameBoard[0][3] = BLACK_QUEEN;
	g->gameBoard[0][4] = BLACK_KING;
}

ChessGame* ChessGameCreate(int historySize) {
	ChessGame* g;
	int i = 0, j = 0;
	g = (ChessGame *)malloc(sizeof(ChessGame));
	if (g == NULL) {
		printf("Error: malloc has failed\n");
		return NULL;
	}
	for (i = 0; i < 8; i++) {
		for (j = 0; j < 8; j++) {
			(g->gameBoard)[i][j] = '\0';
		}
	}
	addWhitePieces(g);
	addBlackPieces(g);
	g->checked = '\0';
	g->checkmated = '\0';
	g->draw = false;
	g->currentPlayer = WHITE_PLAYER;
	g->history = spArrayListCreate(historySize);
	g->gameMode = 1;
	g->difficulty = 2;
	g->userColor = 1;
	if (g->history == NULL)
		return NULL;
	return g;
}

ChessGame* ChessGameCopy(ChessGame* src) {
	ChessGame *g;
	int i = 0, j = 0, k = 0;
	SPArrayList* p;
	if (src == NULL)
		return NULL;
	g = ChessGameCreate(src->history->maxSize);
	if (g == NULL)
		return NULL;
	g->currentPlayer = src->currentPlayer;
	for (i = 0; i < 8; i++) {
		for (j = 0; j < 8; j++) {
			(g->gameBoard)[i][j] = (src->gameBoard)[i][j];
		}
	}
	p = spArrayListCopy(src->history);
	g->checked = src->checked;
	spArrayListDestroy(g->history);
	g->history = p;
	return g;
}

void ChessGameDestroy(ChessGame* src) {
	if (src != NULL) {
		spArrayListDestroy(src->history);
		free(src);
	}
}

bool isValidSquare(int r1_n, int c1_n) {
	if (r1_n < 1 || r1_n>8 || c1_n < 1 || c1_n>8)
		return false;
	return true;
}

bool isCurPlayerPiece(int r1_n, int c1_n, ChessGame* src) {
	bool blackPiece;
	if (src->gameBoard[r1_n][c1_n] == '\0')
		return false;
	blackPiece = isupper(src->gameBoard[r1_n][c1_n]);
	if ((src->currentPlayer == BLACK_PLAYER && blackPiece) || (src->currentPlayer == WHITE_PLAYER && !blackPiece))
		return true;
	return false;
}

bool isOtherPlayerPiece(int r1_n, int c1_n, ChessGame* src) {
	if (src->gameBoard[r1_n][c1_n] == '\0' || isCurPlayerPiece(r1_n, c1_n, src))
		return false;
	return true;
}

bool isOtherPlayerPiece(ChessGame* src, int r1_n, int c1_n) {
	if (src->gameBoard[r1_n][c1_n] == '\0' || isCurPlayerPiece(r1_n, c1_n, src))
		return false;
	return true;
}

char getOtherPlayer(ChessGame* src) {
	if (src->currentPlayer == WHITE_PLAYER)
		return BLACK_PLAYER;
	return WHITE_PLAYER;
}

bool isDiagonal(int r1_n, int c1_n, int r2_n, int c2_n) {
	if (abs(r1_n - r2_n) == abs(c1_n - c2_n))
		return true;
	return false;
}

bool isStraightLine(int r1_n, int c1_n, int r2_n, int c2_n) {
	if (r1_n - r2_n == 0 || c1_n - c2_n == 0)
		return true;
	return false;
}

bool isLegalKnightMove(ChessGame* src, int r1_n, int c1_n, int r2_n, int c2_n) {
	bool isFriendlyPiece = false;
	if (src->gameBoard[r2_n][c2_n] != '\0') {
		if (src->currentPlayer == BLACK_PLAYER) {
			if (isupper(src->gameBoard[r2_n][c2_n]))
				isFriendlyPiece = true;
		}
		else {
			if (!isupper(src->gameBoard[r2_n][c2_n]))
				isFriendlyPiece = true;
		}
	}
	if (isFriendlyPiece)
		return false;
	if (r1_n == r2_n + 1 || r1_n == r2_n - 1) {
		if (c1_n == c2_n + 2 || c1_n == c2_n - 2)
			return true;
	}	
	else if (r1_n == r2_n + 2 || r1_n == r2_n - 2) {
		if (c1_n == c2_n + 1 || c1_n == c2_n - 1)
			return true;
	}
	return false;
}

bool isLegalKingMove(ChessGame* src, int r1_n, int c1_n, int r2_n, int c2_n) {
	if (abs(r1_n - r2_n) > 1)
		return false;
	if (abs(c1_n - c2_n) > 1)
		return false;
	return (!blockedPathCheck(src, r1_n, c1_n, r2_n, c2_n));
}

bool isLegalQueenMove(ChessGame* src, int r1_n, int c1_n, int r2_n, int c2_n) {
	if (!isStraightLine(r1_n, c1_n, r2_n, c2_n) && !isDiagonal(r1_n, c1_n, r2_n, c2_n))
		return false;
	return (!blockedPathCheck(src, r1_n, c1_n, r2_n, c2_n));
}

bool isLegalBishopMove(ChessGame* src, int r1_n, int c1_n, int r2_n, int c2_n) {
	if (!isDiagonal(r1_n, c1_n, r2_n, c2_n))
		return false;
	return (!blockedPathCheck(src, r1_n, c1_n, r2_n, c2_n));
}

bool isLegalRookMove(ChessGame* src, int r1_n, int c1_n, int r2_n, int c2_n) {
	if (!isStraightLine(r1_n, c1_n, r2_n, c2_n))
		return false;
	return (!blockedPathCheck(src, r1_n, c1_n, r2_n, c2_n));
}

bool isLegalWhitePawnMove(ChessGame* src, int r1_n, int c1_n, int r2_n, int c2_n) {
	if (r2_n == r1_n - 1 && c2_n == c1_n) //minus 1 because white pawns start in row 6 and move towards row 0
		return (!blockedPathCheck(src, r1_n, c1_n, r2_n, c2_n));
	else if (r2_n == r1_n - 2 && c2_n == c1_n) {
		if (r1_n != 6)
			return false;
		return (!blockedPathCheck(src, r1_n, c1_n, r2_n, c2_n));
	}
	else if (r2_n == r1_n - 1 && abs(c1_n - c2_n) == 1) {
		if (isOtherPlayerPiece(src, r2_n, c2_n))
			return true;
	}
	return false;
}

bool isLegalBlackPawnMove(ChessGame* src, int r1_n, int c1_n, int r2_n, int c2_n) {
	if (r2_n == r1_n + 1 && c2_n == c1_n) //plus 1 because black pawns start in row 1 and move towards row 7
		return (!blockedPathCheck(src, r1_n, c1_n, r2_n, c2_n));
	else if (r2_n == r1_n + 2 && c2_n == c1_n) {
		if (r1_n != 1)
			return false;
		return (!blockedPathCheck(src, r1_n, c1_n, r2_n, c2_n));
	}
	else if (r2_n == r1_n + 1 && abs(c1_n - c2_n) == 1) {
		if (isOtherPlayerPiece(src, r2_n, c2_n))
			return true;
	}
	return false;
}

int findCurPlayerKing(ChessGame* src) {
	int i = 0, j = 0, rc_index = 0;
	char king = (src->currentPlayer == WHITE_PLAYER) ? WHITE_KING : BLACK_KING;
	for (i = 0; i < 8; i++) {
		for (j = 0; j < 8; j++) {
			if (src->gameBoard[i][j] == king)
				rc_index=10 * i + j;
		}
	}
	return rc_index;
}

bool isLegalMove(ChessGame* src, int r1_n, int c1_n, int r2_n, int c2_n) {
	bool isLegalMove = false;
	switch (src->gameBoard[r1_n][c1_n]) {
	case WHITE_PAWN:
		isLegalMove = isLegalWhitePawnMove(src, r1_n, c1_n, r2_n, c2_n);
		break;
	case BLACK_PAWN:
		isLegalMove = isLegalBlackPawnMove(src, r1_n, c1_n, r2_n, c2_n);
		break;
	case WHITE_KNIGHT:
	case BLACK_KNIGHT:
		isLegalMove = isLegalKnightMove(src, r1_n, c1_n, r2_n, c2_n);
		break;
	case WHITE_BISHOP:
	case BLACK_BISHOP:
		isLegalMove = isLegalBishopMove(src, r1_n, c1_n, r2_n, c2_n);
		break;
	case WHITE_ROOK:
	case BLACK_ROOK:
		isLegalMove = isLegalRookMove(src, r1_n, c1_n, r2_n, c2_n);
		break;
	case WHITE_QUEEN:
	case BLACK_QUEEN:
		isLegalMove = isLegalQueenMove(src, r1_n, c1_n, r2_n, c2_n);
		break;
	case WHITE_KING:
	case BLACK_KING:
		isLegalMove = isLegalKingMove(src, r1_n, c1_n, r2_n, c2_n);
		break;
	}
	return isLegalMove;
}
bool isSquareThreatened(ChessGame* src, int r1_n, int c1_n) {
	int i = 0, j = 0;
	bool upperSquare = false, shouldCheck = false;
	char curSquare = '\0';
	for (i = 0; i < 8; i++) {
		for (j = 0; j < 8; j++) {
			curSquare = src->gameBoard[i][j];
			if (curSquare != '\0') {
				shouldCheck = (src->currentPlayer == WHITE_PLAYER) ? isupper(curSquare) : !isupper(curSquare);
				if (shouldCheck && isLegalMove(src, i, j, r1_n, c1_n))
					return true;
			}
		}
	}
	return false;
}

bool isCurKingThreatened(ChessGame* src) {
	int kingCol = 0, kingRow = 0, res;
	res = findCurPlayerKing(src);
	kingCol = res % 10;
	kingRow = res / 10;
	return isSquareThreatened(src, kingRow, kingCol);
}



void executeMove(ChessGame* src, int r1_n, int c1_n, int r2_n, int c2_n) {
	src->gameBoard[r2_n][c2_n] = src->gameBoard[r1_n][c1_n];
	src->gameBoard[r1_n][c1_n] = '\0';
}

bool isKingStillChecked(ChessGame* src, int r1_n, int c1_n, int r2_n, int c2_n) {
	bool KingStillChecked = false;
	ChessGame* gameCopy= ChessGameCopy(src);
	executeMove(gameCopy, r1_n, c1_n, r2_n, c2_n);
	KingStillChecked = isCurKingThreatened(gameCopy);
	ChessGameDestroy(gameCopy);
	return KingStillChecked;
}

bool isCheckmate(ChessGame* src) {
	int i, j, k, l, res;
	bool upperSquare = false, shouldCheck = false;
	char curSquare = '\0';
	res = findCurPlayerKing(src);
	for (i = 0; i < 8; i++) {
		for (j = 0; j < 8; j++) {
			curSquare = src->gameBoard[i][j];
			if (curSquare != '\0') {
				shouldCheck = (src->currentPlayer == WHITE_PLAYER) ? !isupper(curSquare) : isupper(curSquare);
				if (shouldCheck) {
					for (k = 0; j < 8; j++) {
						for (l = 0; l < 8; l++) {
							if (isLegalMove(src, i, j, k, l) && !isKingStillChecked(src, i, j, k, l))
								return false;
						}
					}
				}
			}
		}
	}
	return true;
}

void addMoveToHistory(ChessGame* src, int r1_n, int c1_n, int r2_n, int c2_n) {
	int i = 0;
	if (spArrayListAddLast(src->history, r1_n) != SP_ARRAY_LIST_SUCCESS) {
		for(i=0;i<5;i++)
			spArrayListRemoveFirst(src->history);
		spArrayListAddLast(src->history, r1_n);
	}
	spArrayListAddLast(src->history, c1_n);
	spArrayListAddLast(src->history, r2_n);
	spArrayListAddLast(src->history, c2_n);
	spArrayListAddLast(src->history, src->gameBoard[r2_n][c2_n]);
}

CHESS_GAME_MESSAGE ChessGameSetMove(ChessGame* src, char r1, char c1, char r2, char c2) {
	int r1_n = 8-(r1 - '0'), r2_n = 8 - (r2 - '0'), c1_n = c1 - 'A', c2_n = c2 - 'A';
	if (src == NULL || !isValidSquare(r1_n, c1_n) || !isValidSquare(r2_n, c2_n))
		return INVALID_POSITION;
	if (!isCurPlayerPiece(r1_n,c1_n,src))
		return NOT_PLAYER_PIECE;
	if (r1_n == r2_n && c1_n == c2_n)
		return ILLEGAL_MOVE;
	if (!isLegalMove(src, r1_n, c1_n, r2_n, c2_n))
		return ILLEGAL_MOVE;
	if (isKingStillChecked(src, r1_n, c1_n, r2_n, c2_n)) {
		if (src->checked == src->currentPlayer)
			return KING_STILL_THREATENED;
		else
			return KING_NOW_THREATENED;
	}
	addMoveToHistory(src, r1_n, c1_n, r2_n, c2_n);
	executeMove(src, r1_n, c1_n, r2_n, c2_n);
	ChessGameSwitchPlayer(src);
	if (isCurKingThreatened(src)) {
		src->checked = src->currentPlayer;
		if (isCheckmate(src))
			src->checkmated = src->currentPlayer;
	}
	else {
		if (isCheckmate(src))
			src->draw = true;
	}
	return SUCCESS;
}

CHESS_GAME_MESSAGE ChessGameGetMoves(ChessGame* src, char r1, char c1) {
	int i = 0, j = 0, r1_n = 8 - (r1 - '0'), c1_n = c1 - 'A';
	if (src == NULL || !isValidSquare(r1_n, c1_n))
		return INVALID_POSITION;
	if (src->gameBoard[r1_n][c1_n] == '\0')
		return NO_PIECE_IN_SQUARE;
	for (i = 0; i < 8; i++) {
		for (j = 0; j < 8; j++) {
			if (isLegalMove(src, r1_n, c1_n, i, j) && !isKingStillChecked(src, r1_n, c1_n, i, j)) {
				printf("<%c,%c>", '8' - i, 'A' + j);
				if (isSquareThreatened(src, i, j))
					printf("*");
				if (isOtherPlayerPiece(i, j, src))
					printf("^");
				printf("\n");
			}
		}
	}
	return SUCCESS;
}


CHESS_GAME_MESSAGE ChessGameUndoPrevMove(ChessGame* src) {
	char lastPiece = spArrayListGetLast(src->history);
	int i=0,size = src->history->actualSize, dstCol = 0, dstRow = 0, srcCol = 0, srcRow = 0;
	if (size == 0) {
		printf("Empty history, no move to undo\n");
		return NO_HISTORY;
	}
	dstCol = spArrayListGetAt(src->history, size - 2);
	dstRow = spArrayListGetAt(src->history, size - 3);
	srcCol = spArrayListGetAt(src->history, size - 4);
	srcRow = spArrayListGetAt(src->history, size - 5);
	src->gameBoard[srcRow][srcCol] = src->gameBoard[dstRow][dstCol];
	src->gameBoard[dstRow][dstCol] = lastPiece;
	ChessGameSwitchPlayer(src);
	if (src->currentPlayer == WHITE_PLAYER)
		printf("Undo move for black player: <%c,%c> -> <%c,%c>\n", '8'-dstRow, 'A' + dstCol, '8'-srcRow, 'A'+srcCol);
	else
		printf("Undo move for white player: <%c,%c> -> <%c,%c>\n", '8' - dstRow, 'A' + dstCol, '8' - srcRow, 'A' + srcCol);
	for (i = 0; i < 5; i++)
		spArrayListRemoveLast(src->history);
	return SUCCESS;
}

CHESS_GAME_MESSAGE ChessGamePrintBoard(ChessGame* src) {
	int i = 0, j = 0;
	if (src == NULL)
		return NULL_SRC;
	for (i = 0; i < 8; i++) {
		printf("%d|", 8 - i);
		for (j = 0; j < 8; j++) {
			if (src->gameBoard[i][j] != '\0')
				printf(" %c", src->gameBoard[i][j]);
			else
				printf(" _");
		}
		printf(" |\n");
	}
	printf("  -----------------\n   A B C D E F G H\n");
	return SUCCESS;
}

CHESS_GAME_MESSAGE ChessGameSave(ChessGame* src, char* path) {
	int i = 0, j = 0,size= src->history->actualSize;
	FILE* myFile = fopen(path, "r+");
	if (src == NULL) {
		fclose(myFile);
		return NULL_SRC;
	}
	if (myFile == NULL) {
		fclose(myFile);
		return FILE_CREATE_FAILED;
	}
	for (i = 0; i < 8; i++) {
		for (j = 0; j < 8; j++) {
			if (putc(src->gameBoard[i][j], myFile) == -1)
			{
				printf("ERROR: writing to file failed.");
				ChessGameDestroy(src);
				fclose(myFile);
				exit(0);
			}
		}
	}
	putc(src->gameMode, myFile);
	putc(src->difficulty, myFile);
	putc(src->userColor, myFile);
	putc(src->currentPlayer, myFile);
	putc(src->checked, myFile);
	putc(src->checkmated, myFile);
	putc(src->draw, myFile);
	putc(size, myFile);
	for (i = 0; i < size; i++)
		putc(spArrayListGetAt(src->history, i), myFile);
	fclose(myFile);
	return SUCCESS;
}


CHESS_GAME_MESSAGE ChessGameLoad(ChessGame* src, char* path) {
	int i = 0, j = 0, size=0;
	char c = '\0';
	FILE* myFile = fopen(path, "r+");
	if (src == NULL) {
		fclose(myFile);
		return NULL_SRC;
	}
	if (myFile == NULL) {
		fclose(myFile);
		return FILE_CREATE_FAILED;
	}
	for (i = 0; i < 8; i++) {
		for (j = 0; j < 8; j++) {
			if ((c=getc(myFile)) == -1)
			{
				printf("ERROR: writing to file failed.");
				ChessGameDestroy(src);
				fclose(myFile);
				exit(0);
			}
			src->gameBoard[i][j] = c;
		}
	}
	src->gameMode = getc(myFile);
	src->difficulty = getc(myFile);
	src->userColor = getc(myFile);
	src->currentPlayer = getc(myFile);
	src->checked = getc(myFile);
	src->checkmated = getc(myFile);
	src->draw = getc(myFile);
	size = getc(myFile);
	for (i = 0; i < size; i++)
		spArrayListAddLast(src->history, getc(myFile));
	fclose(myFile);
	return SUCCESS;
}

void printDifficulty(ChessGame* src) {
	switch (src->difficulty)
	{
	case 1:
		printf("DIFFICULTY: amateur\n");
		break;
	case 2:
		printf("DIFFICULTY: easy\n");
		break;
	case 3:
		printf("DIFFICULTY: moderate\n");
		break;
	case 4:
		printf("DIFFICULTY: hard\n");
		break;
	case 5:
		printf("DIFFICULTY: expert\n");
		break;
	}
}

void printSettings(ChessGame* src) {
	printf("SETTINGS:\n");
	printf("GAME_MODE: %d-player\n", src->gameMode);
	if (src->gameMode == 1) {
		printDifficulty(src);
		if (src->userColor == 0)
			printf("USER_COLOR: black\n");
		else
			printf("USER_COLOR: white\n");
	}
}

void quit(ChessGame* src) {
	ChessGameDestroy(src);
	printf("Exiting...\n");
	exit(0);
}


void default(ChessGame* g) {
	int i = 0, j = 0,size= g->history->actualSize;
	for (i = 0; i < 8; i++) {
		for (j = 0; j < 8; j++) {
			(g->gameBoard)[i][j] = '\0';
		}
	}
	addWhitePieces(g);
	addBlackPieces(g);
	g->checked = '\0';
	g->checkmated = '\0';
	g->draw = false;
	g->currentPlayer = WHITE_PLAYER;
	g->gameMode = 1;
	g->difficulty = 2;
	g->userColor = 1;
	for (i = 0; i < size; i++) {
		spArrayListRemoveLast(g->history);
	}
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
	lastRow = (src->tops)[lastCol] - 1;
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


bool blockedPathCheck(ChessGame* src, int r1_n, int c1_n, int r2_n, int c2_n) {
	int i = 0, j = 0, lower_r = 0, left_c = 0, upper_r = 0, right_c = 0, diag_type = 0, uppercase = 0;
	lower_r = (r1_n > r2_n) ? r1_n : r2_n;
	upper_r = (r1_n > r2_n) ? r2_n : r1_n;
	left_c = (c1_n < c2_n) ? c1_n : c2_n;
	right_c = (c1_n < c2_n) ? c2_n : c1_n; 
	diag_type = ((r1_n > r2_n &&c1_n < c2_n) || (r1_n < r2_n &&c1_n > c2_n)) ? 1 : -1; 
	if (c1_n == c2_n) {		//forward
		for (i = lower_r - 1; i >= upper_r + 1; i--) {
			if (src->gameBoard[i][c1_n] != '\0')
				return true;
		}
	} 
	else if (r1_n == r2_n) {		//sideways
		for (i = left_c + 1; i <= right_c - 1; i++) {
			if (src->gameBoard[r1_n][i] != '\0')
				return true;
		}
	}
	else if (diag_type == 1) {		//diag: '/'
		for (i = 1; i <= lower_r - upper_r - 1; i++) {
			if (src->gameBoard[lower_r - i][left_c + i] != '\0')
				return true;
		}
	}
	else if (diag_type == -1) {		//diag: '\'
		for (i = 1; i <= lower_r - upper_r - 1; i++) {
			if (src->gameBoard[lower_r - i][right_c - i] != '\0')
				return true;
		}
	}
	if (isCurPlayerPiece(r2_n, c2_n, src))
		return true;
	return false;
}