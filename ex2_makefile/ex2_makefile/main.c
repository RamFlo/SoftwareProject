#define MAX_HEAP_SIZE 32
#include <stdio.h>
#include <stdlib.h>
#include "sp_nim.h"
#include "main_aux.h"
#include "SPBufferset.h"
/*
This is the game's main function.
1. The function asks the user for the heaps' number using askNumOfHeaps() (if the input was not legal, the program terminates). 
2. The function asks the user for the number of objects in the heaps using askSizeOfHeaps(heapSizes,heapNum). (if the input was not legal, the program terminates).
3. While the heaps are not empty, prints the board and executes either the user's or the computer's turn according to the turn number. 
4 . When the heaps are empty, The checkWinner function returns 1, the while loop ends and the program ends.
*/
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
	return 0;
}