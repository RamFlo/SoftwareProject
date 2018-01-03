#include "SPMinimax.h"
#include <stdio.h>

bool delTree = false;

/**
* Given a game state,maxDepth and root's address, this function creates a tree structure 
* using the root's address as the tree's root. The tree's depth is set to maxDepth.
* Each node contains it's own copy of the base game, with the corresponding move to the
* node's child index. If the node doesn't contain a valid move, it's created anyway with
* isValidMove=false.
* While creating the tree, each node is marked with type:PLAYER_1_WIN, PLAYER_2_WIN, TIE or 
* REGULAR. if the node's type isn't REGULAR, or if maxDepth was reached, the node is marked as
* lead (isLeaf=true).
* In case of a memory error, the boolean delTree is set to true, so that we can destroy the tree in 
* another function.

* @param currentGame - The current game state
* @param maxDepth - The maximum depth of the miniMax algorithm
* @param root - The address of the tree's root (created elsewhere)
*/
void createTree(SPFiarGame* currentGame, unsigned int maxDepth,Node* root) {
	SPFiarGame* gameCpy = NULL;
	Node* point;
	int i = 0;
	char winner = spFiarCheckWinner(currentGame);
	if (root == NULL)
		return;
	root->isValidMove = true;
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
	for (i=0; i < CHILDREN; i++) {
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

/*
* Given an address of a tree's root, this function scans the entire tree 
* and destroys the game state residing in each node: first, it does so for the node's
* children, and then for the node itself.

* @param root - Address of tree in which the game states should be destroyed
*/
void destroyGamesInTree(Node* root) {
	int i = 0;
	Node* point = NULL;
	if (root == NULL)
		return;
	if (!root->isLeaf && root->isValidMove) {
		point = root->children;
		if (point == NULL)
			return;
		for (i=0; i < CHILDREN; i++) {
			destroyGamesInTree(point);
			point++;
		}
	}
	if (root->isValidMove)
		spFiarGameDestroy(root->gameStatus);
}

/*
* Given an address of a tree's root, this function scans the entire tree
* and destroys the child array of each node: first, it does so for the node's
* children, and then for the node itself. (The root is destroyed seperately).

* @param root - Address of tree in which the child arrays should be destroyed
*/
void destroyChildren(Node* root) {
	int i = 0;
	Node* point = NULL;
	if (root == NULL)
		return;
	if (!root->isLeaf && root->isValidMove) {
		point = root->children;
		if (point == NULL)
			return;
		for (i=0; i < CHILDREN; i++) {
			destroyChildren(point);
			point++;
		}
	}
	if (root->isValidMove)
		free(root->children);
}


/*
* Given an address of a tree's root, this function calls destroyGamesInTree and 
* destroyChildren to destroy the entire tree. Finally, it destroys the root.

* @param root - Address of a tree to be destroyed.
*/
void destroyTree(Node* root) {
	destroyGamesInTree(root);
	destroyChildren(root);
	free(root);
}

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

