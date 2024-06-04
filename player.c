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
	bitmap[0] = al_load_bitmap("ponei0.png");
	bitmap[1] = al_load_bitmap("ponei1.png");
	bitmap[2] = al_load_bitmap("ponei2.png");
	bitmap[3] = al_load_bitmap("ponei3.png");
	bitmap[4] = al_load_bitmap("ponei4.png");
	bitmap[5] = al_load_bitmap("ponei5.png");
	bitmap[6] = al_load_bitmap("ponei6.png");
	bitmap[7] = al_load_bitmap("ponei7.png");

	return (bitmap);
}


struct player *player_create(char id, short x, short y)
{
	struct player *new_player = malloc(sizeof(struct player));

	new_player->id = id;
	new_player->state = IDLE;
	new_player->health = 100;
	new_player->x = x;
	new_player->y = y;

	new_player->bitmap = malloc(10 * sizeof(ALLEGRO_BITMAP*));
	new_player->bitmap = player_load_bitmap(new_player->bitmap);	

	short side_x = al_get_bitmap_width(new_player->bitmap[0]) /RESIZE;
	short side_y = al_get_bitmap_height(new_player->bitmap[0]) /RESIZE;

	new_player->hitbox = box_create(x, y, side_x, side_y, 1);
	new_player->control = joystick_create();

	return (new_player);
}

void player_destroy(struct player *playerD)
{
	joystick_destroy(playerD->control);
	box_destroy(playerD->hitbox);

	for (short i = 0; i <= 7; i++) {
		al_destroy_bitmap(playerD->bitmap[i]);
	}
	free(playerD->bitmap);

	free(playerD);
}


void player_draw(struct player *playerP)
{

	short frame = playerP->state;
	if (playerP->state == JUMP1)
		frame = 7;
	if (playerP->state == JUMP2 || playerP->state == JUMP4 || playerP->state == JUMP5)
		frame = 5;
	if (playerP->state == JUMP3)
		frame = 6; 

#ifdef DEBUG
	printf("state: %d\n", playerP->state);
	printf("frame: %d\n", frame);
#endif

	short side_x = al_get_bitmap_width(playerP->bitmap[frame]);
	short side_y = al_get_bitmap_height(playerP->bitmap[frame]);
	short x = playerP->x - side_x/(2*RESIZE);
	short y = playerP->y - side_y/(2*RESIZE);

	al_draw_scaled_bitmap(playerP->bitmap[frame], 0, 0, side_x, side_y, x, y, side_x/RESIZE, side_y/RESIZE, 0);		
}

void player_move(struct player *playerP, short steps, short direction)
{
#ifdef DEBUG
	printf("box x: %d\n", playerP->hitbox->x);
	printf("box y: %d\n", playerP->hitbox->y);
#endif
	switch (direction) {

		case RIGHT:	
			playerP->hitbox->x = playerP->hitbox->x + steps * STEPS;
			if (box_valid_position(playerP->hitbox)){
				playerP->x = playerP->x + (steps * STEPS);
			}
			else
				playerP->hitbox->x = playerP->hitbox->x - steps * STEPS;
			break;

		case LEFT:	
			playerP->hitbox->x = playerP->hitbox->x - steps * STEPS;
			if (box_valid_position(playerP->hitbox)){
				playerP->x = playerP->x - steps * STEPS;
			}
			else
				playerP->hitbox->x = playerP->hitbox->x + steps * STEPS;
			break;

		case UP:	
			playerP->hitbox->y = playerP->hitbox->y - steps * STEPS;
			if (box_valid_position(playerP->hitbox))
				playerP->y = playerP->y - steps * STEPS;
			else
				playerP->hitbox->y = playerP->hitbox->y + steps * STEPS;
			break;
		case DOWN:	
			playerP->hitbox->y = playerP->hitbox->y + steps * STEPS;
			if (box_valid_position(playerP->hitbox))
				playerP->y = playerP->y + steps * STEPS;
			else
				playerP->hitbox->y = playerP->hitbox->y - steps * STEPS;
			break;
	}
}

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
			box_draw(playerP->hitbox);
			player_draw(playerP);
			break;
		case RIGHT1:
			player_move(playerP, 1, RIGHT);
			box_draw(playerP->hitbox);
			player_draw(playerP);
			break;
		case RIGHT2:
			player_move(playerP, 1, RIGHT);
			box_draw(playerP->hitbox);
			player_draw(playerP);
			break;
		case LEFT1:
			player_move(playerP, 1, LEFT);
			box_draw(playerP->hitbox);
			player_draw(playerP);
			break;
		case LEFT2:
			player_move(playerP, 1, LEFT);
			box_draw(playerP->hitbox);
			player_draw(playerP);
			break;
		case JUMP1:
			box_draw(playerP->hitbox);
			player_draw(playerP);
			break;
		case JUMP2:
			player_move(playerP, 2, UP);
			box_draw(playerP->hitbox);
			player_draw(playerP);
			break;
		case JUMP3:
			player_move(playerP, 2, UP);
			box_draw(playerP->hitbox);
			player_draw(playerP);
			break;
		case JUMP4:
			player_move(playerP, 2, DOWN);
			box_draw(playerP->hitbox);
			player_draw(playerP);
			break;
		case JUMP5:
			player_move(playerP, 2, DOWN);
			box_draw(playerP->hitbox);
			player_draw(playerP);
			break;
	}
}
