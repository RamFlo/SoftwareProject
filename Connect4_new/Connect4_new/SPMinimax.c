#include "SPMinimax.h"


Node* createTree(SPFiarGame* currentGame, unsigned int maxDepth) {
	Node* root = createNode(currentGame);
	Node* child;
	SPFiarGame* gameCpy = NULL;
	if (root == NULL)
		return NULL;
	Node* point = root->children;
	int i = 0;
	char winner = spFiarCheckWinner(currentGame);
	if (winner != '\0') {
		if (winner == SP_FIAR_GAME_TIE_SYMBOL)
			root->type = TIE;
		else if (winner == SP_FIAR_GAME_PLAYER_1_SYMBOL)
			root->type = PLAYER_1_WIN;
		else
			root->type = PLAYER_2_WIN;
		return root;
	}
	root->type = REGULAR;
	if (maxDepth == 0) {
		root->isLeaf = true;
		return root;
	}
	for (i; i < CHILDREN; i++) {
		if (spFiarGameIsValidMove(currentGame, i)) {
			gameCpy = spFiarGameCopy(currentGame);
			spFiarGameSetMove(gameCpy, i);
			child = createTree(gameCpy, maxDepth - 1);
			if (child == NULL)
				delTree = true;
			*point = *child;
		}
		point++;
	}
	return root;
}


void destroyTree(Node* root) {
	int i = 0;
	Node* point = NULL;
	if (root != NULL) {
		point = root->children;
		for (i; i <= CHILDREN; i++) {
			destroyTree(point);
			point++;
		}
		destroyNode(root);
	}
}
/**
* Given a game state, this function evaluates the best move according to
* the current player. The function initiates a Minimax algorithm up to a
* specified length given by maxDepth. The current game state doesn't change
* by this function including the history of previous moves.
*
* @param currentGame - The current game state
* @param maxDepth - The maximum depth of the miniMax algorithm
* @return
* -1 if either currentGame is NULL or maxDepth <= 0.
* On success the function returns a number between [0,SP_FIAR_GAME_N_COLUMNS -1]
* which is the best move for the current player.
*/
int spMinimaxSuggestMove(SPFiarGame* currentGame, unsigned int maxDepth) {
	Node* root;
	int res;
	if (currentGame == NULL || maxDepth <= 0)
		return -1;
	root = createTree(spFiarGameCopy(currentGame), maxDepth);
	if (root == NULL || delTree){
		destroyTree(root);
		return -1;
	}
	res = calcChildrenMax(root)[1];
	destroyTree(root);
	return res;
}

