#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <allegro5/allegro5.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>
#include "player.h"
#include "mylittlefighter.h"
#include "box.h"
#include "joystick.h"

ALLEGRO_BITMAP **player_load_bitmap(ALLEGRO_BITMAP  **bitmap, enum Pony player_id)
{
	char *string = malloc(sizeof(char) * 30);


	if (player_id == PINKIE) {
		for (short i = 0; i <= 28; i++) {
			sprintf(string, "./sprites/players/pinkie/pinkie_%d.png", i);
			bitmap[i] = al_load_bitmap(string);
		}
	}
	if (player_id == RARITY) {
		for (short i = 0; i <= 28; i++) {
			sprintf(string, "./sprites/players/rarity/rarity_%d.png", i);
			bitmap[i] = al_load_bitmap(string);
		}
	}
	

	free(string);
	return (bitmap);
}


struct player *player_create(enum Pony id, short x, short y, float resize)
{
	struct player *new_player = malloc(sizeof(struct player));

	new_player->id = id;
	new_player->win = 0;
	new_player->state = IDLE;
	new_player->frame = IDLE1;
	new_player->dir = RIGHT;
	new_player->hp = 100;
	new_player->dash = 100;
	new_player->vel= VEL_MAX;
	new_player->x = x;
	new_player->y = y;
	new_player->resize = resize;

	new_player->bitmap = malloc(29 * sizeof(ALLEGRO_BITMAP*));
	new_player->bitmap = player_load_bitmap(new_player->bitmap, id);	

	short side_x = al_get_bitmap_width(new_player->bitmap[0]) * resize;
	short side_y = al_get_bitmap_height(new_player->bitmap[0]) * resize;

	new_player->hurtbox = box_create(x, y, side_x/2, side_y*2/3, ACTIVE);
	new_player->hitbox = box_create(x, y, side_x/6, side_y/6, !ACTIVE);
	new_player->control = joystick_create();

	return (new_player);
}

void player_destroy(struct player *player)
{
	if (!player)
		return;

	if (player->control)
		joystick_destroy(player->control);

	if (player->hurtbox)
		box_destroy(player->hurtbox);
	
	if (player->bitmap) {
		for (short i = 0; i <= 19; i++) {
			if (player->bitmap[i])
				al_destroy_bitmap(player->bitmap[i]);
		}
		free(player->bitmap);
	}

	free(player);
}

void player_init(struct player *player, short num)
{
	player->hp = 100;
	player->control->active = 0;
	
	if (num == 1)
		player->dir = RIGHT;
	else
		player->dir = LEFT;
}

void player_draw(struct player *player, short frame, char dir)
{
	short side_x = al_get_bitmap_width(player->bitmap[frame]);
	short side_y = al_get_bitmap_height(player->bitmap[frame]);
	short new_side_x = side_x * player->resize;
	short new_side_y = side_y * player->resize;
	short x = player->x - side_x/(2*RESIZE);
	short y = player->y - side_y/(2*RESIZE);

	if (dir == RIGHT)
		al_draw_scaled_bitmap(player->bitmap[frame], 0, 0, side_x, side_y, x - new_side_x*0.3, y - new_side_y*0.4, new_side_x, new_side_y, 0);		
	else
		al_draw_scaled_bitmap(player->bitmap[frame], 0, 0, side_x, side_y, x- new_side_x*0.3, y- new_side_y*0.4, new_side_x, new_side_y, ALLEGRO_FLIP_HORIZONTAL);		
}

void player_draw_hp(short hp, short num)
{
	// define a cor da barra de saude
	short r = 0;
	short g = 204;

	if (hp < 80) r += 102; 
	if (hp < 60) r += 102;
	if (hp < 40) g -= 102;
	if (hp < 20) g -= 102;
	

	short size = (X_SCREEN*0.27) * (100 - hp)/100;
	if (num == 1)
		al_draw_filled_rectangle(X_SCREEN*0.18 + size, Y_SCREEN*0.1, X_SCREEN*0.45, Y_SCREEN*0.15, al_map_rgb(r, g, 0));		
	else
		al_draw_filled_rectangle(X_SCREEN*0.55, Y_SCREEN*0.1, X_SCREEN*0.82 - size, Y_SCREEN*0.15, al_map_rgb(r, g, 0));		


}

short player_win(struct player *player1, struct player *player2)
{
	if (player1->hp <= 0) {
		player2->win = player2->win + 1;
		return 2;
	}

	if (player2->hp <= 0) {
		player1->win = player1->win + 1;
		return 1;
	}

	return 0;
}

void player_update_joystick(struct player *player1, struct player *player2, int keycode)
{
	if (keycode == ALLEGRO_KEY_A) 
		joystick_left(player1->control);
	if (keycode == ALLEGRO_KEY_D)
		joystick_right(player1->control);
	if (keycode == ALLEGRO_KEY_W)
		joystick_up(player1->control);
	if (keycode == ALLEGRO_KEY_S)
		joystick_down(player1->control);
	if (keycode == ALLEGRO_KEY_R)
		joystick_dash(player1->control);
	if (keycode == ALLEGRO_KEY_6) 
		joystick_hit1(player1->control);
	if (keycode == ALLEGRO_KEY_7)
		joystick_hit2(player1->control);
	if (keycode == ALLEGRO_KEY_8)
		joystick_combo(player1->control);
	

	if (keycode == ALLEGRO_KEY_LEFT)
		joystick_left(player2->control);
	if (keycode == ALLEGRO_KEY_RIGHT)
		joystick_right(player2->control);
	if (keycode == ALLEGRO_KEY_UP)
		joystick_up(player2->control);
	if (keycode == ALLEGRO_KEY_DOWN)
		joystick_down(player2->control);
	if (keycode == ALLEGRO_KEY_PAD_1) 
		joystick_hit1(player2->control);
	if (keycode == ALLEGRO_KEY_PAD_2)
		joystick_hit2(player2->control);
	if (keycode == ALLEGRO_KEY_PAD_3)
		joystick_combo(player2->control);
	
}	


void player_attack(struct player *player1, struct player *player2)
{
	if (!player1->control->active || !player2->control->active)
		return;

	// player nao pode atacar durante pulo ou descida
	if ((player1->state != UP) && (player1->state != FALL) && (player1->state != DASH) && (player1->state != STUNNED)) {

		if (player1->control->hit1 && player1->state != ATTACK2) {
			player1->state = ATTACK1;
		}
		if (player1->control->hit2 && player1->state != ATTACK1) {
			player1->state = ATTACK2;
		}
		if (player1->state == ATTACK1) {

			if (player1->hitbox->active && box_collision(player1->hitbox, player2->hurtbox)) {
				player2->hp = player2->hp - 1;
				player2->state = STUNNED;
				player2->frame = STUN2;
			}

			if (player1->frame == HIT1_4) {
				player1->hitbox->active = ACTIVE;
				short move = STEPS;
				if (player1->dir == LEFT) {
					move *= -1;
				}
				player1->hitbox->x = player1->hitbox->x + move*3;
				player1->hitbox->y = player1->hitbox->y - STEPS*3/2;

			}
			if (player1->frame == HIT1_5) {
				player1->hitbox->active = !ACTIVE;
				player1->hitbox->x = player1->hurtbox->x;
				player1->hitbox->y = player1->hurtbox->y;

				if (player1->control->combo) {
					player1->state = COMBO;
					player1->frame = COMBO1;
				}
			}
			if (player1->frame == HIT1_6)
				player1->state = IDLE;
		}
		if (player1->state == ATTACK2) {

			if (player1->hitbox->active && box_collision(player1->hitbox, player2->hurtbox)) {
				player2->hp = player2->hp - 1;
				player2->state = STUNNED;
				player2->frame = STUN2;
			}

			if (player1->frame == HIT2_3) {
				player1->hitbox->active = ACTIVE;
				short move = STEPS;
				if (player1->dir == RIGHT) {
					move *= -1;
				}
				player1->hitbox->x = player1->hitbox->x + move*3;
				player1->hitbox->y = player1->hitbox->y + STEPS;

			}
			if (player1->frame == HIT2_5) {
				player1->hitbox->active = !ACTIVE;
				player1->hitbox->x = player1->hurtbox->x;
				player1->hitbox->y = player1->hurtbox->y;
				player1->state = IDLE;
			}
		}
		if (player1->state == COMBO) {

			if (player1->hitbox->active && box_collision(player1->hitbox, player2->hurtbox)) {
				player2->hp = player2->hp - 4;
				player2->state = STUNNED;
				player2->frame = STUN1;
			}

			if (player1->frame == COMBO2) {
				player1->hitbox->active = ACTIVE;
				short move = STEPS;
				if (player1->dir == LEFT) {
					move *= -1;
				}
				player1->hitbox->x = player1->hitbox->x + move*3;
				player1->hitbox->y = player1->hitbox->y - STEPS*3/2;
			}

			if (player1->frame == COMBO3) {
				player1->hitbox->active = !ACTIVE;
				player1->hitbox->x = player1->hurtbox->x;
				player1->hitbox->y = player1->hurtbox->y;
				player1->state = HIT1_6;
			}
		}
			
	}
		

}

void player_move(struct player *player1, struct player *player2, struct box *floor)
{
	if (!player1->control->active || !player2->control->active)
		return;

	if (player1->state == ATTACK1 || player1->state == ATTACK2 || player1->state == STUNNED || player1->state == COMBO)
		return;

	// salva posicao inicial
	short in_x1 = player1->x;
	short in_y1 = player1->y;

	// movimento do player 1
	if (player1->dash < 100)
		player1->dash = player1->dash + 1;


//	printf ("dash: %d \n", player1->dash);
	if (player1->control->dash && (player1->dash >= 30)) {

		player1->dash = player1->dash - 10;
		
		short dir;
		if (player1->control->right) {
			player1->dir = RIGHT;
			dir = 1;
		}
		else
			if (player1->control->left) {
				player1->dir = LEFT;
				dir = -1;
			}
			else
				dir = 0;

		float dash_vel = 1.5;
		if (dir != 0) {
			player1->hurtbox->x = player1->hurtbox->x + dash_vel * dir * STEPS;
			if (box_valid_position(player1->hurtbox) && box_valid_position(player2->hurtbox)){
				player1->x = player1->x + dash_vel * dir * STEPS;
				player1->hitbox->x = player1->hitbox->x + dash_vel * dir * STEPS;
				if (box_collision(player1->hurtbox, player2->hurtbox)) {
					player2->x = player2->x + dash_vel * dir * STEPS;
					player2->hurtbox->x = player2->hurtbox->x + dash_vel * dir * STEPS;
					player2->hitbox->x = player2->hitbox->x + dash_vel * dir * STEPS;
				}

			}
		
			else
				player1->hurtbox->x = player1->hurtbox->x -  dash_vel * dir *STEPS;
			
			player1->state = DASH;
		}
	}		
	else {	

		if (player1->control->down && box_collision(player1->hurtbox, floor)) {
			short side_y = al_get_bitmap_height(player1->bitmap[0]) * player1->resize*2/3;
			short new_y = player1->y + side_y /4;
			player1->hurtbox = box_update(player1->hurtbox, player1->hurtbox->x, new_y, player1->hurtbox->side_x, side_y /2, 1);
			player1->state = DOWN;
		}
		else {
			// se antes ele estava agachado, resetamos a hurtbox
			short side_y = al_get_bitmap_height(player1->bitmap[0]) * player1->resize*2/3;
			if (player1->hurtbox->side_y != side_y) {
				player1->hurtbox = box_update(player1->hurtbox, player1->hurtbox->x, player1->y, player1->hurtbox->side_x, side_y, 1);
			}


			if (player1->control->right) {
				player1->hurtbox->x = player1->hurtbox->x +  STEPS;
				if (box_valid_position(player1->hurtbox) && !box_collision(player1->hurtbox, player2->hurtbox)){
					player1->x = player1->x +  STEPS;
					player1->hitbox->x = player1->hitbox->x + STEPS;
				}
				else
					player1->hurtbox->x = player1->hurtbox->x -  STEPS;
			}

			if (player1->control->left) {
				player1->hurtbox->x = player1->hurtbox->x -  STEPS;
				if (box_valid_position(player1->hurtbox) && !box_collision(player1->hurtbox, player2->hurtbox)){
					player1->x = player1->x -  STEPS;
					player1->hitbox->x = player1->hitbox->x - STEPS;
				}
				else
					player1->hurtbox->x = player1->hurtbox->x +  STEPS;
			}

			// pulo inicia com a velocidade maxima 
			if (box_collision(player1->hurtbox, floor))
				player1->vel = VEL_MAX;

			// caso estiver pulando 
			if ((player1->control->up) || !box_collision(player1->hurtbox, floor)) {
				player1->hurtbox->y = player1->hurtbox->y - player1->vel * STEPS/2;
				if (box_valid_position(player1->hurtbox) && !box_collision(player1->hurtbox, player2->hurtbox)) {
					player1->y = player1->y - player1->vel *STEPS/2;
					player1->hitbox->y = player1->hitbox->y -player1->vel * STEPS/2;
					player1->vel = player1->vel - 1;
				}
				else
					player1->hurtbox->y = player1->hurtbox->y + player1->vel * STEPS/2;

			}

			// classifica stateecao final
			if (player1->y < in_y1)
				player1->state = UP;
			else
			if (player1->y > in_y1)
				player1->state = FALL;
			else
			if (player1->x > in_x1) {
				player1->state = RUN;
				player1->dir = RIGHT;
			}
			else
			if (player1->x < in_x1) {
				player1->state = RUN;
				player1->dir = LEFT;
			}
			else
				player1->state = IDLE;
		}
	}

	
}


// fazer funcao tipo animation state trigger
// pega a direcao e seta o state inicial para aquela animacao
void player_animation(struct player *player)
{
	switch (player->frame) {

		case IDLE1:
			player_draw(player, 0, player->dir);	
			break;
		case RUN1:
			player_draw(player, 1, player->dir);	
			break;
		case RUN2:
			player_draw(player, 2, player->dir);	
			break;
		case RUN3:
			player_draw(player, 3, player->dir);
			break;
		case RUN4:
			player_draw(player, 4, player->dir);
			break;
		case UP1:
			player_draw(player, 5, player->dir);
			break;
		case UP2:
			player_draw(player, 6, player->dir);
			break;
		case UP3:
			player_draw(player, 7, player->dir);
			break;
		case UP4:
			player_draw(player, 7, player->dir);
			break;
		case FALL1:
			player_draw(player, 8, player->dir);
			break;
		case FALL2:
			player_draw(player, 8, player->dir);
			break;
		case FALL3:
			player_draw(player, 9, player->dir);
			break;
		case FALL4:
			player_draw(player, 5, player->dir);
			break;
		case DOWN1:
			player_draw(player, 5, player->dir);
			break;
		case DOWN2:
			player_draw(player, 19, player->dir);
			break;
		case HIT1_1:
			player_draw(player, 10, player->dir);
			break;
		case HIT1_2:
			player_draw(player, 11, player->dir);
			break;
		case HIT1_3:
			player_draw(player, 12, player->dir);
			break;
		case HIT1_4:
			player_draw(player, 13, player->dir);
			break;
		case HIT1_5:
			player_draw(player, 14, player->dir);
			break;
		case HIT1_6:
			player_draw(player, 15, player->dir);
			break;
		case HIT2_1:
			player_draw(player, 16, player->dir);
			break;
		case HIT2_2:
			player_draw(player, 17, player->dir);
			break;
		case HIT2_3:
			player_draw(player, 18, player->dir);
			break;
		case HIT2_4:
			player_draw(player, 18, player->dir);
			break;
		case HIT2_5:
			player_draw(player, 16, player->dir);
			break;
		case DASH1:
			player_draw(player, 23, player->dir);
			break;
		case DASH2:
			player_draw(player, 24, player->dir);
			break;
		case DASH3:
			player_draw(player, 25, player->dir);
			break;
		case DASH4:
			player_draw(player, 26, player->dir);
			break;
		case DASH5:
			player_draw(player, 27, player->dir);
			break;
		case STUN1:
			player_draw(player, 20, player->dir);
			break;
		case STUN2:
			player_draw(player, 20, player->dir);
			break;
		case STUN3:
			player_draw(player, 20, player->dir);
			break;
		case STUN4:
			player_draw(player, 20, player->dir);
			break;
		case COMBO1:
			player_draw(player, 11, player->dir);
			break;
		case COMBO2:
			player_draw(player, 21, player->dir);
			break;
		case COMBO3:
			player_draw(player, 22, player->dir);
			break;
	}

}

void player_update_state(struct player *player)
{
//	printf("player state %d \n", player->state);
//	printf("player frame %d \n", player->frame);

	switch (player->frame) {

		case IDLE1:
			if (player->state == RUN)
				player->frame = RUN1;
			if (player->state == UP)
				player->frame = UP1;
			if (player->state == DOWN)
				player->frame = DOWN1;
			if (player->state == FALL)
				player->frame = FALL2;
			if (player->state == ATTACK1)
				player->frame = HIT1_1;
			if (player->state == ATTACK2)
				player->frame = HIT2_1;
			if (player->state == DASH)
				player->frame = DASH1;
			break;
		
		case RUN1:
			if (player->state == RUN)
				player->frame = RUN2;
			if (player->state == UP)
				player->frame = UP1;
			if (player->state == DOWN)
				player->frame = DOWN1;
			if (player->state == IDLE)
				player->frame = IDLE1;
			if (player->state == FALL)
				player->frame = FALL2;
			if (player->state == ATTACK1)
				player->frame = HIT1_1;
			if (player->state == ATTACK2)
				player->frame = HIT2_1;
			if (player->state == DASH)
				player->frame = DASH1;
			break;

		case RUN2:
			if (player->state == RUN)
				player->frame = RUN3;
			if (player->state == UP)
				player->frame = UP1;
			if (player->state == DOWN)
				player->frame = DOWN1;
			if (player->state == IDLE)
				player->frame = IDLE1;
			if (player->state == FALL)
				player->frame = FALL2;
			if (player->state == ATTACK1)
				player->frame = HIT1_1;
			if (player->state == ATTACK2)
				player->frame = HIT2_1;
			if (player->state == DASH)
				player->frame = DASH1;
			break;

		case RUN3:
			if (player->state == RUN)
				player->frame = RUN4;
			if (player->state == UP)
				player->frame = UP1;
			if (player->state == DOWN)
				player->frame = DOWN1;
			if (player->state == IDLE)
				player->frame = IDLE1;
			if (player->state == FALL)
				player->frame = FALL2;
			if (player->state == ATTACK1)
				player->frame = HIT1_1;
			if (player->state == ATTACK2)
				player->frame = HIT2_1;
			if (player->state == DASH)
				player->frame = DASH1;
			break;

		case RUN4:
			if (player->state == RUN)
				player->frame = RUN1;
			if (player->state == UP)
				player->frame = UP1;
			if (player->state == DOWN)
				player->frame = DOWN1;
			if (player->state == IDLE)
				player->frame = IDLE1;
			if (player->state == FALL)
				player->frame = FALL2;
			if (player->state == ATTACK1)
				player->frame = HIT1_1;
			if (player->state == ATTACK2)
				player->frame = HIT2_1;
			if (player->state == DASH)
				player->frame = DASH1;
			break;

		case UP1:
			player->frame = UP2;
			break;

		case UP2:
			player->frame = UP3;
			break;

		case UP3:
			player->frame = UP4;
			break;

		case UP4:
			player->frame = FALL1;
			break;

		case FALL1:
			if (player->state == IDLE)
				player->frame = IDLE1;
			else
				player->frame = FALL2;
			break;

		case FALL2:
			if (player->state == IDLE)
				player->frame = IDLE1;
			else
				player->frame = FALL3;
			break;

		case FALL3:
			if (player->state == IDLE)
				player->frame = IDLE1;
			else
				player->frame = FALL4;
			break;

		case FALL4:
			player->frame = IDLE1;
			break;

		case DOWN1:
			if (player->state == RUN)
				player->frame = RUN1;
			if (player->state == UP)
				player->frame = UP1;
			if (player->state == DOWN)
				player->frame = DOWN2;
			if (player->state == IDLE)
				player->frame = IDLE1;
			if (player->state == ATTACK1)
				player->frame = HIT1_1;
			if (player->state == ATTACK2)
				player->frame = HIT2_1;
			if (player->state == DASH)
				player->frame = DASH1;
			break;

		case DOWN2:
			if (player->state != DOWN)
				player->frame = DOWN1;
			if (player->state == DASH)
				player->frame = DASH1;
			break;

		case HIT1_1:
			player->frame = HIT1_2;
			break;

		case HIT1_2:
			player->frame = HIT1_3;
			break;

		case HIT1_3:
			player->frame = HIT1_4;
			break;

		case HIT1_4:
			player->frame = HIT1_5;
			break;

		case HIT1_5:
			player->frame = HIT1_6;
			if (player->state == COMBO)
				player->frame = COMBO1;
			break;

		case HIT1_6:
			if (player->state == RUN)
				player->frame = RUN1;
			if (player->state == UP)
				player->frame = UP1;
			if (player->state == DOWN)
				player->frame = DOWN1;
			if (player->state == IDLE)
				player->frame = IDLE1;
			if (player->state == ATTACK2)
				player->frame = HIT2_1;
			if (player->state == DASH)
				player->frame = DASH1;
			break;

		case HIT2_1:
			if (player->dir == RIGHT) 
				player->dir = LEFT;
			else
				player->dir = RIGHT;

			player->frame = HIT2_2;
			break;

		case HIT2_2:
			player->frame = HIT2_3;
			break;

		case HIT2_3:
			player->frame = HIT2_4;
			break;

		case HIT2_4:
			player->frame = HIT2_5;
			break;

		case HIT2_5:
			if (player->state == RUN)
				player->frame = RUN1;
			if (player->state == UP)
				player->frame = UP1;
			if (player->state == DOWN)
				player->frame = DOWN1;
			if (player->state == IDLE)
				player->frame = IDLE1;
			if (player->state == ATTACK1)
				player->frame = HIT1_1;
			if (player->dir == RIGHT) 
				player->dir = LEFT;
			else
				player->dir = RIGHT;
			if (player->state == DASH)
				player->frame = DASH1;
			break;

		case DASH1:
			player->frame = DASH2;
			break;

		case DASH2:
			player->frame = DASH3;
			break;

		case DASH3:
			player->frame = DASH4;
			break;

		case DASH4:
			player->frame = DASH5;
			break;

		case DASH5:
			player->frame = IDLE1;
			if (player->state == DASH)
				player->frame = DASH1;
			break;	

		case STUN1:
			player->frame = STUN2;
			break;

		case STUN2:
			player->frame = STUN3;
			break;

		case STUN3:
			player->frame = STUN4;
			break;

		case STUN4:
			player->frame = IDLE1;
			player->state = IDLE;
			break;

		case COMBO1:
			player->frame = COMBO2;
			break;

		case COMBO2:
			player->frame = COMBO3;
			break;

		case COMBO3:
			player->frame = HIT1_6;
			break;
	}
}

/*
void player_update_state(struct player *playerP)
{
	if (!playerP->control->active)
		return;

	switch (playerP->state) {

		case IDLE:
			if (playerP->control->right)
				playerP->state = RIGHT1;
			else
				if (playerP->control->left)
					playerP->state = LEFT1;
				else
					if (playerP->control->up)
						playerP->state = JUMP1;
			break;

		case RIGHT1:
			playerP->state = RIGHT2;
			break;

		case RIGHT2:
			if (playerP->control->right)
				playerP->state = RIGHT1;
			else 
				if (playerP->control->left)
					playerP->state = LEFT1;
				else
					if (playerP->control->up)
						playerP->state = JUMP1;
					else
						playerP->state = IDLE;
			break;

		case LEFT1:
			playerP->state = LEFT2;
			break;

		case LEFT2:
			if (playerP->control->right)
				playerP->state = RIGHT1;
			else 
				if (playerP->control->left)
					playerP->state = LEFT1;
				else
					if (playerP->control->up)
						playerP->state = JUMP1;
					else
						playerP->state = IDLE;
			break;

		case JUMP1:
			playerP->state = JUMP2;
			break;

		case JUMP2:
			playerP->state = JUMP3;
			break;

		case JUMP3:
			playerP->state = JUMP4;
			break;

		case JUMP4:
			playerP->state = JUMP5;
			break;

		case JUMP5:
			if (playerP->control->right)
				playerP->state = RIGHT1;
			else 
				if (playerP->control->left)
					playerP->state = LEFT1;
				else
					if (playerP->control->up)
						playerP->state = JUMP1;
					else
						playerP->state = IDLE;
			break;
	}
}

void player_update_position(struct player *playerP)
{


	
	switch (playerP->state) {

		case IDLE:
			box_draw(playerP->hurtbox, 255, 102, 255);
	//		player_draw(playerP);
			break;
		case RIGHT1:
			player_move(playerP, 1, RIGHT);
			box_draw(playerP->hurtbox, 255, 102, 255);
	//		player_draw(playerP);
			break;
		case RIGHT2:
			player_move(playerP, 1, RIGHT);
			box_draw(playerP->hurtbox, 255, 102, 255);
	//		player_draw(playerP);
			break;
		case LEFT1:
			player_move(playerP, 1, LEFT);
			box_draw(playerP->hurtbox, 255, 102, 255);
	//		player_draw(playerP);
			break;
		case LEFT2:
			player_move(playerP, 1, LEFT);
			box_draw(playerP->hurtbox, 255, 102, 255);
	//		player_draw(playerP);
			break;
		case JUMP1:
			box_draw(playerP->hurtbox, 255, 102, 255);
	//		player_draw(playerP);
			break;
		case JUMP2:
			player_move(playerP, 2, UP);
			box_draw(playerP->hurtbox, 255, 102, 255);
	//		player_draw(playerP);
			break;
		case JUMP3:
			player_move(playerP, 2, UP);
			box_draw(playerP->hurtbox, 255, 102, 255);
	//		player_draw(playerP);
			break;
		case JUMP4:
			player_move(playerP, 2, DOWN);
			box_draw(playerP->hurtbox, 255, 102, 255);
	//		player_draw(playerP);
			break;
		case JUMP5:
			player_move(playerP, 2, DOWN);
			box_draw(playerP->hurtbox, 255, 102, 255);
	//		player_draw(playerP);
			break;
	}
}*/
