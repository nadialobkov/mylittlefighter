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
	struct button *button_start = button_create("button_start.png", 960, 712);

	while (1) {
		
		al_wait_for_event(game->queue, &(game->event));
		
		if (game->event.type == ALLEGRO_EVENT_MOUSE_AXES) {
			game->mouse_x = game->event.mouse.x;
			game->mouse_y = game->event.mouse.y;
		}
		if (game->event.type == ALLEGRO_EVENT_TIMER) {	
			draw_image("background0.png", 0, 0, 1);
			draw_image("mlf_logo.png", 960, 270, 1);
			button_update(button_start, game->mouse_x, game->mouse_y);
			al_flip_display();	
		}
		if (game->event.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN) {
			if (button_pressed(button_start, game->mouse_x, game->mouse_y, game->event)) {
				game->state = MENU_PLAYER_SEL;
			}
		}
	}
}

void mlf_update_game(struct mlf *game)
{
	switch (game->state) {
		
		case MENU_START:
			mlf_menu_start(game);
			break;
	}
}
	
