#include "MainAux.h"

/*
* This function is the main game's function. It operates using an outer 'while' loop
* that handles creating a new game (for the first game and every time that the game is restatred),
* and an inner 'while' loop that handles a single game's turns.

* 1. In case of a memory error or 'quit' command, the function ends the game using endGame (frees all
*	 allocated memory).
* 2. In every turn, the inner 'while' loop checks for a winner or tie. If there's a winner or tie,
*    only the 'restart'\'quit'\'undo_move' commands are accepted.
* 3. If there's no winner\tie, the function executes the user's or the computer's
*    turn according to the current game's state. (In the user's turn, the function also
*    prints the board).

* @return
* Undefined value
*/
int main() {
	SPFiarGame * curGame = NULL;
	SPCommand curCommand;
	int maxDepth = 0;
	while (true) {
		curGame = spFiarGameCreate(HISTORY_SIZE);
		if (curGame == NULL)
			endGame(curGame, true);
		maxDepth = getMaxDepth();
		//1
		if (maxDepth == -1)
			endGame(curGame, false);
		else if (maxDepth == -2)
			endGame(curGame, true);
		while (true) {
			//2
			if (spFiarCheckWinner(curGame) != '\0') {
				spFiarGamePrintBoard(curGame);
				printWinner(curGame);
				curCommand = readCommand(curGame);
				while (curCommand.cmd != SP_RESTART && curCommand.cmd != SP_QUIT && curCommand.cmd != SP_UNDO_MOVE) {
					if (curCommand.cmd == SP_INVALID_LINE)
						printf("Error: invalid command\n");
					else
						printf("Error: the game is over\n");
					curCommand = readCommand(curGame);
				}
				if (curCommand.cmd == SP_QUIT)
					endGame(curGame, false);
				else if (curCommand.cmd == SP_RESTART) {
					spFiarGameDestroy(curGame);
					break;
				}
				else {
					if (spFiarCheckWinner(curGame) != SP_FIAR_GAME_PLAYER_1_SYMBOL)
						spFiarGameUndoPrevMove(curGame);
					spFiarGameUndoPrevMove(curGame);
					continue;
				}
			}
			//3
			if (spFiarGameGetCurrentPlayer(curGame) == SP_FIAR_GAME_PLAYER_2_SYMBOL) {
				computerTurn(curGame, maxDepth);
				continue;
			}
			else {
				spFiarGamePrintBoard(curGame);
				printf("Please make the next move:\n");
				if (doUserCommand(curGame, maxDepth) == -1)
					break;
				else
					continue;
			}
		}
		printf("Game restarted!\n");
	}
}