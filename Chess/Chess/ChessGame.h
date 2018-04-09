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
	NULL_PATH,
} CHESS_GAME_MESSAGE;

//creates a new ChessGame, initializes the board and settings and returns a pointer to the newly created ChessGame
ChessGame* ChessGameCreate(int historySize);

//checks if a square contains a piece of the current player
bool isCurPlayerPiece(ChessGame* src,int r1_n, int c1_n);

//copies a ChessGame, copiesthe board and settings and returns a pointer to the newly created ChessGame
ChessGame* ChessGameCopy(ChessGame* src);

//Tries to do a given move, returns a CHESS_GAME_MESSAGE indicating if the move was done successfully.
CHESS_GAME_MESSAGE ChessGameSetMove(ChessGame* src, char r1, char c1, char r2, char c2);

//given a ChessGame, this function gets it's difficulty (an integer) and prints it's name
void printDifficulty(ChessGame* src);

//loads a previous's game board and settings from a specified path
CHESS_GAME_MESSAGE ChessGameLoad(ChessGame* src, char* path);

//destroys a given ChessGame
void ChessGameDestroy(ChessGame* src);

//resets a game's settings to the default
void chessGameDefault(ChessGame* g);

//prints a given game's settings
void chessGamePrintSettings(ChessGame* src);

//Gets and prints all the legal moves of a piece in a given square
CHESS_GAME_MESSAGE ChessGameGetMoves(ChessGame* src, char r1, char c1);

//checks if a square is currently threatened by a specific player
bool isSquareThreatenedByColor(ChessGame* src, int r1_n, int c1_n, char threateningPlayer);

//given a char representing a piece, prints the piece's name
void printPieceName(char p);

//prints the current state of the given game's board
CHESS_GAME_MESSAGE ChessGamePrintBoard(ChessGame* src);

//save the current game's board and settings in a specified path
CHESS_GAME_MESSAGE ChessGameSave(ChessGame* src, char* path);

//revokes the last move that was made - returns a CHESS_GAME_MESSAGE indicating if execution was successful or not
CHESS_GAME_MESSAGE ChessGameUndoPrevMove(ChessGame* src);

//resets the board and history of the current game
void chessGameReset(ChessGame* g);

//checks if two squares have pieces of opposing colors
bool isOppositeColorsSquares(ChessGame* src, int r1_n, int c1_n, int r2_n, int c2_n);

//given two squares representing a move, checks if the given move is legal
bool isLegalMove(ChessGame* src, int r1_n, int c1_n, int r2_n, int c2_n);

//checks if the king is checked after a specific move is executed
bool isKingCheckedAfterMove(ChessGame* src, int r1_n, int c1_n, int r2_n, int c2_n);

//Checks if a piece is threatened after a move it does
bool isThreatenedAfterMove(ChessGame* src, int r1_n, int c1_n, int r2_n, int c2_n);
#endif
