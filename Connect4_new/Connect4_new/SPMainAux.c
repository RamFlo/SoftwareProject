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
	SPCommand cmd;
	char usr_in[SP_MAX_LINE_LENGTH];
	const char delimiter[5] = " \t\r\n";
	const char * const_pointer = usr_in;
	char *token;
	printf("Please enter the difficulty level between [1-7]:\n");
	fgets(usr_in, SP_MAX_LINE_LENGTH, stdin);
	char* strCopy = my_strdup_main(const_pointer);
	token = strtok(strCopy, delimiter);
	if (token == NULL) {
		printf("Error: invalid level (should be between 1 to 7)\n");

		return getMaxDepth();
	}
	cmd = spParserPraseLine(const_pointer);
	if (cmd.cmd == SP_QUIT)
		return -1;


	
	const_pointer = token;
	token = strtok(NULL, delimiter);
	if (!spParserIsInt(const_pointer) || token != NULL) {
		printf("Error: invalid level (should be between 1 to 7)\n");
		return getMaxDepth();
	}
	curNum = atoi(const_pointer);
	if (curNum < 1 || curNum>7) {
		printf("Error: invalid level (should be between 1 to 7)\n");
		return getMaxDepth();
	}
	return curNum;


}















int getMaxDepth2() {
	int maxDepth = 0,curNum=0;
	SPCommand cmd;
	char command[SP_MAX_LINE_LENGTH];
	char *command_pointer = command,*token;
	const char *constant_pointer;
	char* strCopy;
	printf("Please enter the difficulty level between [1-7]:\n");
	fgets(command, SP_MAX_LINE_LENGTH, stdin);
	constant_pointer = command_pointer;
	strCopy = my_strdup_main(command);
	token = strtok(strCopy, " \t\r\n");
	if (token == NULL) {
		printf("Error: invalid level (should be between 1 to 7)\n");
		return getMaxDepth();
	}
	cmd = spParserPraseLine(command);
	if (cmd.cmd == SP_QUIT)
		return -1;
	constant_pointer = token;
	token = strtok(NULL, " \t\r\n");
	if (!spParserIsInt(constant_pointer) || token!=NULL) {
		printf("Error: invalid level (should be between 1 to 7)\n");
		return getMaxDepth();
	}
	curNum = atoi(constant_pointer);
	if (curNum < 1 || curNum>7) {
		printf("Error: invalid level (should be between 1 to 7)\n");
		return getMaxDepth();
	}
	return curNum;
}

int main() {
	int maxDepth = 0;
	//while(true){
	maxDepth = getMaxDepth();
		



	//}
	return 0;
}