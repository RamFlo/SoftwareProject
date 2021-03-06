#include <stdio.h>
/*This function takes the number of heaps as input from the user, and checks if the input was legal (a number between 1 and 32).
If the input was legal, the number of heaps is returned. else, -1 is returned.*/
int askNumOfHeaps() {
	int heapNum = 0;
	printf("Enter the number of heaps:\n");
	if (scanf("%d", &heapNum) == 1 && heapNum >= 1 && heapNum <= 32)
		return heapNum;
	printf("Error: the number of heaps must be between 1 and 32.\n");
	return -1;
}
/*
This function takes the number of objects at each heap as input from the user.
If the input was illegal (anything other than a positive integer) it returns -1. Else, it returns 1.
*/
int askSizeOfHeaps(int* heapSizes, int heapNum) {
	int i = 0;
	printf("Enter the heap sizes:\n");
	for (i = 0; i < heapNum; i++) {
		if (scanf("%d", &heapSizes[i]) != 1 || heapSizes[i] <= 0){
			printf("Error: the size of heap %d should be positive.\n", i+1);
			return -1;
		}
	}
	return 1;
}
/*
This function prints the current state of the board.
Namely, it prints the current turn, then the heaps sizes ordered from 1 to n.
*/
void printBoard(int* heapSizes, int heapNum,int turn) {
	int i = 0;
	printf("In turn %d heap sizes are:",turn+1);
	for (i = 0; i < heapNum; i++)
		printf(" h%d=%d", i+1, heapSizes[i]);
	printf(".\n");
}