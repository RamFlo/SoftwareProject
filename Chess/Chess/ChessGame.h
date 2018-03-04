#ifndef SPFIARGAME_H_
#define SPFIARGAME_H_
#include <stdbool.h>
#include "ArrayList.h"



//Definitions
#define WHITE_PLAYER 'X'
#define BLACK_PLAYER 'O'
#define WHITE_PAWN 'm'
#define WHITE_QUEEN 'q'
#define WHITE_KING 'k'
#define WHITE_BISHOP 'b'
#define WHITE_ROOK 'r'
#define WHITE_KNIGHT 'n'
#define BLACK_PAWN 'M'
#define BLACK_QUEEN 'Q'
#define BLACK_KING 'K'
#define BLACK_BISHOP 'B'
#define BLACK_ROOK 'R'
#define BLACK_KNIGHT 'N'
#define TIE_SYMBOL '-'

typedef struct chess_game_t {
	char gameBoard[8][8];
	char currentPlayer;
	SPArrayList* history;
	char checked;
	char checkmated;
	bool draw;
	int gameMode, difficulty, userColor;
	int bestMoveSrcCol, bestMoveSrcRow, bestMoveDstCol, bestMoveDstRow;
} ChessGame;

/**
* Type used for returning error codes from game functions
*/
typedef enum chess_game_message_t {
	ILLEGAL_MOVE,
	INVALID_POSITION,
	NOT_PLAYER_PIECE,
	NO_PIECE_IN_SQUARE,
	NO_HISTORY,
	SUCCESS,
	KING_STILL_THREATENED,
	KING_NOW_THREATENED,
	NULL_SRC,
	FILE_CREATE_FAILED,
} CHESS_GAME_MESSAGE;


ChessGame* ChessGameCreate(int historySize);


SPFiarGame* spFiarGameCopy(SPFiarGame* src);


void spFiarGameDestroy(SPFiarGame* src);


SP_FIAR_GAME_MESSAGE spFiarGameSetMove(SPFiarGame* src, int col);


bool spFiarGameIsValidMove(SPFiarGame* src, int col);


SP_FIAR_GAME_MESSAGE spFiarGameUndoPrevMove(SPFiarGame* src);


SP_FIAR_GAME_MESSAGE spFiarGamePrintBoard(SPFiarGame* src);


char spFiarGameGetCurrentPlayer(SPFiarGame* src);


char spFiarCheckWinner(SPFiarGame* src);


bool isCurPlayerPiece(int r1_n, int c1_n, ChessGame* src);

ChessGame* ChessGameCopy(ChessGame* src);

CHESS_GAME_MESSAGE ChessGameSetMove(ChessGame* src, char r1, char c1, char r2, char c2);

void printDifficulty(ChessGame* src);

CHESS_GAME_MESSAGE ChessGameLoad(ChessGame* src, char* path);

void ChessGameDestroy(ChessGame* src);

void chessGameDefault(ChessGame* g);

void chessGamePrintSettings(ChessGame* src);
#endif
