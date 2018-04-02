#include <SDL.h>
#include <SDL_video.h>
#include <stdio.h>

#define SPRT_W 7
#define SPRT_LEN 27

int main2(int argc, char *argv[]) {
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
		SDL_WINDOW_SHOWN);
	//SDL_WINDOW_OPENGL

	// make sure window was created successfully
	if (window == NULL) {
		printf("ERROR: unable to create window: %s\n", SDL_GetError());
		SDL_Quit();
		return 0;
	}

	// create a renderer for the window
	SDL_Renderer* rend = SDL_CreateRenderer(window, -1, SDL_RENDERER_SOFTWARE);
	//SDL_RENDERER_ACCELERATED
	if (rend == NULL) {
		printf("ERROR: unable to create renderer: %s\n", SDL_GetError());
		SDL_DestroyWindow(window);
		SDL_Quit();
		return 0;
	}
	// ensure renderer supports transparency
	SDL_SetRenderDrawBlendMode(rend, SDL_BLENDMODE_BLEND);

	// sprite position & size details
	SDL_Rect pos = { .x = 200,.y = 200,.w = 74,.h = 86 };
	SDL_Rect sprt = { .x = 0,.y = 0,.w = 74,.h = 86 };
	int sprtCurr = 0;
	int xDiff = 0;
	int yDiff = 0;

	// load an image onto an SDL surface
	SDL_Surface* surf = SDL_LoadBMP("braid.bmp");
	if (surf == NULL) {
		printf("ERROR: unable to load image: %s\n", SDL_GetError());
		SDL_DestroyRenderer(rend);
		SDL_DestroyWindow(window);
		SDL_Quit();
		return 0;
	}
	// set a specific color (white: 255,255,255) to transparent
	SDL_SetColorKey(surf, SDL_TRUE, SDL_MapRGB(surf->format, 255, 255, 0));

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

	int done = 0;
	int flip = 0;
	Uint32 curr, last = SDL_GetTicks();

	// event handling loop
	SDL_Event e;
	while (!done) {
		while (SDL_PollEvent(&e)) {
			switch (e.type) {
			case SDL_QUIT:
				done = 1;
				break;
			case SDL_KEYDOWN:
				switch (e.key.keysym.sym) {
				case SDLK_ESCAPE:
					done = 1;
					break;
				case SDLK_LEFT:
					flip = 1;
					xDiff = -3;
					break;
				case SDLK_RIGHT:
					flip = 0;
					xDiff = 3;
					break;
				}
				break;
			case SDL_KEYUP:
				switch (e.key.keysym.sym) {
				case SDLK_LEFT:
					if (xDiff < 0) xDiff = 0;
					break;
				case SDLK_RIGHT:
					if (xDiff > 0) xDiff = 0;
					break;
				}
				break;
			case SDL_MOUSEBUTTONUP:
				if ((e.button.x > pos.x) && (e.button.y > pos.y) &&
					(e.button.x < pos.x + pos.w) && (e.button.y < pos.y + pos.h))
					done = 1;
				break;
			}
		}
		pos.x += xDiff;
		pos.y += yDiff;

		// check if time to move to next sprite
		curr = SDL_GetTicks();
		if (curr - last > 30) {
			// update sprite rect details to print correct frame
			last = curr;
			if (xDiff == 0)
				sprtCurr = SPRT_LEN;
			else
				sprtCurr = (sprtCurr + 1) % SPRT_LEN;
			sprt.x = (sprtCurr % SPRT_W) * sprt.w;
			sprt.y = (sprtCurr / SPRT_W) * sprt.h;
		}


		//SDL_HideWindow(window);
		//SDL_ShowWindow(window);
		// clear window to color red (r,g,b,a)
		SDL_SetRenderDrawColor(rend, 255, 0, 0, 0);
		SDL_RenderClear(rend);

		// draw texture part (sprt) to part of window (pos)
		if (!flip)
			SDL_RenderCopy(rend, tex, &sprt, &pos);
		else
			SDL_RenderCopyEx(rend, tex, &sprt, &pos, 0, NULL, SDL_FLIP_HORIZONTAL);


		// present changes to user
		SDL_RenderPresent(rend);

		// wait a little before next frame
		SDL_Delay(10);
	}

	// free everything and finish
	SDL_DestroyTexture(tex);
	SDL_DestroyRenderer(rend);
	SDL_DestroyWindow(window);
	SDL_Quit();
	return 0;
}
