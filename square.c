#include <stdio.h>
#include <stdlib.h>
#include "square.h"

struct square* square_create(short side_x, short side_y, short x, short y, short x_max, short y_max)
{
	short top, bottom, left, right; // posicao das bordas do quadrilatero
	
	top = y + (side_y / 2);
	bottom = y - (side_y / 2);
	left = x - (side_x / 2);
	right = x + (side_x / 2);

	// verifica se quadrilatero esta em uma posicao valida	
	if (left < 0 || bottom < 0 || right > x_max || top > y_max)
		return NULL; 

	struct square *new_square = malloc(sizeof(struct square));
	new_square->side_x = side_x;
	new_square->side_y = side_y;
	new_square->x = x;
	new_square->y = y;

	return new_square;
}


void square_destroy(struct square *element)
{
	free(element);
}
	

void square_move(struct square *element, short steps, char traj, short x_max, short y_max)
{
	switch (traj) {

		case 0: // movimento para direita
			if (((element->x + steps*STEP) + element->side_x/2) <= x_max)
				element->x = element->x + steps*STEP;	
			break;
		case 1: // movimento para esquerda
			if (((element->x - steps*STEP) - element->side_x/2) >= 0)
				element->x = element->x - steps*STEP;
			break;
		case 2: // movimento para cima
			if (((element->y + steps*STEP) + element->side_y/2) <=  y_max)
				element->y = element->y + steps*STEP;
			break;
		case 3: // movimento para baixo
			if (((element->y - steps*STEP) - element->side_y/2) >= 0)
				element->y = element->y - steps*STEP;
			break;
	}
}
