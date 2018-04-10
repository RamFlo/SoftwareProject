#include "MainAux.h"
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

//Chess code starts here

//gets a command (string) from the user, converts it to ChessCommand and returns it
ChessCommand readCommand() {
	ChessCommand cmd;
	char command[SP_MAX_LINE_LENGTH];
	fgets(command, SP_MAX_LINE_LENGTH, stdin);
	cmd = spParserPraseLine(command);
	return cmd;
}

//Checks if the current player is a human player, or the computer
bool isHuman(ChessGame* g) {
	if (g->gameMode == 2)
		return true;
	if ((g->userColor == 0 && g->currentPlayer == BLACK_PLAYER) || (g->userColor == 1 && g->currentPlayer == WHITE_PLAYER))
		return true;
	return false;
}

//quits game - destroys the game, prints message and exits
void quitGame(ChessGame* g) {
	ChessGameDestroy(g);
	printf("Exiting...\n");
	exit(0);
}

//Starts the settings state - gets a command from the user, makes sure it's a legal command that's executable on the settings state according to the instructions,
//then executes it accordingly
void settingsState(ChessGame* g) {
	ChessCommand curCmd;
	CHESS_GAME_MESSAGE curMsg;
	printf("Specify game settings or type 'start' to begin a game with the current settings:\n");
	while (true) {
		curCmd = readCommand();
		if (curCmd.cmd == START) {
			printf("Starting game...\n");
			return;
		}
		if (curCmd.type == 1 || curCmd.cmd == INVALID_LINE || (curCmd.cmd == USER_COLOR && g->gameMode == 2) || (curCmd.cmd == DIFFICULTY && g->gameMode == 2) || (curCmd.cmd == LOAD && curCmd.path == NULL)) {
			printf("ERROR: invalid command\n");
			continue;
		}
		switch (curCmd.cmd)
		{
		case GAME_MODE:
			if (curCmd.validArg && (curCmd.arg == 1 || curCmd.arg == 2)) {
				g->gameMode = curCmd.arg;
				printf("Game mode is set to %d-player\n", curCmd.arg);
			}
			else
				printf("Wrong game mode\n");
			break;
		case DIFFICULTY:
			if (curCmd.validArg && curCmd.arg >= 1 && curCmd.arg <= 5) {
				g->difficulty = curCmd.arg;
				printf("Difficulty is set to ");
				printDifficulty(g);
				printf("\n");
			}
			else
				printf("Wrong difficulty level. The value should be between 1 to 5\n");
			break;
		case USER_COLOR:
			if (curCmd.validArg && (curCmd.arg == 0 || curCmd.arg == 1)) {
				g->userColor = curCmd.arg;
				if (curCmd.arg == 0)
					printf("User color is set to black\n");
				else
					printf("User color is set to white\n");
			}
			else
				printf("Wrong user color. The value should be 0 or 1\n");
			break;
		case LOAD:
			if ((curMsg = ChessGameLoad(g, curCmd.path)) != SUCCESS) {
				if (curMsg == FILE_CREATE_FAILED)
					printf("Error: File doesn't exist or cannot be opened\n");
				else
					printf("ERROR: invalid command\n");
			}	
			break;
		case DEFAULT:
			chessGameDefault(g);
			printf("All settings reset to default\n");
			break;
		case PRINT_SETTINGS:
			chessGamePrintSettings(g);
			break;
		case QUIT:
			quitGame(g);
			break;
		default:;
		}
	}
}

//if it's a human player's turn - executes a player turn function. Else - executes a computer turn.
void gameState(ChessGame* g) {
	if (isHuman(g))
		playerTurn(g);
	else
		computerTurn(g,true);
}

//Computes the best move using Minimax, executes the move and prints it if shouldPrint=true
void computerTurn(ChessGame* g, bool shouldPrint) {
	MinimaxSuggestMove(g);
	ChessGameSetMove(g, '8' - g->bestMoveSrcRow, 'A' + g->bestMoveSrcCol, '8' - g->bestMoveDstRow, 'A' + g->bestMoveDstCol);
	if (shouldPrint) {
		printf("Computer: move ");
		printPieceName(g->gameBoard[g->bestMoveDstRow][g->bestMoveDstCol]);
		printf(" at <%c, %c> to <%c, %c>\n", '8' - g->bestMoveSrcRow, 'A' + g->bestMoveSrcCol, '8' - g->bestMoveDstRow, 'A' + g->bestMoveDstCol);
	}
}

//Executes a human player's turn:
//prints the relevant messages, gets a command and executes it according to it's specified effect.
void playerTurn(ChessGame* g) {
	ChessCommand curCmd;
	CHESS_GAME_MESSAGE curMsg;
	ChessGamePrintBoard(g);
	while (true) {
		if (g->currentPlayer == WHITE_PLAYER)
			printf("Enter your move (white player):\n");
		else
			printf("Enter your move (black player):\n");
		curCmd = readCommand();
		if (curCmd.type == 0 || curCmd.cmd == INVALID_LINE) {
			printf("ERROR: invalid command\n");
			continue;
		}
		switch (curCmd.cmd)
		{
		case MOVE:
			if (curCmd.validArg) {
				curMsg = ChessGameSetMove(g, curCmd.r1, curCmd.c1, curCmd.r2, curCmd.c2);
				if (curMsg == SUCCESS)
					return;
				else if (curMsg == INVALID_POSITION)
					printf("Invalid position on the board\n");
				else if (curMsg == NOT_PLAYER_PIECE)
					printf("The specified position does not contain your piece\n");
				else if (curMsg == ILLEGAL_MOVE)
					printf("Illegal move\n");
				else if (curMsg == KING_STILL_THREATENED)
					printf("Illegal move: king is still threatened\n");
				else if (curMsg == KING_NOW_THREATENED)
					printf("Illegal move: king will be threatened\n");
			}
			break;
		case GET_MOVES:
			if (curCmd.validArg) {
				curMsg = ChessGameGetMoves(g, curCmd.r1, curCmd.c1);
				if (curMsg == INVALID_POSITION)
					printf("Invalid position on the board\n");
				else if (curMsg == NO_PIECE_IN_SQUARE)
					printf("The specified position does not contain a player piece\n");
			}
			break;
		case SAVE:
			curMsg = ChessGameSave(g, curCmd.path);
			if (curMsg == SUCCESS)
				printf("Game saved to: %s\n", curCmd.path);
			else if (curMsg == NULL_PATH)
				printf("ERROR: invalid command\n");
			else if (curMsg == FILE_CREATE_FAILED) 
				printf("File cannot be created or modified\n");
			break;
		case UNDO:
			curMsg = ChessGameUndoPrevMove(g,true);
			if (curMsg == NO_HISTORY)
				printf("Empty history, no move to undo\n");
			ChessGameUndoPrevMove(g,true);
			if (curMsg == SUCCESS)
				return;
			break;
		case RESET:
			printf("Restarting...\n");
			chessGameReset(g);
			settingsState(g);
			return;
		case QUIT:
			quitGame(g);
			break;
		default:;
		}
	}
}




