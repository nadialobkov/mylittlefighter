#ifndef _MYLITTLEFIGHTER_H_
#define _MYLITTLEFIGHTER_H_


#define X_SCREEN 1280
#define Y_SCREEN 720
#define RESIZE_SCREEN 0.67
#define FPS 24


enum Game_state {
	MENU_START,
	MENU_PLAYER_SEL,
	MENU_BACK_SEL,
	START_FIGHT,
	FIGHT,
	PLAYER_WIN,
};

enum Backgrounds {
	CASTLE,
	PONYVILLE,
	CLOUDSDALE,
};

struct mlf {
	enum Game_state state;
	short round;
	char paused;
	struct player *player1;
	struct player *player2;
	enum Backgrounds back;
	ALLEGRO_EVENT event;
	ALLEGRO_EVENT_QUEUE *queue;
	ALLEGRO_DISPLAY *disp;
	ALLEGRO_TIMER *timer;
	short mouse_x;
	short mouse_y;
};

// ============================================


struct mlf *mlf_create_game();

void mlf_destroy_game(struct mlf *game);

void draw_image(char *filename, short x, short y, float resize);

void mlf_menu_start(struct mlf *game);

void mlf_menu_player_sel(struct mlf *game);

void mlf_update_game(struct mlf *game);


#endif
