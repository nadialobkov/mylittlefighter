#include <stdio.h>
#include <stdlib.h>

#include <allegro5/allegro5.h>
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
	
	
	al_start_timer(game->timer);

	mlf_menu_start(game);
	mlf_menu_player_sel(game);

	mlf_destroy_game(game);

	return 0;
}
