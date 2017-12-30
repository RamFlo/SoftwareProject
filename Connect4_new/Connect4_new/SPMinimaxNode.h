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
	TIE,
	PLAYER_1_WIN,
	PLAYER_2_WIN,
	REGULAR,
} nodeType;

typedef struct Node_t {
	struct Node_t* children;
	SPFiarGame* gameStatus;
	bool isLeaf;
	nodeType type;
	int childIndex;
	int value;
} Node;

Node* createRoot();
int scoringFunction(Node* node, char player);
int colsScoring(Node* node, char player);
int col4score(Node* node, int col, int row, char player);
int rowScoring(Node* node, char player);
int rowSpanValue(Node* node, int colNum, int rowNum, char player);
int diagUpScoring(Node* node, char player);
int diagUpSpanValue(Node* node, int colNum, int rowNum, char player);
int diagDownScoring(Node* node, char player);
int diagDownSpanValue(Node* node, int colNum, int rowNum, char player);
int discValue(Node* node, int colNum, int rowNum, char player);
Node* calcChildrenMax(Node* node,char actualPlayer);
Node* calcChildrenMin(Node* node,char actualPlayer);


#endif
