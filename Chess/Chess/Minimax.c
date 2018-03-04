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

int scoringFunction(ChessGame* src) {
	int i = 0, j = 0, score = 0;
	if (src->checkmated == WHITE_PLAYER)
		return -1000;
	if (src->checkmated == BLACK_PLAYER)
		return 1000;
	if (src->draw)
		return 0;
	for (i = 0; i < 8; i++) {
		for (j = 0; j < 8; j++) {
			switch (src->gameBoard[i][j]) {
			case WHITE_PAWN:
				score += 1;
				break;
			case BLACK_PAWN:
				score -= 1;
				break;
			case WHITE_KNIGHT:
				score += 3;
				break;
			case BLACK_KNIGHT:
				score -= 3;
				break;
			case WHITE_BISHOP:
				score += 3;
				break;
			case BLACK_BISHOP:
				score -= 3;
				break;
			case WHITE_ROOK:
				score += 5;
				break;
			case BLACK_ROOK:
				score -= 5;
				break;
			case WHITE_QUEEN:
				score += 9;
				break;
			case BLACK_QUEEN:
				score -= 9;
				break;
			case WHITE_KING:
				score += 100;
				break;
			case BLACK_KING:
				score -= 100;
				break;
			}
		}
	}
	return score;
}

int MinimaxSuggestMoveRec(ChessGame* g, int maxDepth, int fatherCurBestScore) {
	int minOrMax = 0, curBestScore = 0, i = 0, j = 0, k = 0, l = 0, curMoveScore = 0;
	ChessGame* gCopy;
	minOrMax = (g->currentPlayer == WHITE_PLAYER) ? 1 : 0;
	curBestScore = (minOrMax == 1) ? -1001 : 1001;
	if (maxDepth == 0 || g->checkmated != '\0' || g->draw)
		return scoringFunction(g);
	for (j = 0; j < 8; j++) {
		for (i = 7; i >= 0; i--) {
			if (isCurPlayerPiece(i, j, g)) {
				for (l = 0; l < 8; l++) {
					for (k = 7; k >= 0; k--) {
						if ((minOrMax == 0 && curBestScore <= fatherCurBestScore) || (minOrMax == 1 && curBestScore >= fatherCurBestScore))
							return curBestScore;
						gCopy = ChessGameCopy(g);
						if (gCopy == NULL)
							memError = true;
						if (ChessGameSetMove(gCopy, '8' - i, 'A' + j, '8' - k, 'A' + l) == SUCCESS) {
							curMoveScore = MinimaxSuggestMoveRec(gCopy, maxDepth - 1, curBestScore);
							curBestScore = (minOrMax == 1) ? chooseMax(curBestScore, curMoveScore, g, j, i, l, k) : chooseMin(curBestScore, curMoveScore, g, j, i, l, k);
						}
						ChessGameDestroy(gCopy);
					}
				}
			}
		}
	}
	return curBestScore;
}

void MinimaxSuggestMove(ChessGame* g) {
	int curBestScore = (g->currentPlayer == WHITE_PLAYER) ? 1001 : -1001;
	MinimaxSuggestMoveRec(g, g->difficulty, curBestScore);
	if (memError) {
		printf("ERROR: suggest move memory error.");
		ChessGameDestroy(g);
		exit(0);
	}
}