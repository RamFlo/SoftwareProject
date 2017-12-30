#include "SPMinimax.h"

bool delTree = false;
void createTree(SPFiarGame* currentGame, unsigned int maxDepth,Node* root) {
	SPFiarGame* gameCpy = NULL;
	Node* point;
	int i = 0;
	char winner = spFiarCheckWinner(currentGame);
	if (root == NULL)
		return;
	root->gameStatus = currentGame;
	if (winner != '\0') {
		root->isLeaf = true;
		if (winner == SP_FIAR_GAME_TIE_SYMBOL)
			root->type = TIE;
		else if (winner == SP_FIAR_GAME_PLAYER_1_SYMBOL)
			root->type = PLAYER_1_WIN;
		else
			root->type = PLAYER_2_WIN;
		return;
	}
	root->type = REGULAR;
	if (maxDepth == 0) {
		root->isLeaf = true;
		return;
	}
	root->children = (Node*)calloc(CHILDREN, sizeof(Node));
	if (root->children == NULL) {
		printf("Error: calloc has failed\n");
		delTree = true;
		return;
	}
	point = root->children;
	for (i; i < CHILDREN; i++) {
		if (!delTree &&	spFiarGameIsValidMove(currentGame, i)) {
			gameCpy = spFiarGameCopy(currentGame);
			if (gameCpy == NULL) {
				delTree = true;
				return;
			}
			spFiarGameSetMove(gameCpy, i);
			createTree(gameCpy, maxDepth - 1,point);
		}
		point++;
	}
	return;
}


void destroyGamesInTree(Node* root) {
	int i = 0;
	Node* point = NULL;
	if (root == NULL)
		return;
	if (!root->isLeaf) {
		point = root->children;
		for (i; i < CHILDREN; i++) {
			destroyGamesInTree(point);
			point++;
		}
	}
	spFiarGameDestroy(root->gameStatus);
}


void destroyChildren(Node* root) {
	int i = 0;
	Node* point = NULL;
	if (root == NULL)
		return;
	if (!root->isLeaf) {
		point = root->children;
		for (i; i < CHILDREN; i++) {
			destroyChildren(point);
			point++;
		}
	}
	free(root->children);
}

void destroyTree(Node* root) {
	destroyGamesInTree(root);
	destroyChildren(root);
	free(root);
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
	root = createRoot();
	createTree(spFiarGameCopy(currentGame), maxDepth, root);
	if (root == NULL || delTree){
		destroyTree(root);
		delTree = false;
		return -1;
	}
	res = calcChildrenMax(root, currentGame->currentPlayer)->childIndex;
	destroyTree(root);
	return res;
}

