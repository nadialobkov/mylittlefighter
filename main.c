#include <stdio.h>
#include <stdlib.h>

#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_primitives.h>
#include "square.h"

#define X_SCREEN 540
#define Y_SCREEN 540 

int main()
{
    al_init(); // inicia requisitos da biblioteca
	al_init_primitives_addon(); // biblioteca de figuras basicas
    al_install_keyboard(); // habilita entrada de eventos via teclado
	
	ALLEGRO_TIMER *timer = al_create_timer(1.0/ 30.0); // 30 fps
	ALLEGRO_EVENT_QUEUE *queue = al_create_event_queue(); // cria fila de eventos
	ALLEGRO_FONT *font = al_create_builtin_font(); // carrega fonte padrao
	ALLEGRO_DISPLAY * disp = al_create_display(X_SCREEN, Y_SCREEN); // cria janela

	// adiciona eventos de teclado, display e tempo na fila de eventos
	al_register_event_source(queue, al_get_keyboard_event_source());
	al_register_event_source(queue, al_get_display_event_source(disp));
	al_register_event_source(queue, al_get_timer_event_source(timer));


	// criacao dos personagens
	struct square *player_1 = square_create(50, 100, 150, Y_SCREEN/2, X_SCREEN, Y_SCREEN);
	if (!player_1)
		return 1;

	struct square *player_2 = square_create(80, 120, X_SCREEN-150, Y_SCREEN/2, X_SCREEN, Y_SCREEN);
	if (!player_1)
		return 2;
	


	ALLEGRO_EVENT event; // variavel que guarda um evento capturado
	al_start_timer(timer); // inicializa o relogio

	while (1) {
		al_wait_for_event(queue, &event); // captura os eventos inserindo em event

		if (event.type == 30) { // batida do relogio		

			al_clear_to_color(al_map_rgb(0, 0, 0)); // pinta fundo preto
			// desenha os personagens (quadrilateros)
			al_draw_filled_rectangle(player_1->x - player_1->side_x /2, player_1->y - player_1->side_y /2,
									 player_1->x + player_1->side_x /2, player_1->y + player_1->side_y /2, 
									 al_map_rgb(255, 102, 255));

			al_draw_filled_rectangle(player_2->x - player_2->side_x /2, player_2->y - player_2->side_y /2,
									 player_2->x + player_2->side_x /2, player_2->y + player_2->side_y /2, 
									 al_map_rgb(153, 102, 255));
									 
			al_flip_display();
		}
		if (event.type == 42) break;

	}

	// destruimos tudo que inicializamos
	al_destroy_font(font);
	al_destroy_display(disp);
	al_destroy_timer(timer);
	al_destroy_event_queue(queue);
	square_destroy(player_1);
	square_destroy(player_2);

	return 0;
}
