#ifndef _JOYSTICK_H_
#define _JOYSTICK_H_

// ESTRUTURA ==========================================

// flags
// 0 inativo, 1 ativo
struct joystick {
	char right;
	char left;
	char up;
	char down;
};

// FUNCOES ============================================

struct joystick *joystick_create();

void joystick_destroy(struct joystick *element);

void joystick_right(struct joystick *element);

void joystick_left(struct joystick *element);

void joystick_up(struct joystick *element);

void joystick_down(struct joystick *element);


#endif
