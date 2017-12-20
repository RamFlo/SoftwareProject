#ifndef SPMINIMAXNODE_H_
#define SPMINIMAXNODE_H_
#include "SPFIARGame.h"
#define CHILDREN SP_FIAR_GAME_N_COLUMNS

//Put all decleartions and constants here

typedef struct Node {
	Node* children;
	SPFiarGame* gameStatus;
	bool isLeaf;
} Node;

Node* createNode(SPFiarGame* gameStatus) {
	Node *newNode, *children;
	bool isLeaf = true;
	children = (Node*)calloc(CHILDREN*sizeof(Node));
	newNode = (Node*)calloc(sizeof(Node));
	newNode->children = children;
	newNode->isLeaf = isLeaf;
}

int rowScoring(Node* node,char player) {
	int i=0,j=0,weight=0, curValue =0,rowSpans = SP_FIAR_GAME_N_COLUMNS / SP_FIAR_GAME_SPAN + SP_FIAR_GAME_N_COLUMNS % SP_FIAR_GAME_SPAN;
	for (i = 0; i < SP_FIAR_GAME_N_ROWS; i++)
	 {
		curValue = rowSpanValue(node, 0, i, player);
		weight += weights[curValue +3];
		for (j = 1; j < rowSpans; j++) {
			curValue = curValue - discValue(node, j - 1, i, player) + discValue(node, j + SP_FIAR_GAME_SPAN - 1, i, player);
			weight += weights[curValue + 3];
		}
	}
	return weight;
}

int rowSpanValue(Node* node, int colNum, int rowNum, char player) {
	int i = 0,sum=0;
	for (i = colNum; i < colNum + SP_FIAR_GAME_SPAN; i++)
		sum += discValue(node, i, rowNum, player);
	return sum;
}


int diagUpScoring(Node* node, char player) {
	int diagsInCol = SP_FIAR_GAME_N_ROWS / SP_FIAR_GAME_SPAN + SP_FIAR_GAME_N_ROWS % SP_FIAR_GAME_SPAN;
	int diagsInRow = SP_FIAR_GAME_N_COLUMNS / SP_FIAR_GAME_SPAN + SP_FIAR_GAME_N_COLUMNS % SP_FIAR_GAME_SPAN;
	int i = 0, j = 0, weight = 0;;
	for (i = 0; i < diagsInCol; i++) {
		for (j = 0; j < diagsInRow; j++)
			weight += weights[3+diagUpSpanValue(node, j, i, player)];
	}
}

int diagUpSpanValue(Node* node, int colNum, int rowNum, char player) {
	int i = 0, sum = 0;
	for (i = colNum; i < colNum + SP_FIAR_GAME_SPAN; i++)
		sum += discValue(node, i, rowNum + i - colNum, player);
	return sum;
}

int diagDownScoring(Node* node, char player) {
	int diagsInCol = SP_FIAR_GAME_N_ROWS / SP_FIAR_GAME_SPAN + SP_FIAR_GAME_N_ROWS % SP_FIAR_GAME_SPAN;
	int diagsInRow = SP_FIAR_GAME_N_COLUMNS / SP_FIAR_GAME_SPAN + SP_FIAR_GAME_N_COLUMNS % SP_FIAR_GAME_SPAN;
	int i = 0, j = 0, weight = 0;;
	for (i = SP_FIAR_GAME_N_ROWS-1; i >=SP_FIAR_GAME_N_ROWS- diagsInCol; i--) {
		for (j = 0; j < diagsInRow; j++)
			weight += weights[3 + diagUpSpanValue(node, j, i, player)];
	}
}

int diagDownSpanValue(Node* node, int colNum, int rowNum, char player) {
	int i = 0, sum = 0;
	for (i = colNum; i < colNum + SP_FIAR_GAME_SPAN; i++)
		sum += discValue(node, i, rowNum - i + colNum, player);
	return sum;
}


int discValue(Node* node, int colNum, int rowNum, char player) {
	if (node->gameStatus->tops[colNum] > rowNum) {
		if (player == node->gameStatus->gameBoard[rowNum][colNum])
			return 1;
		else
			return -1;
	}
	return 0;
}



#endif
