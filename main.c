#include <stdio.h>
#include <stdlib.h>

#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>

#include "mylittlefighter.h"
#include "player.h"
#include "box.h"
#include "joystick.h"
#include "button.h"



int main()
{
    al_init(); // inicia requisitos da biblioteca
	al_init_image_addon();
	al_init_primitives_addon(); // biblioteca de figuras basicas
    al_install_keyboard(); // habilita entrada de eventos via teclado
	al_install_mouse();

	struct mlf *game = mlf_create_game();


	// adiciona eventos de teclado, display e tempo na fila de eventos
	al_register_event_source(game->queue, al_get_keyboard_event_source());
	al_register_event_source(game->queue, al_get_display_event_source(game->disp));
	al_register_event_source(game->queue, al_get_timer_event_source(game->timer));
	al_register_event_source(game->queue, al_get_mouse_event_source());
	
	struct button *button_start = button_create("button_start.png", 960, 550);
	
	game->mouse_x = 0;	
	game->mouse_y = 0;	
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
		if (game->event.type == ALLEGRO_EVENT_DISPLAY_CLOSE) 
			break;
	}
	mlf_destroy_game(game);
	return 0;
}
