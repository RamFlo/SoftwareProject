#define MAX_HEAP_SIZE 32
#include <stdio.h>
#include <stdlib.h>
#include "sp_nim.h"
#include "main_aux.h"
#include "SPBufferset.h"
int main() {
	SP_BUFF_SET();
	int heapSizes[MAX_HEAP_SIZE] = { 0 };
	int heapNum = 0,turn=0;
	heapNum = askNumOfHeaps();
	if (heapNum <= 0)
		exit(1);
	if (askSizeOfHeaps(heapSizes,heapNum) < 0)
		exit(2);
	while (checkWinner(turn, heapSizes, heapNum) != 1) {
		printBoard(heapSizes, heapNum, turn);
		if(turn%2==1)
			userTurn(heapSizes, heapNum);
		else
			computerTurn(heapSizes, heapNum);
		turn++;
	}
	exit(3);
}