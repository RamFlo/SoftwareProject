#include "Button.h"
#include <stdio.h>
#include <stdlib.h>
#include "SDL_MainAux.h"
#include <string.h>

Widget* createButton(
	SDL_Renderer* renderer,
	const char* image,
	SDL_Rect location,
	void(*action)(void))
{
	// allocate data
	Widget* res = (Widget*)malloc(sizeof(Widget));
	if (res == NULL)
		return NULL;

	Button* data = (Button*)malloc(sizeof(Button));
	if (data == NULL) {
		free(res);
		return NULL;
	}

	// we use the surface as a temp var
	SDL_Surface* surface = SDL_LoadBMP(image);
	if (surface == NULL) {
		free(res);
		free(data);
		return NULL;
	}
	//transperency

	if (strncmp(image, "assets/pieces/white", 19) == 0 || strncmp(image, "assets/pieces/black", 19) == 0 || strncmp(image, "assets/SquareBG", 15) == 0) {
		if (SDL_SetColorKey(surface, SDL_TRUE, SDL_MapRGB(surface->format, 255, 255, 255)) < 0) {
			free(res);
			free(data);
			SDL_FreeSurface(surface);
			return NULL;
		}
	}

	SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
	if (texture == NULL) {
		free(res);
		free(data);
		SDL_FreeSurface(surface);
		return NULL;
	}

	// surface is not needed after texture is created
	SDL_FreeSurface(surface);

	// store button & widget details
	data->texture = texture;
	data->location = location;
	data->action = action;
	res->destroy = destroyButton;
	res->draw = drawButton;
	res->handleEvent = handleButtonEvent;
	res->data = data;
	return res;
}

void destroyButton(Widget* src)
{
	Button* button = (Button*)src->data;
	SDL_DestroyTexture(button->texture);
	free(button);
	free(src);
}

void handleButtonEvent(Widget* src, SDL_Event* e)
{
	if (e->type == SDL_MOUSEBUTTONUP) {
		Button* button = (Button*)src->data;
		SDL_Point point = { .x = e->button.x,.y = e->button.y };
		lastClickPoint.x = e->button.x;
		lastClickPoint.y = e->button.y;
		curEvent = e;
		if (SDL_PointInRect(&point, &button->location)) {
			button->action();
		}
	}
}

void drawButton(Widget* src, SDL_Renderer* render)
{
	Button* button = (Button*)src->data;
	SDL_RenderCopy(render, button->texture, NULL, &button->location);
}
