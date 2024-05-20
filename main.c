#include <stdio.h>

#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>

int main()
{
    al_init(); // inicia requisitos da biblioteca
    al_install_keyboard(); // habilita entrada de eventos via teclado
	
	ALLEGRO_TIMER *timer = al_create_timer(1.0/ 30.0); // 30 fps
	ALLEGRO_EVENT_QUEUE *queue = al_create_event_queue(); // cria fila de eventos
	ALLEGRO_FONT *font = al_create_builtin_font(); // carrega fonte padrao
	ALLEGRO_DISPLAY * disp = al_create_display(1080, 1080); // cria janela

	// adiciona eventos de teclado, display e tempo na fila de eventos
	al_register_event_source(queue, al_get_keyboard_event_source());
	al_register_event_source(queue, al_get_display_event_source());
	al_register_event_source(queue, al_get_timer_event_source());
    
 
}
