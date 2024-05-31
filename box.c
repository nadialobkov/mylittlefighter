#include <stdio.h>
#include <stdlib.h>
#include "box.h" 


struct box_create(short x, short y, short side_x, short side_x, char active)
{
	struct *new_box = malloc(sizeof(struct box));

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


char box_collision(struct box *box1, struct box *box2)
{
	if ( ( (((box1->y + box1->side_y /2) > (box2->y - box2->side_y /2)) && 
		   ((box2->y - box2->side_y /2) > (box1->y - box1->side_y /2))) ||
		 ( ((box2->y + box2->side_y /2) > (box1->y - box1->side_y /2)) &&
		   ((box1->y - box1->side_y /2) > (box2->y - box2->side_y /2))) ) 
		
		&&

	     ( (((box1->x + box1->side_x /2) > (box2->x - box2->side_x /2)) && 
		   ((box2->x - box2->side_x /2) > (box1->x - box1->side_x /2))) ||
		 ( ((box2->x + box2->side_x /2) > (box1->x - box1->side_x /2)) &&
		   ((box1->x - box1->side_x /2) > (box2->x - box2->side_x /2)) ) ) )

		return 1;

	else
		return 0;
}
