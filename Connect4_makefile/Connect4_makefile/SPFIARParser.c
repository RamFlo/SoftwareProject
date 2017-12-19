#ifndef SPFIARPARSER_H_
#define SPFIARPARSER_H_
#include <stdbool.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
//specify the maximum line length
#define SP_MAX_LINE_LENGTH 1024

//a type used to represent a command
typedef enum {
	SP_UNDO_MOVE,
	SP_ADD_DISC,
	SP_SUGGEST_MOVE,
	SP_QUIT,
	SP_RESTART,
	SP_INVALID_LINE,
} SP_COMMAND;

//a new type that is used to encapsulate a parsed line
typedef struct command_t {
	SP_COMMAND cmd;
	bool validArg; //is set to true if the line contains a valid argument
	int arg;
} SPCommand;

char *my_strdup(const char *s) {
	size_t size = strlen(s) + 1;
	char *p = malloc(size);
	if (p) {
		memcpy(p, s, size);
	}
	return p;
}


/**
* Checks if a specified string represents a valid integer. It is recommended
* to use this function prior to calling the standard library function atoi.
*
* @return
* true if the string represents a valid integer, and false otherwise.
*/
bool spParserIsInt(const char* str) {
	int size = 0;
	int isDigit = 0;
	if (sizeof(str) == 0)
		return false;
	size = strlen(str);
	if (str[0] - 45 != 0) {
		if (!isdigit(str[0]))
			return false;
	} else {
		if (size == 1)
			return false;
	}
	for (int i = 1; i < size; i++) {
		if (!isdigit(str[i]))
			return false;
	}
	return true;
}


/**
* Parses a specified line. If the line is a command which has an integer
* argument then the argument is parsed and is saved in the field arg and the
* field validArg is set to true. In any other case then 'validArg' is set to
* false and the value 'arg' is undefined
*
* @return
* A parsed line such that:
*   cmd - contains the command type, if the line is invalid then this field is
*         set to INVALID_LINE
*   validArg - is set to true if the command is add_disc and the integer argument
*              is valid
*   arg      - the integer argument in case validArg is set to true
*/
//subject to change: validarg and cmd in case of invalid add_disc 
SPCommand spParserPraseLine(const char* str) {
	SPCommand result;
	result.validArg = false;
	result.cmd = SP_INVALID_LINE;
	const char delimiter[7] = "\t\r\n";
	char *token;
	char* strCopy = my_strdup(str);
	token = strtok(strCopy, delimiter);
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

#endif
