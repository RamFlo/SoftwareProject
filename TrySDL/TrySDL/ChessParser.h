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
* Checks if a specified string represents a valid integer. It is recommended
* to use this function prior to calling the standard library function atoi.
*
* @return
* true if the string represents a valid integer, and false otherwise.
*/
bool spParserIsInt(const char* str);

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
ChessCommand spParserPraseLine(char* str);

#endif
