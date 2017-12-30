#include "SPMinimaxNode.h"
#include <stdio.h>
int getWeight(int index) {
	int w[] = { -5,-2,-1,0,1,2,5 };
	return w[index];
}

char getOtherPlayer(char player) {
	if (player == SP_FIAR_GAME_PLAYER_1_SYMBOL)
		return SP_FIAR_GAME_PLAYER_2_SYMBOL;
	return SP_FIAR_GAME_PLAYER_1_SYMBOL;
}

Node* createNode(SPFiarGame* gameStatus) {
	Node *newNode, *children;
	bool isLeaf = false;
	newNode = (Node*)calloc(1, sizeof(Node));
	if (newNode == NULL) {
		printf("Error: calloc has failed\n");
		return NULL;
	}
	newNode->gameStatus = gameStatus;
	children = (Node*)calloc(CHILDREN, sizeof(Node));
	if (children == NULL) {
		printf("Error: calloc has failed\n");
		free(newNode);
		return NULL;
	}
	newNode->children = children;
	newNode->isLeaf = isLeaf;
	return newNode;
}

void destroyNode(Node* node) {
	if (node != NULL) {
		if (!node->isLeaf)
			free(node->children);
		spFiarGameDestroy(node->gameStatus);
		free(node);
	}
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

int col4score(Node* node, int col, int row, char player) {
	int curRow = row, sum = 0, index = 0, i = 0;
	for (i = 0; i < SP_FIAR_GAME_SPAN; i++) {
		if (node->gameStatus->gameBoard[curRow][col] == player && node->gameStatus->tops[col] > curRow)
			sum++;
		else if (node->gameStatus->tops[col] > curRow)
			sum--;
		curRow++;
	}
	return getWeight(sum + 3);
}

int colsScoring(Node* node, char player) {
	int weight = 0, lastRow = SP_FIAR_GAME_N_ROWS - SP_FIAR_GAME_SPAN + 1, curRow = 0, curCol = 0, lastCol = SP_FIAR_GAME_N_COLUMNS;
	for (curCol; curCol < lastCol; curCol++) {
		curRow = 0;
		for (curRow; curRow < lastRow; curRow++) {
			weight += col4score(node, curCol, curRow, player);
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

int rowScoring(Node* node, char player) {
	int i = 0, j = 0, weight = 0, curValue = 0, rowSpans = SP_FIAR_GAME_N_COLUMNS / SP_FIAR_GAME_SPAN + SP_FIAR_GAME_N_COLUMNS % SP_FIAR_GAME_SPAN;
	for (i = 0; i < SP_FIAR_GAME_N_ROWS; i++)
	{
		curValue = rowSpanValue(node, 0, i, player);
		weight += getWeight(curValue + 3);
		for (j = 1; j < rowSpans; j++) {
			curValue = curValue - discValue(node, j - 1, i, player) + discValue(node, j + SP_FIAR_GAME_SPAN - 1, i, player);
			weight += getWeight(curValue + 3);
		}
	}
	return weight;
}

int diagUpSpanValue(Node* node, int colNum, int rowNum, char player) {
	int i = 0, sum = 0;
	for (i = colNum; i < colNum + SP_FIAR_GAME_SPAN; i++)
		sum += discValue(node, i, rowNum + i - colNum, player);
	return sum;
}

int diagUpScoring(Node* node, char player) {
	int diagsInCol = SP_FIAR_GAME_N_ROWS / SP_FIAR_GAME_SPAN + SP_FIAR_GAME_N_ROWS % SP_FIAR_GAME_SPAN;
	int diagsInRow = SP_FIAR_GAME_N_COLUMNS / SP_FIAR_GAME_SPAN + SP_FIAR_GAME_N_COLUMNS % SP_FIAR_GAME_SPAN;
	int i = 0, j = 0, weight = 0;
	for (i = 0; i < diagsInCol; i++) {
		for (j = 0; j < diagsInRow; j++)
			weight += getWeight(3 + diagUpSpanValue(node, j, i, player));
	}
	return weight;
}

int diagDownSpanValue(Node* node, int colNum, int rowNum, char player) {
	int i = 0, sum = 0;
	for (i = colNum; i < colNum + SP_FIAR_GAME_SPAN; i++)
		sum += discValue(node, i, rowNum - i + colNum, player);
	return sum;
}

int diagDownScoring(Node* node, char player) {
	int diagsInCol = SP_FIAR_GAME_N_ROWS / SP_FIAR_GAME_SPAN + SP_FIAR_GAME_N_ROWS % SP_FIAR_GAME_SPAN;
	int diagsInRow = SP_FIAR_GAME_N_COLUMNS / SP_FIAR_GAME_SPAN + SP_FIAR_GAME_N_COLUMNS % SP_FIAR_GAME_SPAN;
	int i = 0, j = 0, weight = 0;;
	for (i = SP_FIAR_GAME_N_ROWS - 1; i >= SP_FIAR_GAME_N_ROWS - diagsInCol; i--) {
		for (j = 0; j < diagsInRow; j++)
			weight += getWeight(3 + diagDownSpanValue(node, j, i, player));
	}
	return weight;
}

int scoringFunction(Node* node, char player) {
	return colsScoring(node, player) + rowScoring(node, player) + diagUpScoring(node, player) + diagDownScoring(node, player);
}





Node* calcChildrenMax(Node* node,char actualPlayer) {
	int maxIndex = -1, maxValue = INT_MIN, i = 0;
	char player = node->gameStatus->currentPlayer;
	Node* point = node->children + SP_FIAR_GAME_N_COLUMNS-1;
	Node* curNode = NULL;
	node->childIndex = -1;
	if (node->type != REGULAR) {
		if (node->type == TIE) {
			node->value = 0;
			return node;
		}
		else if (node->type == PLAYER_1_WIN) {
			if (actualPlayer == SP_FIAR_GAME_PLAYER_1_SYMBOL)
				node->value = INT_MAX;
			else
				node->value = INT_MIN;
			return node;
		}
		else {
			if (actualPlayer == SP_FIAR_GAME_PLAYER_1_SYMBOL)
				node->value = INT_MIN;
			else
				node->value = INT_MAX;
			return node;
		}
	}
	if (node->isLeaf) {
		node->value = scoringFunction(node, actualPlayer);
		return node;
	}
	for (i = SP_FIAR_GAME_N_COLUMNS - 1; i >= 0; i--) {
		if (point != NULL) {
			curNode = calcChildrenMin(point, actualPlayer);
			if (curNode->value >= maxValue) {
				maxValue = curNode->value;
				maxIndex = i;
			}
		}
		point--;
	}
	node->value = maxValue;
	node->childIndex = maxIndex;
	return node;
}


Node* calcChildrenMin(Node* node,char actualPlayer) {
	int minIndex = -1, minValue = INT_MAX, i = 0;
	char player = node->gameStatus->currentPlayer;
	Node* point = node->children + SP_FIAR_GAME_N_COLUMNS-1;
	Node* curNode = NULL;
	node->childIndex = -1;
	if (node->type != REGULAR) {
		if (node->type == TIE) {
			node->value = 0;
			return node;
		}
		else if (node->type == PLAYER_1_WIN) {
			if (actualPlayer == SP_FIAR_GAME_PLAYER_1_SYMBOL)
				node->value = INT_MAX;
			else
				node->value = INT_MIN;
			return node;
		}
		else {
			if (actualPlayer == SP_FIAR_GAME_PLAYER_1_SYMBOL)
				node->value = INT_MIN;
			else
				node->value = INT_MAX;
			return node;
		}
	}
	if (node->isLeaf) {
		node->value = scoringFunction(node, actualPlayer);
		return node;
	}
	for (i = SP_FIAR_GAME_N_COLUMNS - 1; i >= 0; i--) {
		if (point != NULL) {
			curNode = calcChildrenMax(point, actualPlayer);
			if (curNode->value <= minValue) {
				minValue = curNode->value;
				minIndex = i;
			}
		}
		point--;
	}
	node->value = minValue;
	node->childIndex = minIndex;
	return node;
}


/*int* calcChildrenMaxOld(Node* node) {
	int maxIndex = -1, maxValue = INT_MIN, i = 0, *curCalc;
	int retCalc[2] = { 0 };
	char player = node->gameStatus->currentPlayer;
	Node* point = node->children + SP_FIAR_GAME_N_COLUMNS;
	retCalc[1] = -1;
	if (node->type != REGULAR) {
		if (node->type == TIE) {
			retCalc[0] = 0;
			return retCalc;
		}
		else if (node->type == PLAYER_1_WIN) {
			if (player == SP_FIAR_GAME_PLAYER_1_SYMBOL)
				retCalc[0] = INT_MAX;
			else
				retCalc[0] = INT_MIN;
			return retCalc;
		}
		else {
			if (player == SP_FIAR_GAME_PLAYER_1_SYMBOL)
				retCalc[0] = INT_MIN;
			else
				retCalc[0] = INT_MAX;
			return retCalc;
		}
	}
	if (node->isLeaf) {
		retCalc[0] = scoringFunction(node, player);
		return retCalc;
	}
	for (i = SP_FIAR_GAME_N_COLUMNS - 1; i >= 0; i--) {
		if (point != NULL) {
			curCalc = calcChildrenMin(point);
			if (curCalc[0] >= maxValue) {
				maxValue = curCalc[0];
				maxIndex = i;
			}
		}
		point--;
	}
	retCalc[0] = maxValue;
	retCalc[1] = maxIndex;
	return retCalc;
}

int* calcChildrenMin(Node* node) {
	int minIndex = -1, minValue = INT_MAX, i = 0, *curCalc;
	int retCalc[2] = { 0 };
	char player = node->gameStatus->currentPlayer;
	Node* point = node->children + SP_FIAR_GAME_N_COLUMNS;
	retCalc[1] = -1;
	if (node->type != REGULAR) {
		if (node->type == TIE) {
			retCalc[0] = 0;
			return retCalc;
		}
		else if (node->type == PLAYER_1_WIN) {
			if (player == SP_FIAR_GAME_PLAYER_1_SYMBOL)
				retCalc[0] = INT_MIN;
			else
				retCalc[0] = INT_MAX;
			return retCalc;
		}
		else {
			if (player == SP_FIAR_GAME_PLAYER_1_SYMBOL)
				retCalc[0]= INT_MAX;
			else
				retCalc[0] = INT_MIN;
			return retCalc;
		}

	}
	if (node->isLeaf) {
		retCalc[0] = scoringFunction(node, player);
		return retCalc;
	}
	for (i = SP_FIAR_GAME_N_COLUMNS - 1; i >= 0; i--) {
		if (point != NULL) {
			curCalc = calcChildrenMax(point);
			if (curCalc[0] <= minValue) {
				minValue = curCalc[0];
				minIndex = i;
			}
		}
		point--;
	}
	retCalc[0] = minValue;
	retCalc[1] = minIndex;
	return retCalc;
}*/



/*int main() {
SPFiarGame* g = spFiarGameCreate(20);
spFiarGameSetMove(g, 1);
spFiarGameSetMove(g, 3);
spFiarGameSetMove(g, 1);
spFiarGameSetMove(g, 3);

spFiarGameSetMove(g, 3);
spFiarGameSetMove(g, 1);

spFiarGameSetMove(g, 1);
spFiarGameSetMove(g, 5);
spFiarGameSetMove(g, 2);
spFiarGameSetMove(g, 2);
spFiarGameSetMove(g, 1);
spFiarGameSetMove(g, 6);
spFiarGameSetMove(g, 4);
spFiarGameSetMove(g, 2);
spFiarGamePrintBoard(g);
Node* n = createNode(g);
printf("\n\n%d", scoringFunction(n, SP_FIAR_GAME_PLAYER_1_SYMBOL));
return 0;
}*/