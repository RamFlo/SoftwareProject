#include "Widget.h"

/*
* A function that destroys the widget pointed by src.
*/
void destroyWidget(Widget* src)
{
	if (src != NULL)
		src->destroy(src);
}
