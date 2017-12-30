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
	SPFiarGame * curGame = NULL;
	SPCommand curCommand;
	int maxDepth = 0,moveSuggestion=0;
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
			else if (curCommand.cmd == SP_RESTART)
				break;
			else {
				spFiarGameUndoPrevMove(curGame);
				if (spFiarCheckWinner(curGame) != SP_FIAR_GAME_PLAYER_1_SYMBOL)
					spFiarGameUndoPrevMove(curGame);
				continue;
			}
		}
		if (spFiarGameGetCurrentPlayer(curGame) == SP_FIAR_GAME_PLAYER_2_SYMBOL) {
			moveSuggestion = spMinimaxSuggestMove(curGame, maxDepth);
			if (moveSuggestion == -1) {
				memError("spMinimaxSuggestMove");
				endGame(curGame);
			}
			spFiarGameSetMove(curGame, moveSuggestion);
			printf("Computer move: add disc to column %d\n", moveSuggestion + 1);
		}
	}




}