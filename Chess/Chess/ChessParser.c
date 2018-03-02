#include "ChessParser.h"
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
//#include "SPMainAux.h"

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

SPCommand spParserPraseLine(char* str) {
	SPCommand result;
	result.validArg = false;
	result.cmd = INVALID_LINE;
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
	}
	if (strtok(NULL, delimiter) != NULL)
		result.cmd = SP_INVALID_LINE;
	free(strCopy);
	return result;
}