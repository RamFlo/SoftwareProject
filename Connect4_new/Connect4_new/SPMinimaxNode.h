#ifndef SPMINIMAXNODE_H_
#define SPMINIMAXNODE_H_
#include "SPFIARGame.h"
#include <stdlib.h>
#include <limits.h>
#define CHILDREN SP_FIAR_GAME_N_COLUMNS
//const int weights[] = { -5,-2,-1,0,1,2,5 };
//extern const int weights[7]= { -5,-2,-1,0,1,2,5 };

//Put all decleartions and constants here
typedef enum nodeType_t {
	REGULAR,
	PLAYER_1_WIN,
	PLAYER_2_WIN,
	TIE,
} nodeType;

typedef struct Node_t {
	struct Node_t* children;
	SPFiarGame* gameStatus;
	bool isLeaf;
	nodeType type;
	int childIndex;
	int value;
	bool isValidMove;
} Node;
/*
* This functions creates a Node (a root to a later created Minimax tree).
* It takes no parameters, as none are needed.
* The function initializes the new node's parameters,
* for example, isLeaf - initialized to false as we do not consider the root to be a leaf generally.
* it also allocates the needed memory space for the object.
* @return
* a pointer to the newly created Node.
*/
Node* createRoot();
/*
* Given a char representing a player, this function returns the char representing the opposite player.
* @param player - a char representing a player
* @return
* The char representing the other player.
*/
int getWeight(int index);
/*
* Given a char representing a player, this function returns the char representing the opposite player.
* @param player - a char representing a player
* @return
* The char representing the other player.
*/
char getOtherPlayer(char player);

/*
* Given a node containing the current game status and a char representing the "positive player",
* this function returns a score representing who's leading in the current game status.
* Namely - the higher the score it returns, the better the position for the given player, and vice versa.
* @param node - a Node containing the current game status.
* @param player - a character representing the "positive player"
* @return
* a numeric value, representing an analysis of the game status. The higher it is, the better for the given player.
*/
int scoringFunction(Node* node, char player);

/*
* given a node (containing the current game status), and a char representing the "positive player",
* calculates the total score of all the column spans.
* @param node - a node containing the current game status.
* @param player - the "positive player" for the scoring function
* @return
* the accomulative score of all the column spans, according to the "positive player" and the current game status.
*/
int colsScoring(Node* node, char player);

/*
* This function, given a starting location (represented by col and row), calculates the value of
* a span in the given column (col), starting in the given row (row). Then, it returns the relevant value from the weight vector.
* @param node - a node containing the current game status
* @param col - the column in which the relevant span is
* @param row - the row from which the relevant span starts
* @param player - the "positive player" for the scoring function
* @return
* The value of the above mentioned span, according to the weight vector (by using getWeight).
*/
int col4score(Node* node, int col, int row, char player);

/*
* given a node (containing the current game status), and a char representing the "positive player",
* calculates the total score of all the row spans.
* @param node - a node containing the current game status.
* @param player - the "positive player" for the scoring function
* @return
* the accomulative score of all the row spans, according to the "positive player" and the current game status.
*/
int rowScoring(Node* node, char player);

/*
* This function, given a starting location (represented by col and row), calculates the value of
* a span in the given row (row), starting in the given column (col).
* @param node - a node containing the current game status
* @param row - the row in which the relevant span is
* @param col - the column from which the relevant span starts
* @param player - the "positive player" for the scoring function
* @return
* The value of the above mentioned span.
*/
int rowSpanValue(Node* node, int colNum, int rowNum, char player);

/*
* given a node (containing the current game status), and a char representing the "positive player",
* calculates the total score of all the upward sloping diagonals.
* @param node - a node containing the current game status.
* @param player - the "positive player" for the scoring function
* @return
* the accomulative score of all the upward sloping diagonals spans, according to the "positive player" and the current game status.
*/
int diagUpScoring(Node* node, char player);

/*
* This function, given a starting location (represented by col and row), calculates the value of
* the upword sloping diagonal (of length equal to the span), starting in the given index (col and row).
* @param node - a node containing the current game status
* @param row - the starting point's row
* @param col - the starting point's col
* @param player - the "positive player" for the scoring function
* @return
* The value of the above mentioned span).
*/
int diagUpSpanValue(Node* node, int colNum, int rowNum, char player);

/*
* given a node (containing the current game status), and a char representing the "positive player",
* calculates the total score of all the upward sloping diagonals.
* @param node - a node containing the current game status.
* @param player - the "positive player" for the scoring function
* @return
* the accomulative score of all the upward sloping diagonals spans, according to the "positive player" and the current game status.
*/
int diagDownScoring(Node* node, char player);

/*
* This function, given a starting location (represented by col and row), calculates the value of
* the downward sloping diagonal (of length equal to the span), starting in the given index (col and row).
* @param node - a node containing the current game status
* @param row - the starting point's row
* @param col - the starting point's col
* @param player - the "positive player" for the scoring function
* @return
* The value of the above mentioned span).
*/
int diagDownSpanValue(Node* node, int colNum, int rowNum, char player);

/*
* Given a node, row and column numbers and a player,
* this function returns how the scoring function should regard the disc in the given row and column.
* Namely, should it be regarded as a -1 or a +1 when calculating a span's value.
* @param node - a node containing a game status.
* @param colNum - the column index (0 based) of the disc we would like to check.
* @param rowNum - the row index (0 based) of the disc we would like to check.
* @param player - the player who is considered the "Positive player",
* Namely - the player who the scoring function should regard as the positive.
* @return
* if the disc in the given column and row (in the node's game status) belongs to the "positive player", we return 1.
* Otherwise, we return -1.
*/
int discValue(Node* node, int colNum, int rowNum, char player);

/*
* Given a node containing the game status on which we want to perform calculations,
* and a character representing the "positive player" - namely, the player who initiated the calculation,
* this function finds the given node's child that holds the maximal value - and returns it.
* By doing so, it simulates the initiating player's move on the given game status.
* When it reaches a leaf, if the leaf is a terminal state (a win for the positive/negative player or a draw),
* the function returns the suitable value. If it's a non terminal leaf - it calls the scoring function.
* If the Node is not a leaf, it returns the node representing the max value move.
* @param node - a Node containing the current game status.
* @param actualPlayer - contains the player who initiated the calculations.
* @return
* the maximal value of the function, as described above.
*/
Node* calcChildrenMax(Node* node,char actualPlayer);

/*
* Given a node containing the game status on which we want to perform calculations,
* and a character representing the "positive player" - namely, the player who initiated the calculation,
* this function finds the given node's child that holds the minimal value - and returns it.
* By doing so, it simulates the non-initiating player's move on the given game status.
* When it reaches a leaf, if the leaf is a terminal state (a win for the positive/negative player or a draw),
* the function returns the suitable value. If it's a non terminal leaf - it calls the scoring function.
* If the Node is not a leaf, it returns the node representing the min value move.
* @param node - a Node containing the current game status.
* @param actualPlayer - contains the player who initiated the calculations.
* @return
* the minimal value of the function, as described above.
*/
Node* calcChildrenMin(Node* node,char actualPlayer);


#endif
