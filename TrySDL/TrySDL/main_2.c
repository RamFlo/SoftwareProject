#include <SDL.h>
#include <SDL_video.h>
#include <stdio.h>

int main5(int argc, char *argv[]) {
	// initialize SDL2 for video
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		printf("ERROR: unable to init SDL: %s\n", SDL_GetError());
		return 1;
	}

	// create an SDL window
	SDL_Window* window = SDL_CreateWindow(
		"Title",
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		600,
		600,
		SDL_WINDOW_OPENGL);

	// make sure window was created successfully
	if (window == NULL) {
		printf("ERROR: unable to create window: %s\n", SDL_GetError());
		SDL_Quit();
		return 0;
	}

	// create a renderer for the window
	SDL_Renderer* rend = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	if (rend == NULL) {
		printf("ERROR: unable to create renderer: %s\n", SDL_GetError());
		SDL_DestroyWindow(window);
		SDL_Quit();
		return 0;
	}
	// ensure renderer supports transparency
	SDL_SetRenderDrawBlendMode(rend, SDL_BLENDMODE_BLEND);

	SDL_Rect rect = { .x = 150,.y = 150,.w = 300,.h = 300 };

	// clear window to color red (r,g,b,a)
	SDL_SetRenderDrawColor(rend, 255, 0, 0, 0);
	SDL_RenderClear(rend);

	// draw a blue rectangle
	SDL_SetRenderDrawColor(rend, 0, 0, 255, 0);
	SDL_RenderFillRect(rend, &rect);

	// load an image onto an SDL surface
	SDL_Surface* surf = SDL_LoadBMP("Chess.bmp");
	if (surf == NULL) {
		printf("ERROR: unable to load image: %s\n", SDL_GetError());
		SDL_DestroyRenderer(rend);
		SDL_DestroyWindow(window);
		SDL_Quit();
		return 0;
	}
	// set a specific color (white: 255,255,255) to transparent
	SDL_SetColorKey(surf, SDL_TRUE, SDL_MapRGB(surf->format, 255, 255, 255));

	// create a texture from the surface image
	SDL_Texture* tex = SDL_CreateTextureFromSurface(rend, surf);
	if (tex == NULL) {
		printf("ERROR: unable to create texture: %s\n", SDL_GetError());
		SDL_FreeSurface(surf);
		SDL_DestroyRenderer(rend);
		SDL_DestroyWindow(window);
		SDL_Quit();
		return 0;
	}
	// surface not needed anymore - free it
	SDL_FreeSurface(surf);

	// draw entire texture (NULL) to part of window (rect)
	SDL_RenderCopy(rend, tex, NULL, &rect);

	// present changes to user
	SDL_RenderPresent(rend);

	SDL_Delay(10000);

	// free everything and finish
	SDL_DestroyTexture(tex);
	SDL_DestroyRenderer(rend);
	SDL_DestroyWindow(window);
	SDL_Quit();
	return 0;
}
