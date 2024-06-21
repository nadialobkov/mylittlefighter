#include <stdio.h>
#include <stdlib.h>
#include <allegro5/allegro_primitives.h>
#include "box.h" 
#include "mylittlefighter.h"


struct box *box_create(short x, short y, short side_x, short side_y, char active)
{
	struct box *new_box = malloc(sizeof(struct box));

	new_box->x = x;
	new_box->y = y;
	new_box->side_x = side_x;
	new_box->side_y = side_y;
	new_box->active = active;
	
	return (new_box);
}


void box_destroy(struct box *box)
{
	free(box);
}

void box_draw(struct box *box1, short r, short g, short b)
{
	al_draw_filled_rectangle(box1->x - box1->side_x /2, box1->y - box1->side_y /2,
									 box1->x + box1->side_x /2, box1->y + box1->side_y /2, 
									 al_map_rgb(r, g, b));		
}

struct box *box_update(struct box *box1, short x, short y, short side_x, short side_y, char active)
{
	box1->x = x;
	box1->y = y;
	box1->side_x = side_x;
	box1->side_y = side_y;
	box1->active = active;

	return box1;
}


char box_valid_position(struct box *box1)
{
	if ((box1->x - box1->side_x /2 < 0) || (box1->y - box1->side_y /2 < 0) || 
		(box1->x + box1->side_x /2 > X_SCREEN) || (box1->y + box1->side_y /2 > Y_SCREEN) )

		return 0;	
	else
		return 1;
}

char box_collision(struct box *box1, struct box *box2)
{
	if ( ( (((box1->y + box1->side_y /2) >= (box2->y - box2->side_y /2)) && 
		   ((box2->y - box2->side_y /2) >= (box1->y - box1->side_y /2))) ||
		 ( ((box2->y + box2->side_y /2) >= (box1->y - box1->side_y /2)) &&
		   ((box1->y - box1->side_y /2) >= (box2->y - box2->side_y /2))) ) 
		
		&&

	     ( (((box1->x + box1->side_x /2) >= (box2->x - box2->side_x /2)) && 
		   ((box2->x - box2->side_x /2) >= (box1->x - box1->side_x /2))) ||
		 ( ((box2->x + box2->side_x /2) >= (box1->x - box1->side_x /2)) &&
		   ((box1->x - box1->side_x /2) >= (box2->x - box2->side_x /2)) ) ) )

		return 1;

	else
		return 0;
}
