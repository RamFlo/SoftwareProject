#include <stdio.h>
/*
This function checks for an enpty board at the start of each turn. If the board is empty (the heaps are empty), it prints the winner and returns 1. Else, it return -1.
*/
int checkWinner(int turn, int* heapSizes,int heapNum) {
	int i = 0;
	for (i = 0; i < heapNum; i++)
		if (heapSizes[i] != 0)
			return -1;
	if (turn % 2 == 0)
		printf("You win!\n");
	else
		printf("Computer wins!\n");
	return 1;
}
/*
This function executes the user's turn. It scans the heap's index and number of objects to be removed from the user, and checks if they are legal (the index must be a number between 1
and the number of heaps and the number of objects to be removed must be a positive number smaller or equal to the number of objects currently in the heap).
If the input is legal, the objects are removed from the corresponding heap. Else, the user is asked to enter a different input.
*/
void userTurn(int* heapSizes, int heapNum) {
	int index = 0, removed = 0;
	printf("Your turn: please enter the heap index and the number of removed objects.\n");
	while (scanf("%d %d", &index, &removed) != 2||index>heapNum||index<1||removed>heapSizes[index-1]||removed<=0)
		printf("Error: Invalid input.\nPlease enter again the heap index and the number of removed objects.\n");
	heapSizes[index-1] -= removed;
	printf("You take %d objects from heap %d.\n", removed, index);
}
/*
This function calculates the Nim Sum, as defined in the instructions, based on the status of the heaps at the time of it's activation.
First, it calculates the XOR of all the heaps (heap1 XOR heap2 XOR ... XOR heapn). The result is stored at int nimSum.
Finally - it returns the value stored in nimSum, as that is the current Nim Sum of the heaps.
*/
int calcNimSum(int* heapSizes, int heapNum) {
	int i = 0,nimSum=heapSizes[0];
	for (i = 1; i < heapNum; i++)
		nimSum ^= heapSizes[i];
	return nimSum;
}

/*
This function executes the computer's turn.
It operates according to the logic and methodology which were introduced in the instructions.
First, we check if the current Nim Sum of the heap is 0. If it is - the computer takes 1 object from the lowest-index non-empty heap.
However, if Nim Sum is not zero, we search for the first heap which if it's size is XORed with the current Nim Sum, the result will still be bigger then the current Nim Sum.
We then change that heap's size to be it's current size XOR the current Nim Sum.
*/
void computerTurn(int* heapSizes, int heapNum) {
	int nimSum = calcNimSum(heapSizes, heapNum),i=0;
	if (nimSum == 0) {
		for (i = 0; i < heapNum; i++) {
			if (heapSizes[i] > 0) {
				heapSizes[i]--;
				printf("Computer takes 1 objects from heap %d.\n", i + 1);
				break;
			}
		}
	}
	else {
		for (i = 0; i < heapNum; i++) {
			if ((heapSizes[i] ^ nimSum) < heapSizes[i]) {
				printf("Computer takes %d objects from heap %d.\n",(heapSizes[i]- (heapSizes[i] ^ nimSum)), i + 1);
				heapSizes[i] = heapSizes[i] ^ nimSum;
				break;
			}
		}
	}
}