#include "SPMainAux.h"
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
//extern bool delTree = false;




char *my_strdup_main(const char *s) {
	size_t size = strlen(s) + 1;
	char *p = malloc(size);
	if (p) {
		memcpy(p, s, size);
	}
	return p;
}

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
	int maxDepth = 0, curNum = 0;
	char usr_in[SP_MAX_LINE_LENGTH];
	const char delimiter[5] = " \t\r\n";
	const char * const_pointer = usr_in;
	char *token, *next_token;
	printf("Please enter the difficulty level between [1-7]:\n");
	fgets(usr_in, SP_MAX_LINE_LENGTH, stdin);
	char* strCopy = my_strdup_main(const_pointer);
	token = strtok(strCopy, delimiter);
	if (token == NULL) {
		printf("Error: invalid level (should be between 1 to 7)\n");
		free(strCopy);
		return getMaxDepth();
	}
	next_token= strtok(NULL, delimiter);
	if (strcmp(token, "quit") == 0 && next_token == NULL) {
		free(strCopy);
		return -1;
	}
	const_pointer = token;
	if (next_token != NULL || !spParserIsInt(const_pointer)) {
		printf("Error: invalid level (should be between 1 to 7)\n");
		free(strCopy);
		return getMaxDepth();
	}
	curNum = atoi(const_pointer);
	if (curNum < 1 || curNum>7) {
		printf("Error: invalid level (should be between 1 to 7)\n");
		free(strCopy);
		return getMaxDepth();
	}
	free(strCopy);
	return curNum;
}


int main() {
	int maxDepth = 0;
	//while(true){
	maxDepth = getMaxDepth();
		



	//}
	return 0;
}