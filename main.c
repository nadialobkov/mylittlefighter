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
    al_init(); 
	al_init_image_addon();
	al_init_primitives_addon(); 
    al_install_keyboard(); 
	al_install_mouse();


	struct mlf *game = mlf_create_game();
	mlf_draw_logo(game);	

	al_register_event_source(game->queue, al_get_keyboard_event_source());
	al_register_event_source(game->queue, al_get_display_event_source(game->disp));
	al_register_event_source(game->queue, al_get_timer_event_source(game->timer));
	al_register_event_source(game->queue, al_get_mouse_event_source());
	
	
	al_start_timer(game->timer);

	while (game->event.type != ALLEGRO_EVENT_DISPLAY_CLOSE) {
		mlf_update_game(game);
	}

	mlf_destroy_game(game);

	return 0;
}
