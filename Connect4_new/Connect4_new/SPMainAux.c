#include "SPMainAux.h"



SPCommand readCommand() {
	SPCommand cmd;
	char command[SP_MAX_LINE_LENGTH];
	char *p = command;
	const char *cp;
	fgets(command, SP_MAX_LINE_LENGTH, stdin);
	cp = p;
	cmd = spParserPraseLine(command);
	
	
	
	return cmd;
}

int getMaxDepth() {
	int maxDepth = 0;
	
	printf("Please enter the difficulty level between [1-7]:\n");

	

	while (scanf("%d", &maxDepth) != 1 || maxDepth < 1 || maxDepth > 7) {
		printf("Error: invalid level (should be between 1 to 7)\n");
		printf("Please enter the difficulty level between [1-7]:\n");
	}
	return maxDepth;
}

/*int main() {
	int maxDepth = 0;
	while(true){
		maxDepth = getMaxDepth();
		



	}
	return 0;
}*/