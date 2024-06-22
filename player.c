#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <allegro5/allegro5.h>
#include <allegro5/allegro_image.h>
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
	new_player->state = IDLE_R;
	new_player->dir = IDLE;
	new_player->hp = 100;
	new_player->vel= VEL_MAX;
	new_player->x = x;
	new_player->y = y;
	new_player->resize = resize;

	new_player->bitmap = malloc(20 * sizeof(ALLEGRO_BITMAP*));
	new_player->bitmap = player_load_bitmap(new_player->bitmap, id);	

	short side_x = al_get_bitmap_width(new_player->bitmap[0]) * resize;
	short side_y = al_get_bitmap_height(new_player->bitmap[0]) * resize;

	new_player->hitbox = box_create(x, y, side_x/2, side_y*2/3, 1);
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
		for (short i = 0; i <= 18; i++) {
			if (playerD->bitmap[i])
				al_destroy_bitmap(playerD->bitmap[i]);
		}
		free(playerD->bitmap);
	}

	free(playerD);
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

void player_draw_hp(struct player *player, short num)
{
	// define a cor da barra de saude
	short r = 0;
	short g = 204;

	if (player->hp < 80) r += 102; 
	if (player->hp < 60) r += 102;
	if (player->hp < 40) g -= 102;
	if (player->hp < 20) g -= 102;

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


void player_attack(struct player *player1, struct player *player2, struct box *floor)
{
	if (player1->dir != IDLE)
		return;

//	if (player1->control->hit1) {
//		struct box *hurtbox = 

}

void player_move(struct player *player1, struct player *player2, struct box *floor)
{
	// salva posicao inicial
	short in_x1 = player1->x;
	short in_y1 = player1->y;

	// movimento do player 1
	
//	printf("colidindo com chao? %d\n",  box_collision(player1->hitbox, floor));
	if (player1->control->down && box_collision(player1->hitbox, floor)) {
		short side_y = al_get_bitmap_height(player1->bitmap[0]) * player1->resize*2/3;
		short new_y = player1->y + side_y /8;
		player1->hitbox = box_update(player1->hitbox, player1->hitbox->x, new_y, player1->hitbox->side_x, side_y /2, 1);
		player1->dir = DOWN;
	}
	else {
		// se antes ele estava agachado, resetamos a hitbox
		short side_y = al_get_bitmap_height(player1->bitmap[0]) * player1->resize*2/3;
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
			player1->hitbox->y = player1->hitbox->y - player1->vel * STEPS/2;
			if (box_valid_position(player1->hitbox) && !box_collision(player1->hitbox, player2->hitbox)) {
				player1->y = player1->y - player1->vel *STEPS/2;
				player1->vel = player1->vel - 1;
			}
			else
				player1->hitbox->y = player1->hitbox->y + player1->vel * STEPS/2;

		}

		// classifica direcao final
		if (player1->y < in_y1)
			player1->dir = UP;
		else
		if (player1->y > in_y1)
			player1->dir = FALL;
		else
		if (player1->x > in_x1) 
			player1->dir = RIGHT;
		else
		if (player1->x < in_x1) 
			player1->dir = LEFT;
		else
			player1->dir = IDLE;
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

//	printf("dir %d\n", player1->dir);



		

	
			

}


// fazer funcao tipo animation state trigger
// pega a direcao e seta o state inicial para aquela animacao
void player_animation(struct player *player)
{
	switch (player->state) {

		case IDLE_R:
			player_draw(player, 0, 0);	
			break;
		case IDLE_L:
			player_draw(player, 0, 1);	
			break;
		case RIGHT1:
			player_draw(player, 1, 0);	
			break;
		case RIGHT2:
			player_draw(player, 2, 0);	
			break;
		case RIGHT3:
			player_draw(player, 3, 0);
			break;
		case RIGHT4:
			player_draw(player, 4, 0);
			break;
		case LEFT1:
			player_draw(player, 1, 1);	
			break;
		case LEFT2:
			player_draw(player, 2, 1);	
			break;
		case LEFT3:
			player_draw(player, 3, 1);
			break;
		case LEFT4:
			player_draw(player, 4, 1);
			break;
		case UP_R1:
			player_draw(player, 5, 0);
			break;
		case UP_R2:
			player_draw(player, 6, 0);
			break;
		case UP_R3:
			player_draw(player, 7, 0);
			break;
		case UP_R4:
			player_draw(player, 7, 0);
			break;
		case FALL_R1:
			player_draw(player, 8, 0);
			break;
		case FALL_R2:
			player_draw(player, 8, 0);
			break;
		case FALL_R3:
			player_draw(player, 9, 0);
			break;
		case FALL_R4:
			player_draw(player, 5, 0);
			break;
		case UP_L1:
			player_draw(player, 5, 1);
			break;
		case UP_L2:
			player_draw(player, 6, 1);
			break;
		case UP_L3:
			player_draw(player, 7, 1);
			break;
		case UP_L4:
			player_draw(player, 7, 1);
			break;
		case FALL_L1:
			player_draw(player, 8, 1);
			break;
		case FALL_L2:
			player_draw(player, 8, 1);
			break;
		case FALL_L3:
			player_draw(player, 9, 1);
			break;
		case FALL_L4:
			player_draw(player, 5, 1);
			break;
		case DOWN_R1:
			player_draw(player, 5, 0);
			break;
		case DOWN_R2:
			player_draw(player, 19, 0);
			break;
		case DOWN_L1:
			player_draw(player, 5, 1);
			break;
		case DOWN_L2:
			player_draw(player, 19, 1);
			break;
	}

}

void player_update_state(struct player *player)
{
//	printf("player state %d \n", player->state);
//	printf("player state %d \n", player->dir);

	switch (player->state) {

		case IDLE_R:
			if (player->dir == RIGHT)
				player->state = RIGHT1;
			if (player->dir == LEFT)
				player->state = LEFT1;
			if (player->dir == UP)
				player->state = UP_R1;
			if (player->dir == DOWN)
				player->state = DOWN_R1;
			if (player->dir == FALL)
				player->state = FALL_R2;
			break;
		
		case IDLE_L:
			if (player->dir == RIGHT)
				player->state = RIGHT1;
			if (player->dir == LEFT)
				player->state = LEFT1;
			if (player->dir == UP)
				player->state = UP_L1;
			if (player->dir == DOWN)
				player->state = DOWN_L1;
			if (player->dir == FALL)
				player->state = FALL_R2;
			break;

		case RIGHT1:
			if (player->dir == RIGHT)
				player->state = RIGHT2;
			if (player->dir == LEFT)
				player->state = LEFT1;
			if (player->dir == UP)
				player->state = UP_R1;
			if (player->dir == DOWN)
				player->state = DOWN_R1;
			if (player->dir == IDLE)
				player->state = IDLE_R;
			if (player->dir == FALL)
				player->state = FALL_R2;
			break;

		case RIGHT2:
			if (player->dir == RIGHT)
				player->state = RIGHT3;
			if (player->dir == LEFT)
				player->state = LEFT1;
			if (player->dir == UP)
				player->state = UP_R1;
			if (player->dir == DOWN)
				player->state = DOWN_R1;
			if (player->dir == IDLE)
				player->state = IDLE_R;
			if (player->dir == FALL)
				player->state = FALL_R2;
			break;

		case RIGHT3:
			if (player->dir == RIGHT)
				player->state = RIGHT4;
			if (player->dir == LEFT)
				player->state = LEFT1;
			if (player->dir == UP)
				player->state = UP_R1;
			if (player->dir == DOWN)
				player->state = DOWN_R1;
			if (player->dir == IDLE)
				player->state = IDLE_R;
			if (player->dir == FALL)
				player->state = FALL_R2;
			break;

		case RIGHT4:
			if (player->dir == RIGHT)
				player->state = RIGHT1;
			if (player->dir == LEFT)
				player->state = LEFT1;
			if (player->dir == UP)
				player->state = UP_R1;
			if (player->dir == DOWN)
				player->state = DOWN_R1;
			if (player->dir == IDLE)
				player->state = IDLE_R;
			if (player->dir == FALL)
				player->state = FALL_R2;
			break;

		case LEFT1:
			if (player->dir == RIGHT)
				player->state = RIGHT1;
			if (player->dir == LEFT)
				player->state = LEFT2;
			if (player->dir == UP)
				player->state = UP_L1;
			if (player->dir == DOWN)
				player->state = DOWN_L1;
			if (player->dir == IDLE)
				player->state = IDLE_L;
			if (player->dir == FALL)
				player->state = FALL_L2;
			break;

		case LEFT2:
			if (player->dir == RIGHT)
				player->state = RIGHT1;
			if (player->dir == LEFT)
				player->state = LEFT3;
			if (player->dir == UP)
				player->state = UP_L1;
			if (player->dir == DOWN)
				player->state = DOWN_L1;
			if (player->dir == IDLE)
				player->state = IDLE_L;
			if (player->dir == FALL)
				player->state = FALL_L2;
			break;

		case LEFT3:
			if (player->dir == RIGHT)
				player->state = RIGHT1;
			if (player->dir == LEFT)
				player->state = LEFT4;
			if (player->dir == UP)
				player->state = UP_L1;
			if (player->dir == DOWN)
				player->state = DOWN_L1;
			if (player->dir == IDLE)
				player->state = IDLE_L;
			if (player->dir == FALL)
				player->state = FALL_L2;
			break;

		case LEFT4:
			if (player->dir == RIGHT)
				player->state = RIGHT1;
			if (player->dir == LEFT)
				player->state = LEFT1;
			if (player->dir == UP)
				player->state = UP_L1;
			if (player->dir == DOWN)
				player->state = DOWN_L1;
			if (player->dir == IDLE)
				player->state = IDLE_L;
			if (player->dir == FALL)
				player->state = FALL_L2;
			break;

		case UP_R1:
			player->state = UP_R2;
			break;

		case UP_R2:
			player->state = UP_R3;
			break;

		case UP_R3:
			player->state = UP_R4;
			break;

		case UP_R4:
			player->state = FALL_R1;
			break;

		case FALL_R1:
			if (player->dir == IDLE)
				player->state = IDLE_R;
			else
				player->state = FALL_R2;
			break;

		case FALL_R2:
			if (player->dir == IDLE)
				player->state = IDLE_R;
			else
				player->state = FALL_R3;
			break;

		case FALL_R3:
			if (player->dir == IDLE)
				player->state = IDLE_R;
			else
				player->state = FALL_R4;
			break;

		case FALL_R4:
			player->state = IDLE_R;
			break;

		case UP_L1:
			player->state = UP_L2;
			break;

		case UP_L2:
			player->state = UP_L3;
			break;

		case UP_L3:
			player->state = UP_L4;
			break;

		case UP_L4:
			player->state = FALL_L1;
			break;

		case FALL_L1:
			if (player->dir == IDLE)
				player->state = IDLE_L;
			else
				player->state = FALL_L2;
			break;

		case FALL_L2:
			if (player->dir == IDLE)
				player->state = IDLE_L;
			else
				player->state = FALL_L3;
			break;

		case FALL_L3:
			if (player->dir == IDLE)
				player->state = IDLE_L;
			else
				player->state = FALL_L4;
			break;

		case FALL_L4:
			player->state = IDLE_L;
			break;

		case DOWN_R1:
			if (player->dir == RIGHT)
				player->state = RIGHT1;
			if (player->dir == LEFT)
				player->state = LEFT1;
			if (player->dir == UP)
				player->state = UP_R1;
			if (player->dir == DOWN)
				player->state = DOWN_R2;
			if (player->dir == IDLE)
				player->state = IDLE_R;
			break;

		case DOWN_R2:
			if (player->dir != DOWN)
				player->state = DOWN_R1;
			break;

		case DOWN_L1:
			if (player->dir == RIGHT)
				player->state = RIGHT1;
			if (player->dir == LEFT)
				player->state = LEFT1;
			if (player->dir == UP)
				player->state = UP_L1;
			if (player->dir == DOWN)
				player->state = DOWN_L2;
			if (player->dir == IDLE)
				player->state = IDLE_L;
			break;

		case DOWN_L2:
			if (player->dir != DOWN)
				player->state = DOWN_L1;
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
