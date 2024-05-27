#include <stdio.h>
#include <stdlib.h>
#include "joystick.h"


struct joystick *joystick_create()
{
	struct joystick *new_js = malloc(sizeof(struct joystick));

	new_js->right = 0;
	new_js->left = 0;
	new_js->up = 0;
	new_js->down = 0;

	return (new_js);
}


void joystick_destroy(struct joystick *element)
{
	free(element);
}


void joystick_right(struct joystick *element)
{
	element->right = element->right ^ 1;
}

void joystick_left(struct joystick *element)
{
	element->left = element->left ^ 1;
}

void joystick_up(struct joystick *element)
{
	element->up = element->up ^ 1;
}

void joystick_down(struct joystick *element)
{
	element->down = element->down ^ 1;
}

