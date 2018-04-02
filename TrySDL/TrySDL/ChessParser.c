#include "ChessParser.h"
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
//#include "SPMainAux.h"

bool spParserIsInt(const char* str) {
	int size = 0,i=0;
	if (sizeof(str) == 0)
		return false;
	size = (int)strlen(str);
	if (str[0] - 45 != 0) {
		if (!isdigit(str[0]))
			return false;
	}
	else {
		if (size == 1)
			return false;
	}
	for (i = 1; i < size; i++) {
		if (!isdigit(str[i]))
			return false;
	}
	return true;
}


bool isValidNumArg(char* token) {
	if (token == NULL)
		return false;
	if (spParserIsInt(token))
		return true;
	return false;
}

bool isValidSquareArg(char* token, ChessCommand* result, int squareNum) {
	int i = 0, commaIndex = 0, length = (int)strlen(token);
	if (token == NULL || token[0] != '<' || token[length - 1] != '>')
		return false;
	for (i = 1; i < length - 1; i++) {
		if (token[i] == ',') {
			commaIndex = i;
			break;
		}
	}
	if (commaIndex == 0)
		return false;
	if (commaIndex != 2 || length-1 - commaIndex != 2)
		return true;
	if (squareNum == 1) {
		result->r1 = token[1];
		result->c1 = token[3];
	}
	else {
		result->r2 = token[1];
		result->c2 = token[3];
	}
	return true;
}

void initiateEmptyPath(ChessCommand *cmd) {
	int i = 0;
	for (i = 0; i < 2048; i++)
		cmd->path[i] = '\0';
}

ChessCommand spParserPraseLine(char* str) {
	ChessCommand result;
	result.validArg = false;
	result.cmd = INVALID_LINE;
	const char delimiter[5] = " \t\r\n";
	char *token;
	initiateEmptyPath(&result);
	token = strtok(str, delimiter);
	if (token == NULL)
		return result;
	if (strcmp(token, "start") == 0) {
		result.type = 0;
		result.cmd = START;
	}
	else if (strcmp(token, "quit") == 0) {
		result.type = 2;
		result.cmd = QUIT;
	}
	else if (strcmp(token, "reset") == 0) {
		result.type = 1;
		result.cmd = RESET;
	}
	else if (strcmp(token, "default") == 0) {
		result.type = 0;
		result.cmd = DEFAULT;
	}
	else if (strcmp(token, "undo") == 0) {
		result.type = 1; 
		result.cmd = UNDO;
	}
		
	else if (strcmp(token, "print_settings") == 0) {
		result.type = 0;
		result.cmd = PRINT_SETTINGS;
	}
	else if (strcmp(token, "game_mode") == 0) {
		result.cmd = GAME_MODE;
		result.type = 0; 
		token = strtok(NULL, delimiter);
		if ((result.validArg = isValidNumArg(token)) == true)
			result.arg = atoi(token);
	}
	else if (strcmp(token, "difficulty") == 0) {
		result.cmd = DIFFICULTY;
		result.type = 0;
		token = strtok(NULL, delimiter);
		if ((result.validArg = isValidNumArg(token)) == true)
			result.arg = atoi(token);
	}
	else if (strcmp(token, "user_color") == 0) {
		result.type = 0;
		result.cmd = USER_COLOR;
		token = strtok(NULL, delimiter);
		if ((result.validArg = isValidNumArg(token)) == true)
			result.arg = atoi(token);
	}
	else if (strcmp(token, "save") == 0) {
		result.type = 1;
		result.cmd = SAVE;
		token = strtok(NULL, delimiter);
		if (token != NULL)
			strcpy(result.path, token);
	}
	else if (strcmp(token, "load") == 0) {
		result.type = 0;
		result.cmd = LOAD;
		token = strtok(NULL, delimiter);
		if (token != NULL)
			strcpy(result.path, token);
	}
	else if (strcmp(token, "get_moves") == 0) {
		result.type = 1;
		result.cmd = GET_MOVES;
		token = strtok(NULL, delimiter);
		result.validArg = isValidSquareArg(token, &result,1);
	}
	else if (strcmp(token, "move") == 0) {
		result.type = 1;
		result.cmd = MOVE;
		token = strtok(NULL, delimiter);
		result.validArg = isValidSquareArg(token, &result,1);
		if (!result.validArg)
			return result;
		token = strtok(NULL, delimiter);
		if (strcmp(token, "to") != 0) {
			result.validArg = false;
			return result;
		}
		token = strtok(NULL, delimiter);
		result.validArg = isValidSquareArg(token, &result, 2);
	}
	return result;
}