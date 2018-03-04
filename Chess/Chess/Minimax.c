#include "Minimax.h"
#include <stdio.h>

bool memError;


int chooseMax(int curScore, int newScore,ChessGame* g,int srcCol,int srcRow,int dstCol,int dstRow) {
	if (curScore >= newScore)
		return curScore;
	g->bestMoveSrcCol = srcCol;
	g->bestMoveSrcRow = srcRow;
	g->bestMoveDstCol = dstCol;
	g->bestMoveDstRow = dstRow;
	return newScore;
}

int chooseMin(int curScore, int newScore, ChessGame* g, int srcCol, int srcRow, int dstCol, int dstRow) {
	if (curScore <= newScore)
		return curScore;
	g->bestMoveSrcCol = srcCol;
	g->bestMoveSrcRow = srcRow;
	g->bestMoveDstCol = dstCol;
	g->bestMoveDstRow = dstRow;
	return newScore;
}

int MinimaxSuggestMove(ChessGame* g, int maxDepth) {
	int minOrMax =0, curBestScore = 0, i = 0, j = 0, k = 0, l=0,curMoveScore=0;
	ChessGame* gCopy;
	minOrMax = (g->currentPlayer == WHITE_PLAYER) ? 1 : 0;
	curBestScore = (minOrMax == 1) ? -1001 : 1001;

	if (maxDepth==0)
		//return score;
	for (j = 0; j < 8; j++) {
		for (i = 7; i >= 0; i--) {
			if (isCurPlayerPiece(i, j, g)) {
				for (l = 0; l < 8; l++) {
					for (k = 7; k >= 0; k--) {
						gCopy = ChessGameCopy(g);
						if (gCopy == NULL)
							memError = true;
						if (ChessGameSetMove(gCopy, '8' - i, 'A' + j, '8' - k, 'A' + l) == SUCCESS) {
							curMoveScore = MinimaxSuggestMove(gCopy, maxDepth - 1);
							curBestScore = (minOrMax == 1) ? chooseMax(curBestScore, curMoveScore, g, j, i, l, k) : chooseMin(curBestScore, curMoveScore, g, j, i, l, k);
						}	
						ChessGameDestroy(gCopy);
					}
				}
			}
		}
	}



}



int calcChildrenMax(ChessGame* g, int maxDepth) {

}