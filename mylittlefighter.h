#ifndef _MYLITTLEFIGHTER_H_
#define _MYLITTLEFIGHTER_H_


#define X_SCREEN 1920
#define Y_SCREEN 1080
#define RESIZE_SCREEN 1
#define FPS 24
#define PVP 0
#define BOT 1


enum Game_state {
	MENU_START,
	MENU_PLAYER_SEL,
	MENU_BACK_SEL,
	START_FIGHT,
	FIGHT,
	PLAYER_WIN,
	WINNER,
};

enum Backgrounds {
	CASTLE,
	PONYVILLE,
	CLOUDSDALE,
};

// ESTRUTURA ===========================================

struct mlf {
	enum Game_state state;
	short round;
	char paused;
	char mode;
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

// FUNCOES =============================================

// aloca estrutura e a inicializa
struct mlf *mlf_create_game();

// muda jogo para suas configuracoes iniciais
void mlf_reset_game(struct mlf *game);

// desaloca estrutura
void mlf_destroy_game(struct mlf *game);

// desenha logo mlf no icone do display
void mlf_draw_logo(struct mlf *game);

// atualiza estado do jogo
void mlf_update_game(struct mlf *game);


// funcoes que tratam cada estado ---------------------

void mlf_menu_start(struct mlf *game);

void mlf_menu_player_sel(struct mlf *game);

void mlf_menu_back_sel(struct mlf *game);

void mlf_start_fight(struct mlf *game);

void mlf_fight(struct mlf *game);

void mlf_player_win(struct mlf *game);

void mlf_winner(struct mlf *game);
	

#endif
