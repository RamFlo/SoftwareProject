#include "Button.h"
#include <stdio.h>
#include <stdlib.h>
#include "SDL_MainAux.h"
#include <string.h>

/**
* Returns pointer to a button created using the supplied arguments,
* with the assertion that all arguments are valid. If any of the arguments is
* invalid then NULL is returned.
* @param renderer - a pointer to the button's renderer
* @param image - the button's image given as a string
* @param location - the button's rect
* @param (*action)(void) - the button's function to execute upon click
* @return
* NULL on invalid args or memory error
* Otherwise, a pointer to the created button
*/
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
	if (strncmp(image, "assets/pieces/black", 19) == 0) {
		if (SDL_SetColorKey(surface, SDL_TRUE, SDL_MapRGB(surface->format, 0, 0, 0)) < 0) {
			free(res);
			free(data);
			SDL_FreeSurface(surface);
			return NULL;
		}
	}
	if (strncmp(image, "assets/pieces/white", 19) == 0 || strncmp(image, "assets/SquareBG", 15) == 0) {
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

/**
* Destroys the given button (frees all the button's resources)
* @param src - a pointer to the button which should be destroyed
*/
void destroyButton(Widget* src)
{
	Button* button = (Button*)src->data;
	SDL_DestroyTexture(button->texture);
	free(button);
	free(src);
}

/**
* A function called upon a button's click. 
* Updates the last click point, and calls the button action if the click was inside the button.
* @param src - a pointer to the button
* @param e - a pointer to an event
*/
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

/**
* A function used to draw a button using the renderer given.
* @param src - a pointer to the button
* @param render - a pointer to a renderer
*/
void drawButton(Widget* src, SDL_Renderer* render)
{
	Button* button = (Button*)src->data;
	SDL_RenderCopy(render, button->texture, NULL, &button->location);
}
