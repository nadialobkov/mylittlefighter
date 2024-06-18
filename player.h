#ifndef _PLAYER_H_
#define _PLAYER_H_

#include <allegro5/allegro5.h>
#include <allegro5/allegro_image.h>
#include "box.h"
#include "joystick.h"

// players id 
#define PINKIE 0
#define RARITY 1

// estados do personagem
enum State {
	IDLE_R,
	IDLE_L,
	RIGHT1,
	RIGHT2,
	RIGHT3,
	RIGHT4,
};

enum Direction {
	RIGHT,
	LEFT, 
	UP,
	DOWN,
	IDLE,
};

#define STEPS 15 
#define RESIZE 10
#define VEL_MAX 5
#define VEL_MIN -5


// ESTRUTURA  ======================================================

struct player {
	char id;
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

struct player *player_create(short x, short y, float resize);

void player_destroy(struct player *playerD);

void player_update_joystick(struct player *player1, struct player *player2, int keycode);

void player_move(struct player *player1, struct player *player2, struct box *floor);

//void player_update_state(struct player *playerP);

//void player_update_position(struct player *playerP);



#endif
