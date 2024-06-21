#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <allegro5/allegro5.h>
#include <allegro5/allegro_image.h>
#include "player.h"
#include "box.h"
#include "joystick.h"
#include "streetfighter.h"

ALLEGRO_BITMAP **player_load_bitmap(ALLEGRO_BITMAP  **bitmap)
{
	char *string = malloc(sizeof(char) * 30);

	for (short i = 0; i <= 7; i++) {
		sprintf(string, "ponei%d.png", i);
		bitmap[i] = al_load_bitmap(string);
	}

	free(string);
	return (bitmap);
}


struct player *player_create(short x, short y, float resize)
{
	struct player *new_player = malloc(sizeof(struct player));

	new_player->id = -1;
	new_player->state = IDLE_R;
	new_player->dir = IDLE;
	new_player->hp = 100;
	new_player->vel= VEL_MAX;
	new_player->x = x;
	new_player->y = y;
	new_player->resize = resize;

	new_player->bitmap = malloc(8 * sizeof(ALLEGRO_BITMAP*));
	new_player->bitmap = player_load_bitmap(new_player->bitmap);	

	short side_x = al_get_bitmap_width(new_player->bitmap[0]) * resize;
	short side_y = al_get_bitmap_height(new_player->bitmap[0]) * resize;

	new_player->hitbox = box_create(x, y, side_x, side_y, 1);
	new_player->control = joystick_create();

	return (new_player);
}

void player_destroy(struct player *playerD)
{
	if (!playerD)
		return;

	if (playerD->control)
		joystick_destroy(playerD->control);

	if (playerD->hitbox)
		box_destroy(playerD->hitbox);
	
	if (playerD->bitmap) {
		for (short i = 0; i <= 7; i++) {
			if (playerD->bitmap[i])
				al_destroy_bitmap(playerD->bitmap[i]);
		}
		free(playerD->bitmap);
	}

	free(playerD);
}


void player_draw(struct player *playerP)
{
	short frame = playerP->state;
	/*if (playerP->state == JUMP1)
		frame = 7;
	if (playerP->state == JUMP2 || playerP->state == JUMP4 || playerP->state == JUMP5)
		frame = 5;
	if (playerP->state == JUMP3)
		frame = 6; 
*/
	short side_x = al_get_bitmap_width(playerP->bitmap[frame]);
	short side_y = al_get_bitmap_height(playerP->bitmap[frame]);
	short x = playerP->x - side_x/(2*RESIZE);
	short y = playerP->y - side_y/(2*RESIZE);

	al_draw_scaled_bitmap(playerP->bitmap[frame], 0, 0, side_x, side_y, x, y, side_x * playerP->resize, side_y * playerP->resize, 0);		
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
	

	if (keycode == ALLEGRO_KEY_LEFT)
		joystick_left(player2->control);
	if (keycode == ALLEGRO_KEY_RIGHT)
		joystick_right(player2->control);
	if (keycode == ALLEGRO_KEY_UP)
		joystick_up(player2->control);
	if (keycode == ALLEGRO_KEY_DOWN)
		joystick_down(player2->control);
	
}	


void player_move(struct player *player1, struct player *player2, struct box *floor)
{
	// movimento do player 1

	if (player1->control->down && box_collision(player1->hitbox, floor)) {
		short side_y = al_get_bitmap_height(player1->bitmap[0]) * player1->resize;
		short new_y = player1->y + side_y /8;
		player1->hitbox = box_update(player1->hitbox, player1->hitbox->x, new_y, player1->hitbox->side_x, side_y /2, 1);
	}
	else {
		// se antes ele estava agachado, resetamos a hitbox
		short side_y = al_get_bitmap_height(player1->bitmap[0]) * player1->resize;
		if (player1->hitbox->side_y != side_y) {
			short new_y = player1->y - side_y /8;
			player1->hitbox = box_update(player1->hitbox, player1->hitbox->x, new_y, player1->hitbox->side_x, side_y, 1);
		}


		if (player1->control->right) {
			player1->hitbox->x = player1->hitbox->x +  STEPS;
			if (box_valid_position(player1->hitbox) && !box_collision(player1->hitbox, player2->hitbox)){
				player1->x = player1->x +  STEPS;
			}
			else
				player1->hitbox->x = player1->hitbox->x -  STEPS;
		}

		if (player1->control->left) {
			player1->hitbox->x = player1->hitbox->x -  STEPS;
			if (box_valid_position(player1->hitbox) && !box_collision(player1->hitbox, player2->hitbox)){
				player1->x = player1->x -  STEPS;
			}
			else
				player1->hitbox->x = player1->hitbox->x +  STEPS;
		}

		// pulo inicia com a velocidade maxima 
		if (box_collision(player1->hitbox, floor))
			player1->vel = VEL_MAX;

		// caso estiver pulando 
		if ((player1->control->up) || !box_collision(player1->hitbox, floor)) {
			player1->hitbox->y = player1->hitbox->y - player1->vel * STEPS;
			if (box_valid_position(player1->hitbox) && !box_collision(player1->hitbox, player2->hitbox)) {
				player1->y = player1->y - player1->vel *STEPS;
				player1->vel = player1->vel - 1;
			}
			else
				player1->hitbox->y = player1->hitbox->y + player1->vel * STEPS;

		}
	}

	
	// movimento do player 2
	if (player2->control->right) {
		player2->hitbox->x = player2->hitbox->x +  STEPS;
		if (box_valid_position(player2->hitbox) && !box_collision(player1->hitbox, player2->hitbox)){
			player2->x = player2->x +  STEPS;
		}
		else
			player2->hitbox->x = player2->hitbox->x -  STEPS;
	}

	if (player2->control->left) {
		player2->hitbox->x = player2->hitbox->x -  STEPS;
		if (box_valid_position(player2->hitbox) && !box_collision(player1->hitbox, player2->hitbox)){
			player2->x = player2->x -  STEPS;
		}
		else
			player2->hitbox->x = player2->hitbox->x +  STEPS;
	}

	if (player2->control->up) {
		player2->hitbox->y = player2->hitbox->y -  STEPS;
		if (box_valid_position(player2->hitbox) && !box_collision(player1->hitbox, player2->hitbox))
			player2->y = player2->y -  STEPS;
		else
			player2->hitbox->y = player2->hitbox->y +  STEPS;
	}

	if (player2->control->down) {
		player2->hitbox->y = player2->hitbox->y +  STEPS;
		if (box_valid_position(player2->hitbox) && !box_collision(player1->hitbox, player2->hitbox))
			player2->y = player2->y +  STEPS;
		else
			player2->hitbox->y = player2->hitbox->y -  STEPS;
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
			box_draw(playerP->hitbox, 255, 102, 255);
	//		player_draw(playerP);
			break;
		case RIGHT1:
			player_move(playerP, 1, RIGHT);
			box_draw(playerP->hitbox, 255, 102, 255);
	//		player_draw(playerP);
			break;
		case RIGHT2:
			player_move(playerP, 1, RIGHT);
			box_draw(playerP->hitbox, 255, 102, 255);
	//		player_draw(playerP);
			break;
		case LEFT1:
			player_move(playerP, 1, LEFT);
			box_draw(playerP->hitbox, 255, 102, 255);
	//		player_draw(playerP);
			break;
		case LEFT2:
			player_move(playerP, 1, LEFT);
			box_draw(playerP->hitbox, 255, 102, 255);
	//		player_draw(playerP);
			break;
		case JUMP1:
			box_draw(playerP->hitbox, 255, 102, 255);
	//		player_draw(playerP);
			break;
		case JUMP2:
			player_move(playerP, 2, UP);
			box_draw(playerP->hitbox, 255, 102, 255);
	//		player_draw(playerP);
			break;
		case JUMP3:
			player_move(playerP, 2, UP);
			box_draw(playerP->hitbox, 255, 102, 255);
	//		player_draw(playerP);
			break;
		case JUMP4:
			player_move(playerP, 2, DOWN);
			box_draw(playerP->hitbox, 255, 102, 255);
	//		player_draw(playerP);
			break;
		case JUMP5:
			player_move(playerP, 2, DOWN);
			box_draw(playerP->hitbox, 255, 102, 255);
	//		player_draw(playerP);
			break;
	}
}*/
