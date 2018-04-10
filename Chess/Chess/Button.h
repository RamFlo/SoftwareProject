#ifndef BUTTON_H_
#define BUTTON_H_

#include "Widget.h"
#include <SDL.h>
#include <SDL_video.h>

/**
A button 'object', which is also a widget. Linked to a specific renderer, texture and location. Upon clicking it, 'action' function is called.
*/
typedef struct button_t Button;
struct button_t {
	SDL_Renderer* render;
	SDL_Texture* texture;
	SDL_Rect location;
	void(*action)(void);
};

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
	void(*action)(void));
void destroyButton(Widget* src);
void handleButtonEvent(Widget* src, SDL_Event* event);
void drawButton(Widget*, SDL_Renderer*);

#endif /* BUTTON_H_ */
