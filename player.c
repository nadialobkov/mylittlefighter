#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <allegro5/allegro5.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>
#include "player.h"
#include "box.h"
#include "joystick.h"
#include "streetfighter.h"

ALLEGRO_BITMAP **player_load_bitmap(ALLEGRO_BITMAP  **bitmap, enum Pony player_id)
{
	char *string = malloc(sizeof(char) * 30);


	if (player_id == PINKIE) {
		for (short i = 0; i <= 19; i++) {
			sprintf(string, "./sprites/players/pinkie/pinkie_%d.png", i);
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
	new_player->state = IDLE;
	new_player->frame = IDLE1;
	new_player->dir = RIGHT;
	new_player->hp = 100;
	new_player->vel= VEL_MAX;
	new_player->x = x;
	new_player->y = y;
	new_player->resize = resize;

	new_player->bitmap = malloc(20 * sizeof(ALLEGRO_BITMAP*));
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


void player_draw(struct player *player, short frame, short flag)
{
	short side_x = al_get_bitmap_width(player->bitmap[frame]);
	short side_y = al_get_bitmap_height(player->bitmap[frame]);
	short new_side_x = side_x * player->resize;
	short new_side_y = side_y * player->resize;
	short x = player->x - side_x/(2*RESIZE);
	short y = player->y - side_y/(2*RESIZE);

	if (!flag)
		al_draw_scaled_bitmap(player->bitmap[frame], 0, 0, side_x, side_y, x - new_side_x/4, y - new_side_y/3, new_side_x, new_side_y, 0);		
	else
		al_draw_scaled_bitmap(player->bitmap[frame], 0, 0, side_x, side_y, x- new_side_x/4, y- new_side_y/3, new_side_x, new_side_y, ALLEGRO_FLIP_HORIZONTAL);		
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
	

	short size = (X_SCREEN - X_SCREEN/5) * (100 - hp)/100;
	if (num == 1)
		al_draw_filled_rectangle(X_SCREEN/5 + size, Y_SCREEN/8, X_SCREEN, Y_SCREEN/5, al_map_rgb(r, g, 0));		
	else
		al_draw_filled_rectangle(X_SCREEN*3/2, Y_SCREEN/8, X_SCREEN*8/5 + size, Y_SCREEN/5, al_map_rgb(r, g, 0));		


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
	if (keycode == ALLEGRO_KEY_4) 
		joystick_hit1(player1->control);
	if (keycode == ALLEGRO_KEY_5)
		joystick_hit2(player1->control);
	

	if (keycode == ALLEGRO_KEY_LEFT)
		joystick_left(player2->control);
	if (keycode == ALLEGRO_KEY_RIGHT)
		joystick_right(player2->control);
	if (keycode == ALLEGRO_KEY_UP)
		joystick_up(player2->control);
	if (keycode == ALLEGRO_KEY_DOWN)
		joystick_down(player2->control);
	
}	


void player_attack(struct player *player1, struct player *player2)
{
	// player nao pode atacar durante pulo ou descida
	if ((player1->state != UP) && (player1->state != FALL)) {

		if (player1->control->hit1 && player1->state != ATTACK2) {
			player1->state = ATTACK1;
		}
		if (player1->control->hit2 && player1->state != ATTACK1) {
			player1->state = ATTACK2;
		}
		if (player1->state == ATTACK1) {

			if (player1->hitbox->active && box_collision(player1->hitbox, player2->hurtbox))
					player2->hp = player2->hp - 1;

			if (player1->frame == HIT1_4) {
				player1->hitbox->active = ACTIVE;
				short move = STEPS;
				if (player1->dir == LEFT) {
					move *= -1;
				}
				player1->hitbox->x = player1->hitbox->x + move*3;
				player1->hitbox->y = player1->hitbox->y - STEPS*3/2;
			}
			if (player1->frame == HIT1_6) {
				player1->hitbox->active = !ACTIVE;
				player1->hitbox->x = player1->hurtbox->x;
				player1->hitbox->y = player1->hurtbox->y;
				player1->state = IDLE;
			}
		}
		if (player1->state == ATTACK2) {

			if (player1->hitbox->active && box_collision(player1->hitbox, player2->hurtbox))
					player2->hp = player2->hp - 2;

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
	}
		

}

void player_move(struct player *player1, struct player *player2, struct box *floor)
{
	if (player1->state == ATTACK1 || player1->state == ATTACK2)
		return;

	// salva posicao inicial
	short in_x1 = player1->x;
	short in_y1 = player1->y;

	// movimento do player 1
	
//	printf("colidindo com chao? %d\n",  box_collision(player1->hurtbox, floor));
	if (player1->control->down && box_collision(player1->hurtbox, floor)) {
		short side_y = al_get_bitmap_height(player1->bitmap[0]) * player1->resize*2/3;
		short new_y = player1->y + side_y /8;
		player1->hurtbox = box_update(player1->hurtbox, player1->hurtbox->x, new_y, player1->hurtbox->side_x, side_y /2, 1);
		player1->state = DOWN;
	}
	else {
		// se antes ele estava agachado, resetamos a hurtbox
		short side_y = al_get_bitmap_height(player1->bitmap[0]) * player1->resize*2/3;
		if (player1->hurtbox->side_y != side_y) {
			short new_y = player1->y - side_y /8;
			player1->hurtbox = box_update(player1->hurtbox, player1->hurtbox->x, new_y, player1->hurtbox->side_x, side_y, 1);
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

	
	// movimento do player 2
	if (player2->control->right) {
		player2->hurtbox->x = player2->hurtbox->x +  STEPS;
		if (box_valid_position(player2->hurtbox) && !box_collision(player1->hurtbox, player2->hurtbox)){
			player2->x = player2->x +  STEPS;
		}
		else
			player2->hurtbox->x = player2->hurtbox->x -  STEPS;
	}

	if (player2->control->left) {
		player2->hurtbox->x = player2->hurtbox->x -  STEPS;
		if (box_valid_position(player2->hurtbox) && !box_collision(player1->hurtbox, player2->hurtbox)){
			player2->x = player2->x -  STEPS;
		}
		else
			player2->hurtbox->x = player2->hurtbox->x +  STEPS;
	}

	if (player2->control->up) {
		player2->hurtbox->y = player2->hurtbox->y -  STEPS;
		if (box_valid_position(player2->hurtbox) && !box_collision(player1->hurtbox, player2->hurtbox))
			player2->y = player2->y -  STEPS;
		else
			player2->hurtbox->y = player2->hurtbox->y +  STEPS;
	}

	if (player2->control->down) {
		player2->hurtbox->y = player2->hurtbox->y +  STEPS;
		if (box_valid_position(player2->hurtbox) && !box_collision(player1->hurtbox, player2->hurtbox))
			player2->y = player2->y +  STEPS;
		else
			player2->hurtbox->y = player2->hurtbox->y -  STEPS;
	}

//	printf("dir %d\n", player1->dir);



		

	
			

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
			break;

		case DOWN2:
			if (player->state != DOWN)
				player->frame = DOWN1;
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
			break;

		case HIT2_1:
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
