#ifndef _JOYSTICK_H_
#define _JOYSTICK_H_

#define ACTIVE 1
#define DISABLED 0

// ESTRUTURA ==========================================

struct joystick {
	char right;
	char left;
	char up;
	char down;
	char dash;
	char hit1;
	char hit2;
	char combo;
	char active;
};

// FUNCOES ============================================

struct joystick *joystick_create();

void joystick_destroy(struct joystick *element);

void joystick_reset(struct joystick *control);

void joystick_right(struct joystick *element);

void joystick_left(struct joystick *element);

void joystick_up(struct joystick *element);

void joystick_down(struct joystick *element);

void joystick_dash(struct joystick *element);

void joystick_hit1(struct joystick *element);

void joystick_hit2(struct joystick *element);

void joystick_combo(struct joystick *element);


#endif
