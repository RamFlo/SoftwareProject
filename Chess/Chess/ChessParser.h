#ifndef CHESSPARSER_H_
#define CHESSPARSER_H_
#include <stdbool.h>

//specify the maximum line length
#define SP_MAX_LINE_LENGTH 2048

//a type used to represent a command
typedef enum {
	GAME_MODE,
	DIFFICULTY,
	USER_COLOR,
	LOAD,
	DEFAULT,
	PRINT_SETTINGS,
	QUIT,
	START,
	MOVE,
	GET_MOVES,
	SAVE,
	UNDO,
	RESET,
	INVALID_LINE,
} CHESS_COMMAND;

//a new type that is used to encapsulate a parsed line
typedef struct command_t {
	CHESS_COMMAND cmd;
	bool validArg; //is set to true if the line contains a valid argument
	int arg;
	char path[2048];
	char r1, c1, r2, c2;
	int type; //0 - settings command, 1 - game command, 2 - command that belongs to both
} ChessCommand;

/**
* Returns whether or not the given string str is a valid integer.
* @param str - a string (not allowed to be NULL)
* @return
* true ,if the given string is a valid integer
* false ,otherwise
*/
bool spParserIsInt(const char* str);

/**
* This function parses the given string 'str' into a ChessCommand
* @param str - a string
* @return
* a ChessCommand containing the parsed command values
*/
ChessCommand spParserPraseLine(char* str);

#endif
