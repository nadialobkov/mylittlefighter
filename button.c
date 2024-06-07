#include <stdio.h>
#include <stdlib.h>

#include <allegro5/allegro5.h>
#include <allegro5/allegro_image.h>

#include "button.h"

struct button *button_create(char *filename, short x, short y, float resize) 
{
	struct button *new_button = malloc(sizeof(struct button));
	
	new_button->x = x;
	new_button->y = y;

	new_button->bitmap = al_load_bitmap(filename);
	
	new_button->side_x = al_get_bitmap_width(new_button->bitmap);
	new_button->side_y = al_get_bitmap_height(new_button->bitmap);

	new_button->resize = resize;

	return new_button;
}

void button_destroy(struct button *b)
{
	if (!b)
		return;

	if (b->bitmap)
		al_destroy_bitmap(b->bitmap);

	free(b);
}

void button_draw(struct button *b, float resize)
{
	short x = b->x - b->side_x * resize * b->resize /2;
	short y = b->y - b->side_y * resize * b->resize /2;
	al_draw_scaled_bitmap(b->bitmap, 0, 0, b->side_x, b->side_y, x, y, b->side_x * resize * b->resize, b->side_y * resize * b->resize, 0);	
}

char button_pressed(struct button *b, short mouse_x, short mouse_y, ALLEGRO_EVENT event)
{
	short resize_x = b->side_x * b->resize;
	short resize_y = b->side_y * b->resize;

	if ( (mouse_x <= b->x + resize_x /2) && (mouse_x >= b->x - resize_x /2) &&
		 (mouse_y <= b->y + resize_y /2) && (mouse_y >= b->y - resize_y /2) &&
		 (event.mouse.button & 1))

		return 1;

	else
		return 0; 
}

void button_update(struct button *b, short mouse_x, short mouse_y)
{
	short resize_x = b->side_x * b->resize;
	short resize_y = b->side_y * b->resize;

	if ( (mouse_x <= b->x + resize_x /2) && (mouse_x >= b->x - resize_x /2) &&
		 (mouse_y <= b->y + resize_y /2) && (mouse_y >= b->y - resize_y /2)    ) {

		button_draw(b, 1.2);
	}
	else {
		button_draw(b, 1);
	}

}

