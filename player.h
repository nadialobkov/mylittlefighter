#ifndef _PLAYER_H_
#define _PLAYER_H_

#include <allegro5/allegro5.h>
#include <allegro5/allegro_image.h>
#include "box.h"
#include "joystick.h"

// players id 
enum Pony {
	PINKIE,
	RARITY,
};

// estados do personagem
enum State {
	IDLE_R,
	IDLE_L,
	RIGHT1,
	RIGHT2,
	RIGHT3,
	RIGHT4,
	LEFT1,
	LEFT2,
	LEFT3,
	LEFT4,
	UP_R1,
	UP_R2,
	UP_R3,
	UP_R4,
	FALL_R1,
	FALL_R2,
	FALL_R3,
	FALL_R4,
	UP_L1,
	UP_L2,
	UP_L3,
	UP_L4,
	FALL_L1,
	FALL_L2,
	FALL_L3,
	FALL_L4,
	DOWN_R1,
	DOWN_R2,
	DOWN_L1,
	DOWN_L2,
};

enum Direction {
	RIGHT,
	LEFT, 
	UP,
	DOWN,
	FALL,
	IDLE,
};

#define STEPS 15 
#define RESIZE 10
#define VEL_MAX 8
#define VEL_MIN -8


// ESTRUTURA  ======================================================

struct player {
	enum Pony id;
	short hp;
	short vel;
	short x;
	short y;
	float resize;
	enum Direction dir;
	enum State state;
	struct box *hitbox;
	struct joystick *control;
	ALLEGRO_BITMAP **bitmap;
};

// FUNCOES =========================================================

struct player *player_create(enum Pony id, short x, short y, float resize);

void player_destroy(struct player *playerD);

void player_update_joystick(struct player *player1, struct player *player2, int keycode);

void player_move(struct player *player1, struct player *player2, struct box *floor);

void player_animation(struct player *player);

void player_update_state(struct player *player);

//void player_update_position(struct player *playerP);



#endif
