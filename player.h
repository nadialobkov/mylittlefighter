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
enum Frames {
	IDLE1,
	RUN1,RUN2,RUN3,RUN4,
	UP1,UP2,UP3,UP4,
	FALL1,FALL2,FALL3,FALL4,
	DOWN1,DOWN2,
	DASH1,DASH2,DASH3,DASH4,DASH5,
	HIT1_1,HIT1_2,HIT1_3,HIT1_4,HIT1_5,HIT1_6,
	HIT2_1,HIT2_2,HIT2_3,HIT2_4,HIT2_5,
	STUN1,STUN2,STUN3,STUN4,
	COMBO1,COMBO2,COMBO3,
};

enum State {
	IDLE,
	RUN,
	UP,
	DOWN,
	FALL,
	DASH,
	ATTACK1,
	ATTACK2,
	STUNNED,
	COMBO,
};

#define RIGHT 0
#define LEFT 1

#define STEPS 15 
#define RESIZE 10
#define VEL_MAX 8
#define VEL_MIN -8


// ESTRUTURA  ======================================================

struct player {
	enum Pony id;
	short hp;
	short win;
	short vel;
	short dash;
	short x;
	short y;
	float resize;
	char dir;
	enum State state;
	enum Frames frame;
	struct box *hurtbox;
	struct box *hitbox;
	struct joystick *control;
	ALLEGRO_BITMAP **bitmap;
};

// FUNCOES =========================================================

struct player *player_create(enum Pony id, short x, short y, float resize);

void player_destroy(struct player *playerD);

void player_draw_hp(short hp, short num);

void player_update_joystick(struct player *player1, struct player *player2, int keycode);

void player_attack(struct player *player1, struct player *player2);

void player_move(struct player *player1, struct player *player2, struct box *floor);

void player_animation(struct player *player);

void player_update_state(struct player *player);

short player_win(struct player *player1, struct player *player2);

void player_init(struct player *player, short num);


//void player_update_position(struct player *playerP);



#endif
