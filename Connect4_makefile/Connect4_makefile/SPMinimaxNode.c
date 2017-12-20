#ifndef SPMINIMAXNODE_H_
#define SPMINIMAXNODE_H_
#include "SPFIARGame.h"
#include <stdlib.h>
#include <limits.h>
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
	newNode = (Node*)calloc(1,sizeof(Node));
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
int rowScoring(Node* node, char player) {
	int i = 0, j = 0, weight = 0, curValue = 0, rowSpans = SP_FIAR_GAME_N_COLUMNS / SP_FIAR_GAME_SPAN + SP_FIAR_GAME_N_COLUMNS % SP_FIAR_GAME_SPAN;
	for (i = 0; i < SP_FIAR_GAME_N_ROWS; i++)
	{
		curValue = rowSpanValue(node, 0, i, player);
		weight += weights[curValue + 3];
		for (j = 1; j < rowSpans; j++) {
			curValue = curValue - discValue(node, j - 1, i, player) + discValue(node, j + SP_FIAR_GAME_SPAN - 1, i, player);
			weight += weights[curValue + 3];
		}
	}
	return weight;
}

int rowSpanValue(Node* node, int colNum, int rowNum, char player) {
	int i = 0, sum = 0;
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
			weight += weights[3 + diagUpSpanValue(node, j, i, player)];
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
	for (i = SP_FIAR_GAME_N_ROWS - 1; i >= SP_FIAR_GAME_N_ROWS - diagsInCol; i--) {
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

int* calcChildrenMax(Node* node, int depth, char player) {
	int maxIndex = -1, maxValue = INT_MIN, i = 0, *curCalc;
	char winner = spFiarCheckWinner(node->gameStatus);
	Node* point = node->children;
	if (winner != NULL) {
		if (winner == SP_FIAR_GAME_TIE_SYMBOL)
			return 0;
		else if (winner == player)
			return INT_MAX;
		return INT_MIN;
	}
	if (depth == 0)
		return scoringFunction(node, player);

	for (i; i < SP_FIAR_GAME_N_COLUMNS;i++) {
		if (spFiarGameIsValidMove(node->gameStatus, i)) {
			*node->children = *createNode(spFiarGameSetMove(spFiarGameCopy(node->gameStatus), i));
			curCalc = calcChildrenMin(node->children+i, depth - 1, getOtherPlayer(player));
			if (curCalc[0] > maxValue) {
				maxIndex = i;
				maxValue = curCalc[0];
			}
		}
		point++;
	}
	//call for destroy
	return curCalc;
}

int* calcChildrenMin(Node* node, int depth, char player) {
	int minIndex = -1, minValue = INT_MAX, i = 0, *curCalc;
	char winner = spFiarCheckWinner(node->gameStatus);
	Node* point = node->children;
	if (winner != NULL) {
		if (winner == SP_FIAR_GAME_TIE_SYMBOL)
			return 0;
		else if (winner == player)
			return INT_MAX;
		return INT_MIN;
	}
	if (depth == 0)
		return scoringFunction(node, player);

	for (i; i < SP_FIAR_GAME_N_COLUMNS; i++) {
		if (spFiarGameIsValidMove(node->gameStatus, i)) {
			*node->children = *createNode(spFiarGameSetMove(spFiarGameCopy(node->gameStatus), i));
			curCalc = calcChildrenMax(node->children + i, depth - 1, getOtherPlayer(player));
			if (curCalc[0] < minValue) {
				minIndex = i;
				minValue = curCalc[0];
			}
		}
		point++;
	}
	//call for destroy
	return curCalc;
}


#endif
