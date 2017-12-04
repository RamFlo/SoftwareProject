#include <stdio.h>
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
void userTurn(int* heapSizes, int heapNum) {
	int index = 0, removed = 0;
	printf("Your turn: please enter the heap index and the number of removed objects.\n");
	while (scanf("%d %d", &index, &removed) != 2||index>heapNum||index<1||removed>heapSizes[index-1]||removed<=0)
		printf("Error: Invalid input.\nPlease enter again the heap index and the number of removed objects.\n");
	heapSizes[index-1] -= removed;
	printf("You take %d objects from heap %d.\n", removed, index);
}
int calcNimSum(int* heapSizes, int heapNum) {
	int i = 0,nimSum=heapSizes[0];
	for (i = 1; i < heapNum; i++)
		nimSum ^= heapSizes[i];
	return nimSum;
}
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