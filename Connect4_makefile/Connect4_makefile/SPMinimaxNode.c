#ifndef SPMINIMAXNODE_H_
#define SPMINIMAXNODE_H_
#include "SPFIARGame.h"
#define CHILDREN SP_FIAR_GAME_N_COLUMNS

//Put all decleartions and constants here

typedef struct Node {
	Node* children[CHILDREN];
	SPFiarGame* gameStatus;
	bool isLeaf;
} Node;

Node* createNode(SPFiarGame* gameStatus) {
	Node *newNode, *children;
	bool isLeaf = true;
	SPFiarGame* gameStatus = spFiarGameCopy(SPFiarGame* gameStatus);
	children = (Node*)calloc(CHILDREN*sizeof(Node));
	newNode = (Node*)calloc(sizeof(Node));
	newNode->children = children;
	newNode->isLeaf = isLeaf;
}


#endif
