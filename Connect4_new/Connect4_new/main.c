#include "SPMainAux.h"

int main() {
	SPFiarGame * curGame = NULL;
	SPCommand curCommand;
	int maxDepth = 0;
	while (true) {
		curGame = spFiarGameCreate(HISTORY_SIZE);
		if (curGame == NULL)
			endGame(curGame, true);
		maxDepth = getMaxDepth();
		if (maxDepth == -1)
			endGame(curGame, false);
		else if (maxDepth == -2)
			endGame(curGame, true);
		while (true) {
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