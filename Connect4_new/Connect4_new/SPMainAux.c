#include "SPMainAux.h"
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
//extern bool delTree = false;



void printWinner(SPFiarGame* curGame) {
	if (spFiarCheckWinner(curGame) == SP_FIAR_GAME_PLAYER_1_SYMBOL)
		printf("Game over: you win\nPlease enter 'quit' to exit or 'restart' to start a new game!\n");
	if (spFiarCheckWinner(curGame) == SP_FIAR_GAME_PLAYER_2_SYMBOL)
		printf("Game over: computer wins\nPlease enter 'quit' to exit or 'restart' to start a new game!\n");
	if (spFiarCheckWinner(curGame) == SP_FIAR_GAME_TIE_SYMBOL)
		printf("Game over: it’s a tie\nPlease enter 'quit' to exit or 'restart' to start a new game!\n");
}

void memError(char * funcName) {
	printf("Error: %s has failed", funcName);
}

void endGame(SPFiarGame * game) {
	spFiarGameDestroy(game);
	exit(0);
}

char *my_strdup_main(const char *s) {
	size_t size = strlen(s) + 1;
	char *p = malloc(size);
	if (p) {
		memcpy(p, s, size);
	}
	return p;
}

int doUserCommand(SPFiarGame* curGame,int maxDepth) {
	int suggestedMove = 0;
	SPCommand curCommand;
	SP_FIAR_GAME_MESSAGE curGameMsg;
	char command[SP_MAX_LINE_LENGTH];
	char *p = command;
	const char *cp;
	fgets(command, SP_MAX_LINE_LENGTH, stdin);
	cp = p;
	curCommand = spParserPraseLine(command);
	if (curCommand.cmd == SP_INVALID_LINE) {
		printf("Error: invalid command\n");
		return doUserCommand(curGame,maxDepth);
	}
	else if (curCommand.cmd == SP_QUIT)
		endGame(curGame);
	else if (curCommand.cmd == SP_RESTART) {
		spFiarGameDestroy(curGame);
		return -1;
	}
	else if (curCommand.cmd == SP_UNDO_MOVE) {
		if (spFiarGameUndoPrevMove(curGame) == SP_FIAR_GAME_NO_HISTORY)
			return doUserCommand(curGame,maxDepth);
		else {
			spFiarGameUndoPrevMove(curGame);
			return 0;
		}	
	}
	else if (curCommand.cmd == SP_ADD_DISC) {
		if (!curCommand.validArg)
			printf("Error: column number must be in range 1-7\n");
		curGameMsg = spFiarGameSetMove(curGame, curCommand.arg - 1);
		if (curGameMsg==SP_FIAR_GAME_INVALID_ARGUMENT)
			printf("Error: column number must be in range 1-7\n");
		if (curGameMsg == SP_FIAR_GAME_INVALID_MOVE)
			printf("Error: column %d is full\n", curCommand.arg);
		if (curGameMsg == SP_FIAR_GAME_SUCCESS)
			return 0;
		return doUserCommand(curGame,maxDepth);
	}
	else {
		suggestedMove = spMinimaxSuggestMove(curGame, maxDepth);
		if (suggestedMove == -1) {
				memError("spMinimaxSuggestMove");
				endGame(curGame);
		}
		printf("Suggested move: drop a disc to column %d\n", suggestedMove+1);
		return doUserCommand(curGame, maxDepth);
	}
	return 0;
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


void computerTurn(SPFiarGame* curGame,int maxDepth) {
	int moveSuggestion = spMinimaxSuggestMove(curGame, maxDepth);
	if (moveSuggestion == -1) {
		memError("spMinimaxSuggestMove");
		endGame(curGame);
	}
	spFiarGameSetMove(curGame, moveSuggestion);
	printf("Computer move: add disc to column %d\n", moveSuggestion + 1);
}

int main() {
	SPFiarGame * curGame = NULL;
	SPCommand curCommand;
	int maxDepth = 0;
	//while(true)
	curGame = spFiarGameCreate(HISTORY_SIZE);
	if (curGame == NULL) {
		memError("spFiarGameCreate");
		endGame(curGame);
	}
	maxDepth = getMaxDepth();
	if (maxDepth == -1)
		endGame(curGame);
	while (true) {
		if (spFiarCheckWinner(curGame) != NULL) {
			spFiarGamePrintBoard(curGame);
			printWinner(curGame);
			curCommand = readCommand();
			while (curCommand.cmd != SP_RESTART && curCommand.cmd != SP_QUIT && curCommand.cmd != SP_UNDO_MOVE) {
				printf("Error: invalid command\n");
				curCommand = readCommand();
			}
			if (curCommand.cmd == SP_QUIT)
				endGame(curGame);
			else if (curCommand.cmd == SP_RESTART) {
				spFiarGameDestroy(curGame);
				break;
			}
			else {
				spFiarGameUndoPrevMove(curGame);
				if (spFiarCheckWinner(curGame) != SP_FIAR_GAME_PLAYER_1_SYMBOL)
					spFiarGameUndoPrevMove(curGame);
				continue;
			}
		}
		if (spFiarGameGetCurrentPlayer(curGame) == SP_FIAR_GAME_PLAYER_2_SYMBOL) {
			computerTurn(curGame, maxDepth);
			continue;
		}
		else {
			spFiarGamePrintBoard(curGame);
			printf("Please make the next move:\n");
			doUserCommand(curGame);
		}
	}
}