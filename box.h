#ifndef _BOX_H_
#define _BOX_H_

struct box {
	short x;
	short y;
	short side_x;
	short side_y;
	char active;
};

struct box *box_create(short x, short y, short side_x, short side_y, char active);

void box_destroy(struct box *box);

void box_draw(struct box *box1);

char box_valid_position(struct box *box1);

char box_collision(struct box *box1, struct box *box2);

#endif
