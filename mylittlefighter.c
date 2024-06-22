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

void draw_background(enum Backgrounds back)
{
	ALLEGRO_BITMAP *castle = al_load_bitmap("./sprites/backgrounds/castle.png");
	ALLEGRO_BITMAP *ponyville = al_load_bitmap("./sprites/backgrounds/ponyville.png");
	ALLEGRO_BITMAP *cloudsdale = al_load_bitmap("./sprites/backgrounds/sky.png");

	switch (back) {
		case CASTLE:
			draw_image_resized(castle, X_SCREEN*0.5, Y_SCREEN*0.5, 1);
			break;
		case PONYVILLE:
			draw_image_resized(ponyville, X_SCREEN*0.5, Y_SCREEN*0.5, 1);
			break;
		case CLOUDSDALE:
			draw_image_resized(cloudsdale, X_SCREEN*0.5, Y_SCREEN*0.5, 1);
			break;
	}
	
	al_destroy_bitmap(castle);
	al_destroy_bitmap(ponyville);
	al_destroy_bitmap(cloudsdale);
}

ALLEGRO_BITMAP *background_sel(enum Backgrounds back)
{
	ALLEGRO_BITMAP *background;

	switch (back) {
		case CASTLE:
			background = al_load_bitmap("./sprites/backgrounds/castle.png");
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


struct mlf *mlf_create_game()
{
	struct mlf *game = malloc(sizeof(struct mlf));

	game->state = MENU_START;
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
	struct button *button_start = button_create("./sprites/buttons/button_start.png", X_SCREEN /2, Y_SCREEN /2, 1);
	ALLEGRO_BITMAP *castle = al_load_bitmap("./sprites/backgrounds/castle.png");
	ALLEGRO_BITMAP *mlf_logo = al_load_bitmap("./sprites/menu/mlf_logo.png");


	while (game->state == MENU_START) {
		
		al_wait_for_event(game->queue, &(game->event));
		
		if (game->event.type == ALLEGRO_EVENT_MOUSE_AXES) {
			game->mouse_x = game->event.mouse.x;
			game->mouse_y = game->event.mouse.y;
		}
		if (game->event.type == ALLEGRO_EVENT_TIMER) {	
			draw_image_resized(castle, X_SCREEN /2, Y_SCREEN /2, 1);
			draw_image_resized(mlf_logo, X_SCREEN /2, Y_SCREEN /5, 1);
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
	struct button *p1_icon0 = button_create("./sprites/buttons/icon_pinkie.png", X_SCREEN /4, Y_SCREEN /2 + Y_SCREEN/15, 0.1);
	struct button *p1_icon1 = button_create("./sprites/buttons/icon_rarity.png", X_SCREEN /4 + X_SCREEN /15, Y_SCREEN /2 + Y_SCREEN/15, 0.1);
	struct button *p2_icon0 = button_create("./sprites/buttons/icon_pinkie_ch.png", X_SCREEN /2 + X_SCREEN /15 + 40, Y_SCREEN /2 + Y_SCREEN/15, 0.1);
	struct button *p2_icon1 = button_create("./sprites/buttons/icon_rarity_ch.png", X_SCREEN /2 + X_SCREEN *2/15 + 40, Y_SCREEN /2 + Y_SCREEN/15, 0.1);
	struct button *back = button_create("./sprites/buttons/back.png", X_SCREEN/12, Y_SCREEN*10/12, 1);
	struct button *next = button_create("./sprites/buttons/next.png", X_SCREEN*11/12, Y_SCREEN*10/12, 1);
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
			draw_image_resized(castle, X_SCREEN*0.5, Y_SCREEN*0.5, 1);
			draw_image_resized(mlf_logo, X_SCREEN*0.1, Y_SCREEN*0.12, 0.5);
			draw_image_resized(choose_player_display, X_SCREEN*0.5, Y_SCREEN*0.45, 1);
			button_update(back, game->mouse_x, game->mouse_y);
			button_update(p1_icon0, game->mouse_x, game->mouse_y);
			button_update(p1_icon1, game->mouse_x, game->mouse_y);
			button_update(p2_icon0, game->mouse_x, game->mouse_y);
			button_update(p2_icon1, game->mouse_x, game->mouse_y);

			if (player1_id == PINKIE)
				draw_image_resized(text_pinkie, X_SCREEN/3, Y_SCREEN*0.84, 1);
			if (player2_id == PINKIE)
				draw_image_resized(text_pinkie, X_SCREEN*2/3, Y_SCREEN*8.4/10, 1);
			if (player1_id == RARITY)
				draw_image_resized(text_rarity, X_SCREEN/3, Y_SCREEN*8.4/10, 1);
			if (player2_id == RARITY)
				draw_image_resized(text_rarity, X_SCREEN*2/3, Y_SCREEN*8.4/10, 1);

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


	game->player1 = player_create(player1_id, X_SCREEN/4, Y_SCREEN*3/4, 0.2);
	game->player2 = player_create(player2_id, X_SCREEN*3/4, Y_SCREEN*3/4, 0.2);

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
	struct button *back = button_create("./sprites/buttons/back.png", X_SCREEN/12, Y_SCREEN*10/12, 1);
	struct button *next = button_create("./sprites/buttons/next.png", X_SCREEN*11/12, Y_SCREEN*10/12, 1);
	struct button *castle = button_create("./sprites/buttons/icon_castle.png", X_SCREEN/4, Y_SCREEN/2, 0.4);
	struct button *ponyville = button_create("./sprites/buttons/icon_ponyville.png", X_SCREEN/2, Y_SCREEN/2, 0.4);
	struct button *cloudsdale = button_create("./sprites/buttons/icon_sky.png", X_SCREEN*3/4, Y_SCREEN/2, 0.4);
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
			draw_image_resized(castle_back, X_SCREEN /2, Y_SCREEN /2, 1);
			draw_image_resized(mlf_logo, X_SCREEN /10, Y_SCREEN /8, 0.5);
			draw_image_resized(choose_background, X_SCREEN /2, Y_SCREEN /10, 1);
			button_update(back, game->mouse_x, game->mouse_y);
			button_update(next, game->mouse_x, game->mouse_y);
			button_update(castle, game->mouse_x, game->mouse_y);
			button_update(ponyville, game->mouse_x, game->mouse_y);
			button_update(cloudsdale, game->mouse_x, game->mouse_y);

			if (game->back == CASTLE)
				draw_image_resized(text_castle, X_SCREEN /2, Y_SCREEN*0.84, 1);
			if (game->back == PONYVILLE)
				draw_image_resized(text_ponyville, X_SCREEN /2, Y_SCREEN*0.84, 1);
			if (game->back == CLOUDSDALE)
				draw_image_resized(text_cloudsdale, X_SCREEN /2, Y_SCREEN*0.84, 1);

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
	struct box *floor = box_create(X_SCREEN*0.5, Y_SCREEN*0.85, X_SCREEN, Y_SCREEN*0.1, 1);
	ALLEGRO_BITMAP *mlf_logo = al_load_bitmap("./sprites/menu/mlf_logo.png");
	ALLEGRO_BITMAP *white_bar = al_load_bitmap("./sprites/menu/white_bar.png");
	ALLEGRO_BITMAP *background = background_sel(game->back);
	
	short cooldown = 0;

	while (game->state == START_FIGHT) {

		al_wait_for_event(game->queue, &(game->event));

		if (game->event.type == ALLEGRO_EVENT_MOUSE_AXES) {
			game->mouse_x = game->event.mouse.x;
			game->mouse_y = game->event.mouse.y;
		}
		if (game->event.type == ALLEGRO_EVENT_TIMER) {
			//draw_background(game->back);
			draw_image_resized(background, X_SCREEN/2, Y_SCREEN /2, 1);
			box_draw(floor, 0, 153, 51);
			player_move(game->player1, game->player2, floor);
			box_draw(game->player1->hitbox, 255, 122, 255);
			box_draw(game->player2->hitbox, 255, 122, 255);
			player_animation(game->player1);
			if (!cooldown) {
				player_update_state(game->player1);
				cooldown = 1;
			}
			else
				cooldown = 0;

			draw_image_resized(white_bar, X_SCREEN/2, Y_SCREEN /8, 1);
			draw_image_resized(mlf_logo, X_SCREEN/2, Y_SCREEN /8, 0.5);

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

	}
}
	
