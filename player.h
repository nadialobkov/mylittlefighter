#ifndef _PLAYER_H_
#define _PLAYER_H_

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
#define DOWN 7

// ESTRUTURA  ======================================================

struct player {
	char id;
	short state;
	short health;
	short x;
	short y;
	struct box *hitbox;
	struct joystick *control;
	ALLEGRO_BITMAP **bitmap;
}

// FUNCOES =========================================================

struct player_create(char id, short state, short health, short x, short y);

void player_destroy(struct player *playerD);

void player_update_position(struct player *playerP);



#endif
