#include "MainAux.h"
#include "SDL_main.h"
#include <stdlib.h>
#include <string.h>

/*
* This function is the main game's function. It operates using an outer 'while' loop
* that handles the game state using the function 'gameState'.
* If the user initiated the graphics mode (using -g arg), this function calls SDL_chessGame() to initiate the game.
* 1. In case of a memory error or 'quit' command, the function ends the game (frees all allocated memory).
* 2. In every turn, the inner 'while' loop checks for a winner or tie. If there's a winner or tie,
*    the game ends.
* @return
* Undefined value
*/
int main(int argc, char* argv[]) {
	ChessGame* g;
	if (argc > 2){
		printf("Too many arguments");
		exit(0);
	}
	if (argc == 2 && strcmp(argv[1], "-g") == 0) {
		return SDL_chessGame();
	}
	if (argc == 2 && strcmp(argv[1], "-c") != 0) {
		printf("wrong argument");
		exit(0);
	}
	printf(" Chess\n-------\n");
	g = ChessGameCreate(HISTORY_SIZE);
	if (g == NULL)
		exit(0);
	settingsState(g);
	while (true) {
		if (g->checkmated == WHITE_PLAYER)
			printf("Checkmate! black player wins the game\n");
		else if (g->checkmated == BLACK_PLAYER)
			printf("Checkmate! white player wins the game\n");
		else if (g->draw)
			printf("The game ends in a draw\n");
		else if (g->checked == WHITE_PLAYER)
			printf("Check: white king is threatened\n");
		else if (g->checked == BLACK_PLAYER)
			printf("Check: black king is threatened\n");
		if (g->checkmated != '\0' || g->draw) {
			ChessGameDestroy(g);
			exit(0);
		}
		gameState(g);
	}
	return 0;
}