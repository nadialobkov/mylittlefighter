#include <stdio.h>
#include <stdlib.h>

#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>

#include "streetfighter.h"
#include "player.h"
#include "box.h"
#include "joystick.h"
#include "button.h"

#define X_SCREEN 540
#define Y_SCREEN 540 



int main()
{
    al_init(); // inicia requisitos da biblioteca
	al_init_image_addon();
	al_init_primitives_addon(); // biblioteca de figuras basicas
    al_install_keyboard(); // habilita entrada de eventos via teclado
	al_install_mouse();
	
	ALLEGRO_TIMER *timer = al_create_timer(1.0/ 5.0); // 30 fps
	ALLEGRO_EVENT_QUEUE *queue = al_create_event_queue(); // cria fila de eventos
	ALLEGRO_FONT *font = al_create_builtin_font(); // carrega fonte padrao
	ALLEGRO_DISPLAY * disp = al_create_display(X_SCREEN, Y_SCREEN); // cria janela

#ifdef DEBUG
	printf("Estou debugando codigo\n");
#endif

	// adiciona eventos de teclado, display e tempo na fila de eventos
	al_register_event_source(queue, al_get_keyboard_event_source());
	al_register_event_source(queue, al_get_display_event_source(disp));
	al_register_event_source(queue, al_get_timer_event_source(timer));
	al_register_event_source(queue, al_get_mouse_event_source());
	

	struct player *player1 = player_create(PINKIE, 200, 300);
	struct button *botao = button_create("carinha.png", 400, 300);
	ALLEGRO_EVENT event; // variavel que guarda um evento capturado
	al_start_timer(timer); // inicializa o relogio
	short mouse_x = event.mouse.x;
	short mouse_y = event.mouse.y;

	while (1) {
		al_wait_for_event(queue, &event); // captura os eventos inserindo em event

		if (event.type == ALLEGRO_EVENT_TIMER) { // batida do relogio		

			al_clear_to_color(al_map_rgb(30, 30, 50)); // pinta fundo preto
			player_update_state(player1);
			player_update_position(player1);

			button_update(botao, mouse_x, mouse_y);
									 
			al_flip_display();
		}
		else
			// caso uma tecla foi pressionada ou solta 
			if ((event.type == ALLEGRO_EVENT_KEY_DOWN) || (event.type == ALLEGRO_EVENT_KEY_UP)) {
				// identificamos tecla e atalizamos o joystick do player
				if (event.keyboard.keycode == ALLEGRO_KEY_A) joystick_left(player1->control);
				else if (event.keyboard.keycode == ALLEGRO_KEY_D) joystick_right(player1->control);
				else if (event.keyboard.keycode == ALLEGRO_KEY_W) joystick_up(player1->control);
				else if (event.keyboard.keycode == ALLEGRO_KEY_S) joystick_down(player1->control);
				//else if (event.keyboard.keycode == ALLEGRO_KEY_LEFT) joystick_left(player_2->control);
				//else if (event.keyboard.keycode == ALLEGRO_KEY_RIGHT) joystick_right(player_2->control);
				//else if (event.keyboard.keycode == ALLEGRO_KEY_UP) joystick_up(player_2->control);
				//else if (event.keyboard.keycode == ALLEGRO_KEY_DOWN) joystick_down(player_2->control);
			}
		
		if (event.type == ALLEGRO_EVENT_MOUSE_AXES){
			mouse_x = event.mouse.x;
			mouse_y = event.mouse.y;
			if (button_pressed(botao, mouse_x, mouse_y, event))
				printf("oieee\n");
		}
		if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE) break;

	}

	// destruimos tudo que inicializamos
	al_destroy_font(font);
	al_destroy_display(disp);
	al_destroy_timer(timer);
	al_destroy_event_queue(queue);
	player_destroy(player1);
	button_destroy(botao);
	return 0;
}
