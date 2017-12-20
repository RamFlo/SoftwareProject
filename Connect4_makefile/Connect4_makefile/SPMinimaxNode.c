#ifndef SPMINIMAXNODE_H_
#define SPMINIMAXNODE_H_
#include "SPFIARGame.h"
#include <stdlib.h>
#define CHILDREN SP_FIAR_GAME_N_COLUMNS
const int weights[] = {-5,-2,-1,0,1,2,5};

//Put all decleartions and constants here

typedef struct Node {
	Node* children;
	SPFiarGame* gameStatus;
	bool isLeaf;
} Node;

Node* createNode(SPFiarGame* gameStatus) {
	Node *newNode, *children;
	bool isLeaf = true;
	newNode = (Node*)calloc(sizeof(Node));
	if (newNode == NULL)
		return NULL;
	newNode->gameStatus = gameStatus;
	children = (Node*)calloc(CHILDREN,sizeof(Node));
	if (children == NULL) {
		free(newNode);
		return NULL;
	}	
	newNode->children = children;
	newNode->isLeaf = isLeaf;
	return newNode;
}

int scoringFunction(Node* node, char player) {
	return colsScoring(node, player) + rowScoring(node, player) + diagScoring(node, player);
}

int colsScoring(Node* node, char player) {
	int weight = 0, lastRow = SP_FIAR_GAME_N_ROWS - SP_FIAR_GAME_SPAN + 1,curRow=0,curCol=0, lastCol= SP_FIAR_GAME_N_COLUMNS;
	for (curCol; curCol < lastCol; curCol++) {
		curRow = 0;
		for (curRow; curRow <= lastRow; curRow++) {
			weight += col4score(node, curCol, curRow, player);
		}
	}
	return weight;
}

int col4score(Node* node, int col, int row, char player) {
	int curRow = row, sum = 0, index = 0, iterNum = SP_FIAR_GAME_SPAN, i=0;
	for (i = 0; i < SP_FIAR_GAME_SPAN; i++) {
		if (node->gameStatus->gameBoard[curRow + i][col] == player && node->gameStatus->tops[col] > curRow)
			sum++;
		else
			if (node->gameStatus->tops[col] > curRow)
				sum--;
		return weights[sum+3];
	}
}


#endif
