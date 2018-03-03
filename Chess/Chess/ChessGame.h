#ifndef SPFIARGAME_H_
#define SPFIARGAME_H_
#include <stdbool.h>
#include "ArrayList.h"

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
	char checked;	//0-black is checked, 1-white is checked, 2-no check
} ChessGame;

/**
* Type used for returning error codes from game functions
*/
typedef enum chess_game_message_t {
	ILLEGAL_MOVE,
	INVALID_POSITION,
	NOT_PLAYER_PIECE,
	NO_HISTORY,
	SUCCESS,
	KING_STILL_THREATENED,
	KING_NOW_THREATENED,
	//You may add any message you like
} CHESS_GAME_MESSAGE;

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
ChessGame* ChessGameCreate(int historySize);

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
SPFiarGame* spFiarGameCopy(SPFiarGame* src);

/**
* Frees all memory allocation associated with a given game. If src==NULL
* the function does nothing.
*
* @param src - the source game
*/
void spFiarGameDestroy(SPFiarGame* src);

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
SP_FIAR_GAME_MESSAGE spFiarGameSetMove(SPFiarGame* src, int col);

/**
* Checks if a disk can be put in the specified column.
*
* @param src - The source game
* @param col - The specified column
* @return
* true  - if the a disc can be put in the target column
* false - otherwise.
*/
bool spFiarGameIsValidMove(SPFiarGame* src, int col);

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
SP_FIAR_GAME_MESSAGE spFiarGameUndoPrevMove(SPFiarGame* src);

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
SP_FIAR_GAME_MESSAGE spFiarGamePrintBoard(SPFiarGame* src);

/**
* Returns the current player of the specified game.
* @param src - the source game
* @return
* SP_FIAR_GAME_PLAYER_1_SYMBOL - if it's player one's turn
* SP_FIAR_GAME_PLAYER_2_SYMBOL - if it's player two's turn
* SP_FIAR_GAME_EMPTY_ENTRY     - otherwise
*/
char spFiarGameGetCurrentPlayer(SPFiarGame* src);

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
char spFiarCheckWinner(SPFiarGame* src);

#endif
