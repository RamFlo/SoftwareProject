#include "SPMinimaxNode.h"

Node* createNode(SPFiarGame* gameStatus) {
	Node *newNode, *children;
	bool isLeaf = true;
	newNode = (Node*)calloc(1, sizeof(Node));
	if (newNode == NULL)
		return NULL;
	newNode->gameStatus = gameStatus;
	children = (Node*)calloc(CHILDREN, sizeof(Node));
	if (children == NULL) {
		free(newNode);
		return NULL;
	}
	newNode->children = children;
	newNode->isLeaf = isLeaf;
	return newNode;
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
	return weights[sum + 3];
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
		weight += weights[curValue + 3];
		for (j = 1; j < rowSpans; j++) {
			curValue = curValue - discValue(node, j - 1, i, player) + discValue(node, j + SP_FIAR_GAME_SPAN - 1, i, player);
			weight += weights[curValue + 3];
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
			weight += weights[3 + diagUpSpanValue(node, j, i, player)];
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
			weight += weights[3 + diagDownSpanValue(node, j, i, player)];
	}
	return weight;
}

int scoringFunction(Node* node, char player) {
	return colsScoring(node, player) + rowScoring(node, player) + diagUpScoring(node, player) + diagDownScoring(node, player);
}

int* calcChildrenMax(Node* node) {
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
	if (node->isLeaf)
		return scoringFunction(node, getOtherPlayer(player));
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
}



int main() {
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
}