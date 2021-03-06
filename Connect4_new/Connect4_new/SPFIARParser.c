#include "SPFIARParser.h"
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include "SPMainAux.h"

/*
* Given a const char sequence's address, this function allocates a non-const char space of the same
* size and copies the given const char sequence into the non-const char space.
* In case of memory allocation error, parserMemError is set to true.
*
* @param s - The address of a const char sequence to copy
* @return
* NULL if a memory allocation error occurred
* On success, the function returns a pointer to the non-const char sequence which is
* a copy of the char sequence residing at address s.
*/
char *my_strdup(const char *s) {
	size_t size = strlen(s) + 1;
	char *p = malloc(size);
	if (p) {
		memcpy(p, s, size);
	}
	else {
		printf("Error: malloc has failed\n");
		parserMemError = true;
	}	
	return p;
}


bool spParserIsInt(const char* str) {
	int size = 0;
	if (sizeof(str) == 0)
		return false;
	size = strlen(str);
	if (str[0] - 45 != 0) {
		if (!isdigit(str[0]))
			return false;
	}
	else {
		if (size == 1)
			return false;
	}
	for (int i = 1; i < size; i++) {
		if (!isdigit(str[i]))
			return false;
	}
	return true;
}

SPCommand spParserPraseLine(const char* str) {
	SPCommand result;
	result.validArg = false;
	result.cmd = SP_INVALID_LINE;
	const char delimiter[5] = " \t\r\n";
	char *token;
	char* strCopy = my_strdup(str);
	token = strtok(strCopy, delimiter);
	if (token == NULL) {
		free(strCopy);
		return result;
	}
	if (strcmp(token, "suggest_move") == 0)
		result.cmd = SP_SUGGEST_MOVE;
	else if (strcmp(token, "undo_move") == 0)
		result.cmd = SP_UNDO_MOVE;
	else if (strcmp(token, "quit") == 0)
		result.cmd = SP_QUIT;
	else if (strcmp(token, "restart_game") == 0)
		result.cmd = SP_RESTART;
	else if (strcmp(token, "add_disc") == 0) {
		result.cmd = SP_ADD_DISC;
		token = strtok(NULL, delimiter);
		if (token == NULL) {
			free(strCopy);
			return result;
		}
		if (spParserIsInt(token)) {
			result.arg = atoi(token);
			result.validArg = true;
		}
		if (strtok(NULL, delimiter) != NULL)
			result.validArg = false;
		free(strCopy);
		return result;
		//2
	}
	if (strtok(NULL, delimiter) != NULL)
		result.cmd = SP_INVALID_LINE;
	free(strCopy);
	return result;
}