#ifndef SDL_MAIN_H_
#define SDL_MAIN_H_

/*
* This function is the main game's function for graphic mode.
* It created a new game and all the game program's windows using initializeAllWindows.
* It operates using an outer 'while' loop the polls the current event and acts accordignly (changing and rednering screens).
* In case of a memory error the function ends the game (frees all allocated memory).
* @return
* Undefined value
*/
int SDL_chessGame();
#endif