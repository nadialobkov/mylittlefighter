#include <stdio.h>
#include <stdlib.h>

#include <allegro5/allegro5.h>
#include <allegro5/allegro_image.h>

#include "mylittlefighter.h"
#include "player.h"
#include "button.h"

void draw_image(char *filename, short x, short y, float resize)
{
	ALLEGRO_BITMAP *bitmap = al_load_bitmap(filename);

	short side_x = al_get_bitmap_width(bitmap);
	short side_y = al_get_bitmap_height(bitmap);

	short new_x = x - side_x/(2/resize);
	short new_y = y - side_y/(2/resize);

	al_draw_scaled_bitmap(bitmap, 0, 0, side_x, side_y, new_x, new_y, side_x * resize, side_y * resize, 0);	
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

	while (game->state == MENU_START) {
		
		al_wait_for_event(game->queue, &(game->event));
		
		if (game->event.type == ALLEGRO_EVENT_MOUSE_AXES) {
			game->mouse_x = game->event.mouse.x;
			game->mouse_y = game->event.mouse.y;
		}
		if (game->event.type == ALLEGRO_EVENT_TIMER) {	
			draw_image("./sprites/backgrounds/castle.png", X_SCREEN /2, Y_SCREEN /2, 1);
			draw_image("./sprites/menu/mlf_logo.png", X_SCREEN /2, Y_SCREEN /5, 1);
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
}

void mlf_menu_player_sel(struct mlf *game)
{
	struct button *p1_icon0 = button_create("./sprites/buttons/icon_pinkie.png", X_SCREEN /4, Y_SCREEN /2 + Y_SCREEN/15, 0.1);
	struct button *p1_icon1 = button_create("./sprites/buttons/icon_rarity.png", X_SCREEN /4 + X_SCREEN /15, Y_SCREEN /2 + Y_SCREEN/15, 0.1);
	struct button *p2_icon0 = button_create("./sprites/buttons/icon_pinkie_ch.png", X_SCREEN /2 + X_SCREEN /15 + 40, Y_SCREEN /2 + Y_SCREEN/15, 0.1);
	struct button *p2_icon1 = button_create("./sprites/buttons/icon_rarity_ch.png", X_SCREEN /2 + X_SCREEN *2/15 + 40, Y_SCREEN /2 + Y_SCREEN/15, 0.1);

	while (game->state == MENU_PLAYER_SEL) {
		
		al_wait_for_event(game->queue, &(game->event));
		
		if (game->event.type == ALLEGRO_EVENT_MOUSE_AXES) {
			game->mouse_x = game->event.mouse.x;
			game->mouse_y = game->event.mouse.y;
		}
		if (game->event.type == ALLEGRO_EVENT_TIMER) {	
			draw_image("./sprites/backgrounds/castle.png", X_SCREEN /2, Y_SCREEN /2, 1);
			draw_image("./sprites/menu/mlf_logo.png", X_SCREEN /10, Y_SCREEN /8, 0.5);
			draw_image("./sprites/menu/player_display_pink.png", X_SCREEN /3, Y_SCREEN /2, 1);
			draw_image("./sprites/menu/player_display_green.png", X_SCREEN *2 /3, Y_SCREEN /2 -60, 1);
			draw_image("./sprites/menu/player1_pinkie.png", X_SCREEN /3, Y_SCREEN /3 -60, 1);
			draw_image("./sprites/menu/player2_changeling.png", X_SCREEN *2 /3, Y_SCREEN /3 -60, 1);
			draw_image("./sprites/menu/text_choose_your_player.png", X_SCREEN /2, Y_SCREEN /10, 1);
			button_update(p1_icon0, game->mouse_x, game->mouse_y);
			button_update(p1_icon1, game->mouse_x, game->mouse_y);
			button_update(p2_icon0, game->mouse_x, game->mouse_y);
			button_update(p2_icon1, game->mouse_x, game->mouse_y);
			al_flip_display();	
		}
		if (game->event.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN) {
			if (button_pressed(p1_icon0, game->mouse_x, game->mouse_y, game->event)) {
				game->player1->id = PINKIE;
			}
			if (button_pressed(p1_icon1, game->mouse_x, game->mouse_y, game->event)) {
				game->player1->id = RARITY;
			}
			if (button_pressed(p2_icon0, game->mouse_x, game->mouse_y, game->event)) {
				game->player2->id = PINKIE;
			}
			if (button_pressed(p2_icon1, game->mouse_x, game->mouse_y, game->event)) {
				game->player2->id = RARITY;
			}

		}
		if (game->event.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
			break;
		}
	}

	button_destroy(p1_icon0);
	button_destroy(p1_icon1);
	button_destroy(p2_icon0);
	button_destroy(p2_icon1);
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

	}
}
	
