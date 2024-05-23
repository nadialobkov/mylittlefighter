#ifndef _SQUARE_H_	
#define _SQUARE_H_

#define STEP 20

// ESTRUTURA ============================================================

struct square {
	short side_x;
	short side_y;
	short x;
	short y;
};

// FUNCOES =============================================================


struct square* square_create(short side_x, short side_y, short x, short y, short x_max, short y_max);

void square_destroy(struct square *element);

void square_move(struct square *element, short steps, char traj, short x_max, short y_max);

#endif
