#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "player.h"
#include "box.h"
#include "joystick.h"

void player_load_bitmap(ALLEGRO_BITMAP  **bitmap)
{
	bitmap[0] = al_load_bitmap("ponei0.png");
	bitmap[1] = al_load_bitmap("ponei1.png");
	bitmap[2] = al_load_bitmap("ponei2.png");
	bitmap[3] = al_load_bitmap("ponei3.png");
	bitmap[4] = al_load_bitmap("ponei4.png");
	bitmap[5] = al_load_bitmap("ponei5.png");
	bitmap[6] = al_load_bitmap("ponei6.png");
	bitmap[7] = al_load_bitmap("ponei7.png");
}


struct player_create(char id, short state, short health, short x, short y)
{
	struct player *new_player = malloc(sizeof(struct player));

	new_player->id = id;
	new_player->state = state;
	new_player->health = health;
	new_player->x = x;
	new_player->y = y;

	new_player->bitmap = malloc(sizeof(ALLEGRO_BITMAP*));
	for (short i = 0; i < 10; i++) {
		new_player->bitmap[i] = malloc(sizeof(ALLEGRO_BITMAP));
	}
	player_load_bitmap(new_player->bitmap);
	
	
	new_player->control = joystick_create();
}

void player_destroy(struct player *playerD);

void player_update_position(struct player *playerP);
