#ifndef WIDGET_H_
#define WIDGET_H_

#include <SDL.h>
#include <SDL_video.h>

/*
* A widget 'object' that is drawn on screen using rednerer, used as a containing type for a button.
*/
typedef struct widget_t Widget;
struct widget_t {
	void(*draw)(Widget*, SDL_Renderer*);
	void(*handleEvent)(Widget*, SDL_Event*);
	void(*destroy)(Widget*);
	void* data;
};

/*
* A function that destroys the widget pointed by src.
*/
void destroyWidget(Widget* src);

#endif /* WIDGET_H_ */
