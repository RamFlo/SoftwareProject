#ifndef SPMINIMAXNODE_H_
#define SPMINIMAXNODE_H_
#include "SPFIARGame.h"
#define CHILDREN SP_FIAR_GAME_N_COLUMNS
const int weights[] = { -5,-2,-1,0,1,2,5 };

//Put all decleartions and constants here

typedef struct Node_t {
	struct Node_t* children;
	SPFiarGame* gameStatus;
	bool isLeaf;
} Node;

Node* createNode(SPFiarGame* gameStatus);

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

//int* calcChildrenMax(Node* node, int depth, char player);

//int* calcChildrenMin(Node* node, int depth, char player);

#endif
