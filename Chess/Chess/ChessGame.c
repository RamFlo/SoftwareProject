#include "ChessGame.h"
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>

//checks if two squares have pieces of opposing colors
bool isOppositeColorsSquares(ChessGame* src, int r1_n, int c1_n, int r2_n, int c2_n) {
	char s1 = src->gameBoard[r1_n][c1_n], s2 = src->gameBoard[r2_n][c2_n];
	if (s1 == '\0' || s2 == '\0')
		return false;
	if ((isupper(s1) && !isupper(s2)) || (!isupper(s1) && isupper(s2)))
		return true;
	return false;
}

//given a char representing a piece, prints the piece's name
void printPieceName(char p) {
	switch (p) {
	case WHITE_PAWN:
	case BLACK_PAWN:
		printf("pawn");
		break;
	case WHITE_KNIGHT:
	case BLACK_KNIGHT:
		printf("knight");
		break;
	case WHITE_BISHOP:
	case BLACK_BISHOP:
		printf("bishop");
		break;
	case WHITE_ROOK:
	case BLACK_ROOK:
		printf("rook");
		break;
	case WHITE_QUEEN:
	case BLACK_QUEEN:
		printf("queen");
		break;
	case WHITE_KING:
	case BLACK_KING:
		printf("king");
		break;
	}
}

//switches the game's current player
void ChessGameSwitchPlayer(ChessGame* src) {
	if (src != NULL) {
		if (src->currentPlayer == WHITE_PLAYER)
			src->currentPlayer = BLACK_PLAYER;
		else
			src->currentPlayer = WHITE_PLAYER;
	}
}

//adds the white pieces to the board at the beginning of the game
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

//adds the black pieces to the board at the beginning of the game
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

//creates a new ChessGame, initializes the board and settings and returns a pointer to the newly created ChessGame
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

//copies a ChessGame, copiesthe board and settings and returns a pointer to the newly created ChessGame
ChessGame* ChessGameCopy(ChessGame* src) {
	ChessGame *g;
	int i = 0, j = 0;
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

//destroys a given ChessGame
void ChessGameDestroy(ChessGame* src) {
	if (src != NULL) {
		spArrayListDestroy(src->history);
		free(src);
	}
}

//given a src square and dest square - checks if the path from the src square to the dest square is blocked
bool blockedPathCheck(ChessGame* src, int r1_n, int c1_n, int r2_n, int c2_n) {
	int i = 0, lower_r = 0, left_c = 0, upper_r = 0, right_c = 0, diag_type = 0;
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
	//if (isCurPlayerPiece(src,r2_n, c2_n)) return true;
	if (src->gameBoard[r2_n][c2_n] != '\0' && !isOppositeColorsSquares(src, r1_n, c1_n, r2_n, c2_n))
		return true;
	return false;
}

//checks if given indexes represent a legal board square
bool isValidSquare(int r1_n, int c1_n) {
	if (r1_n < 0|| r1_n>7 || c1_n < 0 || c1_n>7)
		return false;
	return true;
}

//checks if a square contains a piece of the current player
bool isCurPlayerPiece(ChessGame* src, int r1_n, int c1_n) {
	bool blackPiece;
	if (src->gameBoard[r1_n][c1_n] == '\0')
		return false;
	blackPiece = isupper(src->gameBoard[r1_n][c1_n]);
	if ((src->currentPlayer == BLACK_PLAYER && blackPiece) || (src->currentPlayer == WHITE_PLAYER && !blackPiece))
		return true;
	return false;
}

//checks if a square contains a piece of the other player
bool isOtherPlayerPiece(ChessGame* src, int r1_n, int c1_n) {
	if (src->gameBoard[r1_n][c1_n] == '\0' || isCurPlayerPiece( src,r1_n, c1_n))
		return false;
	return true;
}

//returns the player which isn't (!) the current player
char getOtherPlayer(ChessGame* src) {
	if (src->currentPlayer == WHITE_PLAYER)
		return BLACK_PLAYER;
	return WHITE_PLAYER;
}

//checks if the path between two squares is a diagonal
bool isDiagonal(int r1_n, int c1_n, int r2_n, int c2_n) {
	if (abs(r1_n - r2_n) == abs(c1_n - c2_n))
		return true;
	return false;
}

//checks if the path between two squares is a straight line
bool isStraightLine(int r1_n, int c1_n, int r2_n, int c2_n) {
	if (r1_n - r2_n == 0 || c1_n - c2_n == 0)
		return true;
	return false;
}

//checks if the path between two squares is a legal knight move
bool isLegalKnightMove(ChessGame* src, int r1_n, int c1_n, int r2_n, int c2_n) {
	bool isFriendlyPiece = false;
	if (src->gameBoard[r2_n][c2_n] != '\0' && !isOppositeColorsSquares(src, r1_n, c1_n, r2_n, c2_n))
		isFriendlyPiece = true;
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

//checks if the path between two squares is a legal king move
bool isLegalKingMove(ChessGame* src, int r1_n, int c1_n, int r2_n, int c2_n) {
	if (abs(r1_n - r2_n) > 1)
		return false;
	if (abs(c1_n - c2_n) > 1)
		return false;
	return (!blockedPathCheck(src, r1_n, c1_n, r2_n, c2_n));
}

//checks if the path between two squares is a legal queen move
bool isLegalQueenMove(ChessGame* src, int r1_n, int c1_n, int r2_n, int c2_n) {
	if (!isStraightLine(r1_n, c1_n, r2_n, c2_n) && !isDiagonal(r1_n, c1_n, r2_n, c2_n))
		return false;
	return (!blockedPathCheck(src, r1_n, c1_n, r2_n, c2_n));
}

//checks if the path between two squares is a legal bishop move
bool isLegalBishopMove(ChessGame* src, int r1_n, int c1_n, int r2_n, int c2_n) {
	if (!isDiagonal(r1_n, c1_n, r2_n, c2_n))
		return false;
	return (!blockedPathCheck(src, r1_n, c1_n, r2_n, c2_n));
}

//checks if the path between two squares is a legal rook move
bool isLegalRookMove(ChessGame* src, int r1_n, int c1_n, int r2_n, int c2_n) {
	if (!isStraightLine(r1_n, c1_n, r2_n, c2_n))
		return false;
	return (!blockedPathCheck(src, r1_n, c1_n, r2_n, c2_n));
}

//checks if the path between two squares is a legal white pawn move
bool isLegalWhitePawnMove(ChessGame* src, int r1_n, int c1_n, int r2_n, int c2_n) {
	if (r2_n == r1_n - 1 && c2_n == c1_n) //minus 1 because white pawns start in row 6 and move towards row 0
		return (src->gameBoard[r2_n][c2_n]=='\0');
	else if (r2_n == r1_n - 2 && c2_n == c1_n) {
		if (r1_n != 6)
			return false;
		return (src->gameBoard[r2_n][c2_n] == '\0' && src->gameBoard[r2_n+1][c2_n] == '\0');
	}
	else if (r2_n == r1_n - 1 && abs(c1_n - c2_n) == 1) {
		if (isOppositeColorsSquares(src,r1_n, c1_n, r2_n, c2_n))
			return true;
	}
	return false;
}

//checks if the path between two squares is a legal black pawn move
bool isLegalBlackPawnMove(ChessGame* src, int r1_n, int c1_n, int r2_n, int c2_n) {
	if (r2_n == r1_n + 1 && c2_n == c1_n) //plus 1 because black pawns start in row 1 and move towards row 7
		return (src->gameBoard[r2_n][c2_n] == '\0');
	else if (r2_n == r1_n + 2 && c2_n == c1_n) {
		if (r1_n != 1)
			return false;
		return (src->gameBoard[r2_n][c2_n] == '\0' && src->gameBoard[r2_n - 1][c2_n] == '\0');
	}
	else if (r2_n == r1_n + 1 && abs(c1_n - c2_n) == 1) {
		if (isOppositeColorsSquares(src, r1_n, c1_n, r2_n, c2_n))
			return true;
	}
	return false;
}

//findes the square of the current player's king
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

//given two squares representing a move, checks if the given move is legal
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

//checks if a square is currently threatened by the opposing player to the current player
bool isSquareThreatened(ChessGame* src, int r1_n, int c1_n) {
	int i = 0, j = 0;
	bool shouldCheck = false;
	char curSquare = '\0';
	for (i = 0; i < 8; i++) {
		for (j = 0; j < 8; j++) {
			curSquare = src->gameBoard[i][j];
			if (curSquare != '\0') {
				shouldCheck = (src->currentPlayer == WHITE_PLAYER) ? isupper(curSquare) : !isupper(curSquare);
				if (shouldCheck && isLegalMove(src, i, j, r1_n, c1_n)){
					return true;
				}
			}
		}
	}
	return false;
}

//checks if a square is currently threatened by a specific player
bool isSquareThreatenedByColor(ChessGame* src, int r1_n, int c1_n,char threateningPlayer) {
	int i = 0, j = 0;
	bool shouldCheck = false;
	char curSquare = '\0';
	for (i = 0; i < 8; i++) {
		for (j = 0; j < 8; j++) {
			curSquare = src->gameBoard[i][j];
			if (curSquare != '\0') {
				shouldCheck = (threateningPlayer == WHITE_PLAYER) ? !isupper(curSquare) : isupper(curSquare);
				if (shouldCheck && isLegalMove(src, i, j, r1_n, c1_n)) {
					return true;
				}
			}
		}
	}
	return false;
}

//checks if the current player's king is threatened
bool isCurKingThreatened(ChessGame* src) {
	int kingCol = 0, kingRow = 0, res;
	res = findCurPlayerKing(src);
	kingCol = res % 10;
	kingRow = res / 10;
	return isSquareThreatened(src, kingRow, kingCol);
}


//executes a move (updates the piece's location)
void executeMove(ChessGame* src, int r1_n, int c1_n, int r2_n, int c2_n) {
	src->gameBoard[r2_n][c2_n] = src->gameBoard[r1_n][c1_n];
	src->gameBoard[r1_n][c1_n] = '\0';
}

//checks if the king is checked after a specific move is executed
bool isKingCheckedAfterMove(ChessGame* src, int r1_n, int c1_n, int r2_n, int c2_n) {
	bool KingStillChecked = false;
	ChessGame* gameCopy= ChessGameCopy(src);
	executeMove(gameCopy, r1_n, c1_n, r2_n, c2_n);
	KingStillChecked = isCurKingThreatened(gameCopy);
	ChessGameDestroy(gameCopy);
	return KingStillChecked;
}

//checks if the current player is checkmated
bool isCheckmate(ChessGame* src) {
	int i, j, k, l;
	bool shouldCheck = false;
	char curSquare = '\0';
	for (i = 0; i < 8; i++) {
		for (j = 0; j < 8; j++) {
			curSquare = src->gameBoard[i][j];
			if (curSquare != '\0') {
				shouldCheck = (src->currentPlayer == WHITE_PLAYER) ? !isupper(curSquare) : isupper(curSquare);
				if (shouldCheck) {
					for (k = 0; k < 8; k++) {
						for (l = 0; l < 8; l++) {
							if (isLegalMove(src, i, j, k, l) && !isKingCheckedAfterMove(src, i, j, k, l))
								return false;
						}
					}
				}
			}
		}
	}
	return true;
}

// adds a move to the ChessGame's history
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

//Checks if the current player is checked, checkmated or there is a draw
void CheckCheckmateDrawTest(ChessGame* src) {
	src->checked = '\0';
	if (isCurKingThreatened(src)) {
		src->checked = src->currentPlayer;
		if (isCheckmate(src))
			src->checkmated = src->currentPlayer;
	}
	else {
		if (isCheckmate(src))
			src->draw = true;
	}
}

//Tries to do a given move, returns a CHESS_GAME_MESSAGE indicating if the move was done successfully.
CHESS_GAME_MESSAGE ChessGameSetMove(ChessGame* src, char r1, char c1, char r2, char c2) {
	int r1_n = 8-(r1 - '0'), r2_n = 8 - (r2 - '0'), c1_n = c1 - 'A', c2_n = c2 - 'A';
	if (src == NULL || !isValidSquare(r1_n, c1_n) || !isValidSquare(r2_n, c2_n))
		return INVALID_POSITION;
	if (!isCurPlayerPiece(src,r1_n,c1_n))
		return NOT_PLAYER_PIECE;
	if (r1_n == r2_n && c1_n == c2_n)
		return ILLEGAL_MOVE;
	if (!isLegalMove(src, r1_n, c1_n, r2_n, c2_n))
		return ILLEGAL_MOVE;
	if (isKingCheckedAfterMove(src, r1_n, c1_n, r2_n, c2_n)) {
		if (src->checked == src->currentPlayer)
			return KING_STILL_THREATENED;
		else
			return KING_NOW_THREATENED;
	}
	addMoveToHistory(src, r1_n, c1_n, r2_n, c2_n);
	executeMove(src, r1_n, c1_n, r2_n, c2_n);
	ChessGameSwitchPlayer(src);
	CheckCheckmateDrawTest(src);
	return SUCCESS;
}

//Checks if a piece is threatened after a move it does
bool isThreatenedAfterMove(ChessGame* src, int r1_n, int c1_n, int r2_n, int c2_n) {
	ChessGame* gameCopy;
	bool squareThreatened = false, isCurrentPlayerPiece =true;
	gameCopy = ChessGameCopy(src);
	if (gameCopy == NULL) {
		ChessGameDestroy(src);
		printf("Error: malloc has failed\n");
		exit(0);
	}
	isCurrentPlayerPiece = isCurPlayerPiece(gameCopy, r1_n, c1_n);
	executeMove(gameCopy, r1_n, c1_n, r2_n, c2_n);
	if (!isCurrentPlayerPiece)
		ChessGameSwitchPlayer(gameCopy);
	squareThreatened = isSquareThreatened(gameCopy, r2_n, c2_n);
	ChessGameDestroy(gameCopy);
	return squareThreatened;
}

//Gets and prints all the legal moves of a piece in a given square
CHESS_GAME_MESSAGE ChessGameGetMoves(ChessGame* src, char r1, char c1) {
	int i = 0, j = 0, r1_n = 8 - (r1 - '0'), c1_n = c1 - 'A';
	ChessGame* newGameCopy;
	if (src == NULL || !isValidSquare(r1_n, c1_n))
		return INVALID_POSITION;
	if (src->gameBoard[r1_n][c1_n] == '\0')
		return NO_PIECE_IN_SQUARE;
	newGameCopy = ChessGameCopy(src);
	if (newGameCopy==NULL)
		return NULL_SRC;
	if (!isCurPlayerPiece(src, r1_n, c1_n))
		ChessGameSwitchPlayer(newGameCopy);
	for (i = 0; i < 8; i++) {
		for (j = 0; j < 8; j++) {
			if (isLegalMove(newGameCopy, r1_n, c1_n, i, j) && !isKingCheckedAfterMove(newGameCopy, r1_n, c1_n, i, j)) {
				printf("<%c,%c>", '8' - i, 'A' + j);
				if (isThreatenedAfterMove(newGameCopy, r1_n, c1_n, i, j))
						printf("*");
				if (isOppositeColorsSquares(newGameCopy,i,j, r1_n, c1_n))
					printf("^");
				printf("\n");
			}
		}
	}
	ChessGameDestroy(newGameCopy);
	return SUCCESS;
}

//revokes the last move that was made - returns a CHESS_GAME_MESSAGE indicating if execution was successful or not
CHESS_GAME_MESSAGE ChessGameUndoPrevMove(ChessGame* src, bool shouldPrint) {
	char lastPiece = spArrayListGetLast(src->history);
	int i=0,size = src->history->actualSize, dstCol = 0, dstRow = 0, srcCol = 0, srcRow = 0;
	if (size == 0)
		return NO_HISTORY;
	dstCol = spArrayListGetAt(src->history, size - 2);
	dstRow = spArrayListGetAt(src->history, size - 3);
	srcCol = spArrayListGetAt(src->history, size - 4);
	srcRow = spArrayListGetAt(src->history, size - 5);
	src->gameBoard[srcRow][srcCol] = src->gameBoard[dstRow][dstCol];
	src->gameBoard[dstRow][dstCol] = lastPiece;
	ChessGameSwitchPlayer(src);
	if (shouldPrint) {
		if (src->currentPlayer == WHITE_PLAYER)
			printf("Undo move for black player: <%c,%c> -> <%c,%c>\n", '8' - dstRow, 'A' + dstCol, '8' - srcRow, 'A' + srcCol);
		else
			printf("Undo move for white player: <%c,%c> -> <%c,%c>\n", '8' - dstRow, 'A' + dstCol, '8' - srcRow, 'A' + srcCol);
	}
	for (i = 0; i < 5; i++)
		spArrayListRemoveLast(src->history);
	return SUCCESS;
}

//prints the current state of the given game's board
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

//given a ChessGame, this function gets it's difficulty (an integer) and returns it's name
char* getDifficultyString(ChessGame* src) {
	switch (src->difficulty)
	{
	case 1:
		return "amateur";
		break;
	case 2:
		return "easy";
		break;
	case 3:
		return "moderate";
		break;
	case 4:
		return "hard";
		break;
	case 5:
		return "expert";
		break;
	}
	return "amateur";
}

//given a difficulty (a string) prints it's suitable number
int getDifficultyNum(char* diff) {
	if (strcmp(diff, "DIFFICULTY: amateur\n") == 0)
		return 1;
	if (strcmp(diff, "DIFFICULTY: easy\n") == 0)
		return 2;
	if (strcmp(diff, "DIFFICULTY: moderate\n") == 0)
		return 3;
	if (strcmp(diff, "DIFFICULTY: hard\n") == 0)
		return 4;
	return 5;
}

//given a ChessGame, this function gets it's difficulty (an integer) and prints it's name
void printDifficulty(ChessGame* src) {
	switch (src->difficulty)
	{
	case 1:
		printf("amateur");
		break;
	case 2:
		printf("easy");
		break;
	case 3:
		printf("moderate");
		break;
	case 4:
		printf("hard");
		break;
	case 5:
		printf("expert");
		break;
	}
}

//save the current game's board and settings in a specified path
CHESS_GAME_MESSAGE ChessGameSave(ChessGame* src, char* path) {
	int i = 0, j = 0;
	FILE* myFile;
	char* curPlayerColor;
	if (path[0] == '\0')
		return NULL_PATH;
	if (src == NULL)
		return NULL_SRC;
	myFile = fopen(path, "w+");
	if (myFile == NULL)
		return FILE_CREATE_FAILED;
	curPlayerColor = src->currentPlayer == WHITE_PLAYER ? "white" : "black";
	fprintf(myFile, "%s\nSETTINGS:\nGAME_MODE: %d-player\n", curPlayerColor, src->gameMode);
	if (src->gameMode == 1) {
		fprintf(myFile, "DIFFICULTY: %s\n", getDifficultyString(src));
		if (src->userColor == 0)
			fprintf(myFile, "USER_COLOR: black\n");
		else
			fprintf(myFile, "USER_COLOR: white\n");
	}
	for (i = 0; i < 8; i++) {
		fprintf(myFile, "%d|", 8 - i);
		for (j = 0; j < 8; j++) {
			if (src->gameBoard[i][j] != '\0')
				fprintf(myFile, " %c", src->gameBoard[i][j]);
			else
				fprintf(myFile, " _");
		}
		fprintf(myFile, " |\n");
	}
	fprintf(myFile, "  -----------------\n   A B C D E F G H\n");
	fclose(myFile);
	return SUCCESS;
}

//loads a previous's game board and settings from a specified path
CHESS_GAME_MESSAGE ChessGameLoad(ChessGame* src, char* path) {
	int i = 0, j = 0;
	char curLine[100];
	FILE* myFile;
	if (src == NULL)
		return NULL_SRC;
	if (path[0] == '\0')
		return NULL_PATH;
	myFile = fopen(path, "r+");
	if (myFile == NULL)
		return FILE_CREATE_FAILED;
	fgets(curLine, 100, myFile);
	src->currentPlayer = (strcmp(curLine, "white\n") == 0) ? WHITE_PLAYER : BLACK_PLAYER;
	fgets(curLine, 100, myFile);
	fgets(curLine, 100, myFile);
	src->gameMode = (strcmp(curLine, "GAME_MODE: 1-player\n") == 0) ? 1 : 2;
	if (src->gameMode == 1) {
		fgets(curLine, 100, myFile);
		src->difficulty = getDifficultyNum(curLine);
		fgets(curLine, 100, myFile);
		src->userColor = (strcmp(curLine, "USER_COLOR: white\n") == 0) ? 1 : 0;
	}
	for (i = 0; i < 8; i++) {
		fgets(curLine, 100, myFile);
		for (j = 0; j < 8; j++) {
			if (curLine[3 + 2 * j] != '_')
				src->gameBoard[i][j] = curLine[3 + 2 * j];
			else
				src->gameBoard[i][j] = '\0';
		}
	}
	CheckCheckmateDrawTest(src);
	fclose(myFile);
	return SUCCESS;
}

//prints a given game's settings
void chessGamePrintSettings(ChessGame* src) {
	printf("SETTINGS:\n");
	printf("GAME_MODE: %d-player\n", src->gameMode);
	if (src->gameMode == 1) {
		printf("DIFFICULTY: ");
		printDifficulty(src);
		printf("\n");
		if (src->userColor == 0)
			printf("USER_COLOR: black\n");
		else
			printf("USER_COLOR: white\n");
	}
}

//destroys and quits the current game
void quit(ChessGame* src) {
	ChessGameDestroy(src);
	printf("Exiting...\n");
	exit(0);
}

//resets the board and history of the current game
void chessGameReset(ChessGame* g) {
	int i = 0, j = 0, size = g->history->actualSize;
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
	for (i = 0; i < size; i++) {
		spArrayListRemoveLast(g->history);
	}
}

//resets a game's settings to the default
void chessGameDefault(ChessGame* g) {
	g->gameMode = 1;
	g->difficulty = 2;
	g->userColor = 1;
	chessGameReset(g);
}




