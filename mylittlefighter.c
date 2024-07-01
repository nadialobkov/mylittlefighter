#include <stdio.h>
#include <stdlib.h>

#include <allegro5/allegro5.h>
#include <allegro5/allegro_image.h>

#include "mylittlefighter.h"
#include "player.h"
#include "button.h"

void draw_image_resized(ALLEGRO_BITMAP *bitmap, short x, short y, float resize)
{
	short side_x = al_get_bitmap_width(bitmap);
	short side_y = al_get_bitmap_height(bitmap);

	short new_x = x - side_x/(2/resize);
	short new_y = y - side_y/(2/resize);

	al_draw_scaled_bitmap(bitmap, 0, 0, side_x, side_y, new_x, new_y, side_x * resize, side_y * resize, 0);	
}


ALLEGRO_BITMAP *background_sel(enum Backgrounds back)
{
	ALLEGRO_BITMAP *background;

	switch (back) {
		case CASTLE:
			background = al_load_bitmap("./sprites/backgrounds/castle_of_friendship.png");
			break;
		case PONYVILLE:
			background = al_load_bitmap("./sprites/backgrounds/ponyville.png");
			break;
		case CLOUDSDALE:
			background = al_load_bitmap("./sprites/backgrounds/sky.png");
			break;
	}
	
	return background;
}

ALLEGRO_BITMAP *round_sel(short num)
{
	ALLEGRO_BITMAP *round;

	switch (num) {
		case 1:
			round = al_load_bitmap("./sprites/text/round1.png");
			break;
		case 2:
			round = al_load_bitmap("./sprites/text/round2.png");
			break;
		case 3:
			round = al_load_bitmap("./sprites/text/round3.png");
			break;
	}
	
	return round;
}

ALLEGRO_BITMAP *player_icon_sel(short id, short num)
{
	ALLEGRO_BITMAP *icon;

	switch (id) {
		case PINKIE:
			if (num == 1) 
				icon = al_load_bitmap("./sprites/buttons/icon_pinkie.png");
			else
				icon = al_load_bitmap("./sprites/buttons/icon_pinkie_ch.png");
			break;

		case RARITY:
			if (num == 1) 
				icon = al_load_bitmap("./sprites/buttons/icon_rarity.png");
			else
				icon = al_load_bitmap("./sprites/buttons/icon_rarity_ch.png");
			break;
	}
	
	return icon;

}

ALLEGRO_BITMAP *player_win_sel(short num)
{
	ALLEGRO_BITMAP *player;

	switch (num) {
		case 1:
			player = al_load_bitmap("./sprites/text/player1.png");
			break;
		case 2:
			player = al_load_bitmap("./sprites/text/player2.png");
			break;
	}
	
	return player;
}

struct mlf *mlf_create_game()
{
	struct mlf *game = malloc(sizeof(struct mlf));

	game->state = MENU_START;
	game->round = 1;
	game->paused = 0;
	game->player1 = NULL; 
	game->player2 = NULL;
	game->queue = al_create_event_queue();
	game->timer = al_create_timer(1.0/ FPS);
	game->disp = al_create_display(X_SCREEN, Y_SCREEN);
	game->mouse_x = 0;
	game->mouse_y = 0;
		
	return game;
}

void mlf_destroy_game(struct mlf *game)
{
	player_destroy(game->player1);
	player_destroy(game->player2);

	al_destroy_display(game->disp);
	al_destroy_timer(game->timer);
	al_destroy_event_queue(game->queue);

	free(game);
}

void mlf_menu_start(struct mlf *game)
{
	struct button *button_start = button_create("./sprites/buttons/button_start.png", X_SCREEN /2, Y_SCREEN /2, RESIZE_SCREEN);
	ALLEGRO_BITMAP *castle = al_load_bitmap("./sprites/backgrounds/castle.png");
	ALLEGRO_BITMAP *mlf_logo = al_load_bitmap("./sprites/menu/mlf_logo.png");


	while (game->state == MENU_START) {
		
		al_wait_for_event(game->queue, &(game->event));
		
		if (game->event.type == ALLEGRO_EVENT_MOUSE_AXES) {
			game->mouse_x = game->event.mouse.x;
			game->mouse_y = game->event.mouse.y;
		}
		if (game->event.type == ALLEGRO_EVENT_TIMER) {	
			draw_image_resized(castle, X_SCREEN /2, Y_SCREEN /2, RESIZE_SCREEN);
			draw_image_resized(mlf_logo, X_SCREEN /2, Y_SCREEN /5, RESIZE_SCREEN);
			button_update(button_start, game->mouse_x, game->mouse_y);
			al_flip_display();	
		}
		if (game->event.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN) {
			if (button_pressed(button_start, game->mouse_x, game->mouse_y, game->event)) {
				game->state = MENU_PLAYER_SEL;
			}
		}
		if (game->event.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
			break;
		}
	}

	button_destroy(button_start);
	al_destroy_bitmap(castle);
	al_destroy_bitmap(mlf_logo);
}

void mlf_menu_player_sel(struct mlf *game)
{
	struct button *p1_icon0 = button_create("./sprites/buttons/icon_pinkie.png", X_SCREEN*0.28, Y_SCREEN*0.6, 0.1 * RESIZE_SCREEN);
	struct button *p1_icon1 = button_create("./sprites/buttons/icon_rarity.png", X_SCREEN*0.36, Y_SCREEN*0.6, 0.1 * RESIZE_SCREEN);
	struct button *p2_icon0 = button_create("./sprites/buttons/icon_pinkie_ch.png", X_SCREEN*0.6, Y_SCREEN*0.6, 0.1 * RESIZE_SCREEN);
	struct button *p2_icon1 = button_create("./sprites/buttons/icon_rarity_ch.png", X_SCREEN*0.68, Y_SCREEN*0.6, 0.1 * RESIZE_SCREEN);
	struct button *back = button_create("./sprites/buttons/back.png", X_SCREEN/12, Y_SCREEN*10/12, RESIZE_SCREEN);
	struct button *next = button_create("./sprites/buttons/next.png", X_SCREEN*11/12, Y_SCREEN*10/12, RESIZE_SCREEN);
	ALLEGRO_BITMAP *castle = al_load_bitmap("./sprites/backgrounds/castle.png");
	ALLEGRO_BITMAP *mlf_logo = al_load_bitmap("./sprites/menu/mlf_logo.png");
	ALLEGRO_BITMAP *choose_player_display = al_load_bitmap("./sprites/menu/choose_char.png");
	ALLEGRO_BITMAP *text_pinkie = al_load_bitmap("./sprites/text/pinkie_pie.png");
	ALLEGRO_BITMAP *text_rarity = al_load_bitmap("./sprites/text/rarity.png");

	enum Pony player1_id = -1;
	enum Pony player2_id = -1;

	while (game->state == MENU_PLAYER_SEL) {
		
		al_wait_for_event(game->queue, &(game->event));
		
		if (game->event.type == ALLEGRO_EVENT_MOUSE_AXES) {
			game->mouse_x = game->event.mouse.x;
			game->mouse_y = game->event.mouse.y;
		}
		if (game->event.type == ALLEGRO_EVENT_TIMER) {	
			draw_image_resized(castle, X_SCREEN*0.5, Y_SCREEN*0.5, RESIZE_SCREEN);
			draw_image_resized(mlf_logo, X_SCREEN*0.1, Y_SCREEN*0.12, 0.5 * RESIZE_SCREEN);
			draw_image_resized(choose_player_display, X_SCREEN*0.5, Y_SCREEN*0.45, RESIZE_SCREEN);
			button_update(back, game->mouse_x, game->mouse_y);
			button_update(p1_icon0, game->mouse_x, game->mouse_y);
			button_update(p1_icon1, game->mouse_x, game->mouse_y);
			button_update(p2_icon0, game->mouse_x, game->mouse_y);
			button_update(p2_icon1, game->mouse_x, game->mouse_y);

			if (player1_id == PINKIE)
				draw_image_resized(text_pinkie, X_SCREEN/3, Y_SCREEN*0.88, RESIZE_SCREEN);
			if (player2_id == PINKIE)
				draw_image_resized(text_pinkie, X_SCREEN*2/3, Y_SCREEN*0.88, RESIZE_SCREEN);
			if (player1_id == RARITY)
				draw_image_resized(text_rarity, X_SCREEN/3, Y_SCREEN*0.88, RESIZE_SCREEN);
			if (player2_id == RARITY)
				draw_image_resized(text_rarity, X_SCREEN*2/3, Y_SCREEN*0.88, RESIZE_SCREEN);

			if ((player1_id != -1) && (player2_id != -1))
				button_update(next, game->mouse_x, game->mouse_y);

			al_flip_display();	
		}
		if (game->event.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN) {
			if (button_pressed(back, game->mouse_x, game->mouse_y, game->event)) {
				game->state = MENU_START;
			}
			if (button_pressed(next, game->mouse_x, game->mouse_y, game->event)) {
				if ((player1_id != -1) && (player2_id != -1))
					game->state = MENU_BACK_SEL;
			}
			if (button_pressed(p1_icon0, game->mouse_x, game->mouse_y, game->event)) {
				player1_id = PINKIE;
			}
			if (button_pressed(p1_icon1, game->mouse_x, game->mouse_y, game->event)) {
				player1_id = RARITY;
			}
			if (button_pressed(p2_icon0, game->mouse_x, game->mouse_y, game->event)) {
				player2_id = PINKIE;
			}
			if (button_pressed(p2_icon1, game->mouse_x, game->mouse_y, game->event)) {
				player2_id = RARITY;
			}

		}
		if (game->event.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
			break;
		}
	}


	game->player1 = player_create(player1_id, X_SCREEN/4, Y_SCREEN*3/4, 0.5 * RESIZE_SCREEN);
	game->player2 = player_create(player2_id, X_SCREEN*3/4, Y_SCREEN*3/4, 0.5 * RESIZE_SCREEN);

	button_destroy(back);
	button_destroy(next);
	button_destroy(p1_icon0);
	button_destroy(p1_icon1);
	button_destroy(p2_icon0);
	button_destroy(p2_icon1);
	al_destroy_bitmap(castle);
	al_destroy_bitmap(mlf_logo);
	al_destroy_bitmap(choose_player_display);
	al_destroy_bitmap(text_pinkie);
	al_destroy_bitmap(text_rarity);
}

void mlf_menu_back_sel(struct mlf *game)
{
	struct button *back = button_create("./sprites/buttons/back.png", X_SCREEN/12, Y_SCREEN*10/12, RESIZE_SCREEN);
	struct button *next = button_create("./sprites/buttons/next.png", X_SCREEN*11/12, Y_SCREEN*10/12, RESIZE_SCREEN);
	struct button *castle = button_create("./sprites/buttons/icon_castle.png", X_SCREEN/4, Y_SCREEN/2, 0.4 * RESIZE_SCREEN);
	struct button *ponyville = button_create("./sprites/buttons/icon_ponyville.png", X_SCREEN/2, Y_SCREEN/2, 0.4 * RESIZE_SCREEN);
	struct button *cloudsdale = button_create("./sprites/buttons/icon_sky.png", X_SCREEN*3/4, Y_SCREEN/2, 0.4 * RESIZE_SCREEN);
	ALLEGRO_BITMAP *castle_back = al_load_bitmap("./sprites/backgrounds/castle.png");
	ALLEGRO_BITMAP *mlf_logo = al_load_bitmap("./sprites/menu/mlf_logo.png");
	ALLEGRO_BITMAP *choose_background = al_load_bitmap("./sprites/text/choose_your_background.png");
	ALLEGRO_BITMAP *text_castle = al_load_bitmap("./sprites/text/castle_of_friendship.png");
	ALLEGRO_BITMAP *text_ponyville = al_load_bitmap("./sprites/text/ponyville.png");
	ALLEGRO_BITMAP *text_cloudsdale = al_load_bitmap("./sprites/text/cloudsdale.png");

	while (game->state == MENU_BACK_SEL) {
		
		al_wait_for_event(game->queue, &(game->event));
		
		if (game->event.type == ALLEGRO_EVENT_MOUSE_AXES) {
			game->mouse_x = game->event.mouse.x;
			game->mouse_y = game->event.mouse.y;
		}
		if (game->event.type == ALLEGRO_EVENT_TIMER) {	
			draw_image_resized(castle_back, X_SCREEN /2, Y_SCREEN /2, RESIZE_SCREEN);
			draw_image_resized(mlf_logo, X_SCREEN /10, Y_SCREEN /8, 0.5 * RESIZE_SCREEN);
			draw_image_resized(choose_background, X_SCREEN /2, Y_SCREEN /10, RESIZE_SCREEN);
			button_update(back, game->mouse_x, game->mouse_y);
			button_update(next, game->mouse_x, game->mouse_y);
			button_update(castle, game->mouse_x, game->mouse_y);
			button_update(ponyville, game->mouse_x, game->mouse_y);
			button_update(cloudsdale, game->mouse_x, game->mouse_y);

			if (game->back == CASTLE)
				draw_image_resized(text_castle, X_SCREEN /2, Y_SCREEN*0.84, RESIZE_SCREEN);
			if (game->back == PONYVILLE)
				draw_image_resized(text_ponyville, X_SCREEN /2, Y_SCREEN*0.84, RESIZE_SCREEN);
			if (game->back == CLOUDSDALE)
				draw_image_resized(text_cloudsdale, X_SCREEN /2, Y_SCREEN*0.84, RESIZE_SCREEN);

			al_flip_display();	
		}
		if (game->event.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN) {
			if (button_pressed(back, game->mouse_x, game->mouse_y, game->event)) {
				game->state = MENU_PLAYER_SEL;
			}
			if (button_pressed(next, game->mouse_x, game->mouse_y, game->event)) {
				game->state = START_FIGHT;
			}
			if (button_pressed(castle, game->mouse_x, game->mouse_y, game->event)) {
				game->back = CASTLE;
			}
			if (button_pressed(ponyville, game->mouse_x, game->mouse_y, game->event)) {
				game->back = PONYVILLE;
			}
			if (button_pressed(cloudsdale, game->mouse_x, game->mouse_y, game->event)) {
				game->back = CLOUDSDALE;
			}
		}
		if (game->event.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
			break;
		}
	}

	button_destroy(back);
	button_destroy(next);
	button_destroy(castle);
	button_destroy(ponyville);
	button_destroy(cloudsdale);
	al_destroy_bitmap(castle_back);
	al_destroy_bitmap(mlf_logo);
	al_destroy_bitmap(choose_background);
	al_destroy_bitmap(text_castle);
	al_destroy_bitmap(text_ponyville);
	al_destroy_bitmap(text_cloudsdale);
}

void mlf_start_fight(struct mlf *game)
{
	struct box *floor = box_create(X_SCREEN*0.5, Y_SCREEN*0.9, X_SCREEN, Y_SCREEN*0.1, RESIZE_SCREEN);
	struct button *pause = button_create("./sprites/buttons/pause.png", X_SCREEN*0.95, Y_SCREEN*0.1, 0.3*RESIZE_SCREEN);
	ALLEGRO_BITMAP *mlf_logo = al_load_bitmap("./sprites/menu/mlf_logo.png");
	ALLEGRO_BITMAP *white_bar = al_load_bitmap("./sprites/menu/white_bar.png");
	ALLEGRO_BITMAP *text1 = al_load_bitmap("./sprites/text/1.png");
	ALLEGRO_BITMAP *text2 = al_load_bitmap("./sprites/text/2.png");
	ALLEGRO_BITMAP *text3 = al_load_bitmap("./sprites/text/3.png");
	ALLEGRO_BITMAP *text_fight = al_load_bitmap("./sprites/text/fight.png");
	ALLEGRO_BITMAP *background = background_sel(game->back);
	ALLEGRO_BITMAP *icon_p1 = player_icon_sel(game->player1->id, 1);
	ALLEGRO_BITMAP *icon_p2 = player_icon_sel(game->player2->id, 2);


	joystick_reset(game->player1->control);
	joystick_reset(game->player2->control);
	player_init(game->player1, 1);
	player_init(game->player2, 2);
	
	short cooldown = 5 *FPS;

	while (game->state == START_FIGHT) {

		al_wait_for_event(game->queue, &(game->event));

		if (game->event.type == ALLEGRO_EVENT_MOUSE_AXES) {
			game->mouse_x = game->event.mouse.x;
			game->mouse_y = game->event.mouse.y;
		}
		if (game->event.type == ALLEGRO_EVENT_TIMER) {
			draw_image_resized(background, X_SCREEN/2, Y_SCREEN /2, RESIZE_SCREEN);
			button_update(pause, game->mouse_x, game->mouse_y);
			
			player_animation(game->player1);
			player_animation(game->player2);
			if (cooldown == 0) {
				game->state = FIGHT;
				break;
			}

			cooldown--;

			if (cooldown >= (4 * FPS))
				draw_image_resized(text3, X_SCREEN/2, Y_SCREEN/2, 0.5 * RESIZE_SCREEN);
			else
				if (cooldown >= (3 * FPS))
					draw_image_resized(text2, X_SCREEN/2, Y_SCREEN/2, 0.5 * RESIZE_SCREEN);
				else
					if (cooldown >= 2 * FPS)
						draw_image_resized(text1, X_SCREEN/2, Y_SCREEN/2, 0.5 * RESIZE_SCREEN);
					else
						if (cooldown >= FPS) {
							ALLEGRO_BITMAP *round = round_sel(game->round);
							draw_image_resized(round, X_SCREEN/2, Y_SCREEN/2, 0.5 * RESIZE_SCREEN);
						}
						else	
							draw_image_resized(text_fight, X_SCREEN/2, Y_SCREEN/2, 0.5 * RESIZE_SCREEN);
						

			draw_image_resized(white_bar, X_SCREEN/2, Y_SCREEN /8, RESIZE_SCREEN);
			player_draw_hp(game->player1->hp, 1);
			player_draw_hp(game->player2->hp, 2);
			player_draw_dash(game->player1->dash, 1);
			player_draw_dash(game->player2->dash, 2);
			draw_image_resized(mlf_logo, X_SCREEN/2, Y_SCREEN /8, 0.5 * RESIZE_SCREEN);
			draw_image_resized(icon_p1, X_SCREEN*0.08, Y_SCREEN /8, 0.16 * RESIZE_SCREEN);
			draw_image_resized(icon_p2, X_SCREEN*0.92, Y_SCREEN /8, 0.16 * RESIZE_SCREEN);

			al_flip_display();	
		}

		if (game->event.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
			break;
		}
	}

	box_destroy(floor);
	al_destroy_bitmap(mlf_logo);
	al_destroy_bitmap(white_bar);
	al_destroy_bitmap(background);
	al_destroy_bitmap(text1);
	al_destroy_bitmap(text2);
	al_destroy_bitmap(text3);
	al_destroy_bitmap(text_fight);
	al_destroy_bitmap(icon_p1);
	al_destroy_bitmap(icon_p2);
}

void mlf_fight(struct mlf *game)
{
	struct box *floor = box_create(X_SCREEN*0.5, Y_SCREEN*0.90, X_SCREEN, Y_SCREEN*0.1, RESIZE_SCREEN);
	ALLEGRO_BITMAP *mlf_logo = al_load_bitmap("./sprites/menu/mlf_logo.png");
	ALLEGRO_BITMAP *white_bar = al_load_bitmap("./sprites/menu/white_bar.png");
	ALLEGRO_BITMAP *background = background_sel(game->back);
	ALLEGRO_BITMAP *round = round_sel(game->round);
	ALLEGRO_BITMAP *icon_p1 = player_icon_sel(game->player1->id, 1);
	ALLEGRO_BITMAP *icon_p2 = player_icon_sel(game->player2->id, 2);
	
	game->player1->control->active = 1;
	game->player2->control->active = 1;

	short cooldown = 0;

	while (game->state == FIGHT) {

		al_wait_for_event(game->queue, &(game->event));

		if (game->event.type == ALLEGRO_EVENT_MOUSE_AXES) {
			game->mouse_x = game->event.mouse.x;
			game->mouse_y = game->event.mouse.y;
		}
		if (game->event.type == ALLEGRO_EVENT_TIMER) {
			draw_image_resized(background, X_SCREEN/2, Y_SCREEN /2, RESIZE_SCREEN);
			//box_draw(floor, 0, 153, 51);
			player_attack(game->player1, game->player2);
			player_attack(game->player2, game->player1);
			player_move(game->player1, game->player2, floor);
			player_move(game->player2, game->player1, floor);
			//box_draw(game->player1->hurtbox, 255, 122, 255);
			//box_draw(game->player1->hitbox, 102, 0, 204);
			//box_draw(game->player2->hurtbox, 255, 122, 255);
			player_animation(game->player1);
			player_animation(game->player2);
			if (!cooldown) {
				player_update_state(game->player1);
				player_update_state(game->player2);
				cooldown = 1;
			}
			else
				cooldown = 0;

			draw_image_resized(white_bar, X_SCREEN/2, Y_SCREEN /8, RESIZE_SCREEN);
			player_draw_hp(game->player1->hp, 1);
			player_draw_hp(game->player2->hp, 2);
			player_draw_dash(game->player1->dash, 1);
			player_draw_dash(game->player2->dash, 2);
			draw_image_resized(mlf_logo, X_SCREEN/2, Y_SCREEN /8, 0.5 * RESIZE_SCREEN);
			draw_image_resized(round, X_SCREEN/2, Y_SCREEN/4, 0.2 * RESIZE_SCREEN);
			draw_image_resized(icon_p1, X_SCREEN*0.08, Y_SCREEN /8, 0.16 * RESIZE_SCREEN);
			draw_image_resized(icon_p2, X_SCREEN*0.92, Y_SCREEN /8, 0.16 * RESIZE_SCREEN);

			if (player_win(game->player1, game->player2)) {
				game->state = PLAYER_WIN;		
				break;
			}


			al_flip_display();	
		}
		if ((game->event.type == ALLEGRO_EVENT_KEY_DOWN) || (game->event.type == ALLEGRO_EVENT_KEY_UP)) {
			player_update_joystick(game->player1, game->player2, game->event.keyboard.keycode);
		}

		if (game->event.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
			break;
		}
	}

	box_destroy(floor);
	al_destroy_bitmap(mlf_logo);
	al_destroy_bitmap(white_bar);
	al_destroy_bitmap(background);
	al_destroy_bitmap(round);
	al_destroy_bitmap(icon_p1);
	al_destroy_bitmap(icon_p2);
}

void mlf_player_win(struct mlf *game)
{
	struct box *floor = box_create(X_SCREEN*0.5, Y_SCREEN*0.9, X_SCREEN, Y_SCREEN*0.1, RESIZE_SCREEN);
	ALLEGRO_BITMAP *mlf_logo = al_load_bitmap("./sprites/menu/mlf_logo.png");
	ALLEGRO_BITMAP *white_bar = al_load_bitmap("./sprites/menu/white_bar.png");
	ALLEGRO_BITMAP *victory = al_load_bitmap("./sprites/text/victory.png");
	ALLEGRO_BITMAP *background = background_sel(game->back);
	ALLEGRO_BITMAP *icon_p1 = player_icon_sel(game->player1->id, 1);
	ALLEGRO_BITMAP *icon_p2 = player_icon_sel(game->player2->id, 2);

	ALLEGRO_BITMAP *player = player_win_sel(player_win(game->player1, game->player2));

	printf("player win: %d \n", player_win(game->player1, game->player2));

	game->player1->control->active = 0;
	game->player2->control->active = 0;
	
	short cooldown = 5 *FPS;

	while (game->state == PLAYER_WIN) {

		al_wait_for_event(game->queue, &(game->event));

		if (game->event.type == ALLEGRO_EVENT_MOUSE_AXES) {
			game->mouse_x = game->event.mouse.x;
			game->mouse_y = game->event.mouse.y;
		}
		if (game->event.type == ALLEGRO_EVENT_TIMER) {
			draw_image_resized(background, X_SCREEN/2, Y_SCREEN /2, RESIZE_SCREEN);
			
			player_animation(game->player1);
			player_animation(game->player2);

			if (cooldown == 0) {
				game->round = game->round + 1;
				game->state = START_FIGHT;
			}

			cooldown--;
			
			

			draw_image_resized(white_bar, X_SCREEN/2, Y_SCREEN /8, RESIZE_SCREEN);
			player_draw_hp(game->player1->hp, 1);
			player_draw_hp(game->player2->hp, 2);
			player_draw_dash(game->player1->dash, 1);
			player_draw_dash(game->player2->dash, 2);
			draw_image_resized(mlf_logo, X_SCREEN/2, Y_SCREEN /8, 0.5 * RESIZE_SCREEN);
			draw_image_resized(icon_p1, X_SCREEN*0.08, Y_SCREEN /8, 0.16 * RESIZE_SCREEN);
			draw_image_resized(icon_p2, X_SCREEN*0.92, Y_SCREEN /8, 0.16 * RESIZE_SCREEN);

			draw_image_resized(victory, X_SCREEN/2, Y_SCREEN*0.40, 0.3 * RESIZE_SCREEN);
			draw_image_resized(player, X_SCREEN/2, Y_SCREEN*0.50, 0.5 * RESIZE_SCREEN);

			al_flip_display();	
		}

		if (game->event.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
			break;
		}
	}

	box_destroy(floor);
	al_destroy_bitmap(mlf_logo);
	al_destroy_bitmap(white_bar);
	al_destroy_bitmap(background);
	al_destroy_bitmap(victory);
	al_destroy_bitmap(player);
	al_destroy_bitmap(icon_p1);
	al_destroy_bitmap(icon_p2);
}



void mlf_update_game(struct mlf *game)
{
	switch (game->state) {
		
		case MENU_START:
			mlf_menu_start(game);
			break;
		case MENU_PLAYER_SEL:
			mlf_menu_player_sel(game);
			break;
		case MENU_BACK_SEL:
			mlf_menu_back_sel(game);
			break;
		case START_FIGHT:
			mlf_start_fight(game);
			break;
		case FIGHT:
			mlf_fight(game);
			break;
		case PLAYER_WIN:
			mlf_player_win(game);
			break;
	}
}
	
