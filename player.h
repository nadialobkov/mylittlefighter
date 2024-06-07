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
#define IDLE 0
#define RIGHT1 1
#define RIGHT2 2
#define LEFT1 3
#define LEFT2 4
#define JUMP1 5
#define JUMP2 6
#define JUMP3 7
#define JUMP4 8
#define JUMP5 9
#define DOWN1 10

#define STEPS 15

//direcoes
#define RIGHT 0
#define LEFT 1
#define UP 2
#define DOWN 3

#define RESIZE 6

// ESTRUTURA  ======================================================

struct player {
	char id;
	short state;
	short health;
	short x;
	short y;
	float resize;
	struct box *hitbox;
	struct joystick *control;
	ALLEGRO_BITMAP **bitmap;
};

// FUNCOES =========================================================

struct player *player_create(short x, short y, float resize);

void player_destroy(struct player *playerD);

void player_update_state(struct player *playerP);

void player_update_position(struct player *playerP);



#endif
