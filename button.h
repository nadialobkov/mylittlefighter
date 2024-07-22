#ifndef _BUTTON_H_
#define _BUTTON_H_

// ESTRUTURA =========================================================================

struct button {
	short x;
	short y;
	short side_x;
	short side_y;
	float resize;
	ALLEGRO_BITMAP *bitmap;
};

// FUNCOES ============================================================================


struct button *button_create(char *filename, short x, short y, float resize);

void button_destroy(struct button *b);

void button_draw(struct button *b, float resize);

char button_pressed(struct button *b, short mouse_x, short mouse_y, ALLEGRO_EVENT event);

void button_update(struct button *b, short mouse_x, short mouse_y);


#endif
