#ifndef _SQUARE_H_	
#define _SQUARE_H_

#include "joystick.h"

#define STEP 10

// ESTRUTURA ============================================================

struct square {
	short side_x;
	short side_y;
	short x;
	short y;
	struct joystick *control;
};

// FUNCOES =============================================================


struct square* square_create(short side_x, short side_y, short x, short y, short x_max, short y_max);

void square_destroy(struct square *element);

// trajetoria: 0 = direita
//             1 = esquerda
//             2 = cima
//             3 = baixo
void square_move(struct square *element, short steps, char traj, short x_max, short y_max);

#endif
