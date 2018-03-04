#include "Minimax.h"
#include <stdio.h>


int MinimaxSuggestMove(ChessGame* currentGame, int maxDepth) {
	int res;
	if (currentGame == NULL || maxDepth <= 0)
		return -1;
	createTree(ChessGameCopy(currentGame), maxDepth, root);
	if (root == NULL || delTree) {
		destroyTree(root);
		delTree = false;
		return -1;
	}
	res = calcChildrenMax(root, currentGame->currentPlayer)->childIndex;
	destroyTree(root);
	return res;
}

