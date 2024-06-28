#include <stdio.h>
#include <stdlib.h>
#include "joystick.h"


struct joystick *joystick_create()
{
	struct joystick *new_js = malloc(sizeof(struct joystick));

	joystick_reset(new_js);

	return (new_js);
}


void joystick_destroy(struct joystick *control)
{
	free(control);
}

void joystick_reset(struct joystick *control)
{
	control->right = 0;
	control->left = 0;
	control->up = 0;
	control->down = 0;
	control->dash = 0;
	control->hit1 = 0;
	control->hit2 = 0;
	control->combo = 0;
	control->active = 0;
}

void joystick_right(struct joystick *control)
{
	if (control->active)
		control->right = control->right ^ 1;
}

void joystick_left(struct joystick *control)
{
	if (control->active)
		control->left = control->left ^ 1;
}

void joystick_up(struct joystick *control)
{
	if (control->active)
		control->up = control->up ^ 1;
}

void joystick_down(struct joystick *control)
{
	if (control->active)
		control->down = control->down ^ 1;
}

void joystick_dash(struct joystick *control)
{
	if (control->active)
		control->dash= control->dash ^ 1;
}

void joystick_hit1(struct joystick *control)
{
	if (control->active)
		control->hit1 = control->hit1 ^ 1;
}

void joystick_hit2(struct joystick *control)
{
	if (control->active)
		control->hit2 = control->hit2 ^ 1;
}

void joystick_combo(struct joystick *control)
{
	if (control->active)
		control->combo = control->combo ^ 1;
}
