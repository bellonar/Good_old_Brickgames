#include <ncurses.h>

#include "../../brick_game/s21_api.h"

#define START_FIELD_ROW 0
#define START_FIELD_COL 0
#define HALF_PIXEL 1
#define PIXEL_MULT 2

#define ESCAPE 27
#define ENTER_KEY 10
#define LEFT_ARROW 4
#define UP_ARROW 3
#define RIGHT_ARROW 5
#define DOWN_ARROW 2
#define PAUSE_BTN 112
#define SPACE 32

void screen_init();
void destroy_screen();

void game_loop();
int get_action(UserAction_t *action);
void print_current_state(GameInfo_t game_info);

void print_game_stage(GameInfo_t game_info);

void print_stats_field(GameInfo_t game_info);

void print_start_banner();
void print_gameover_banner();
void print_stats(GameInfo_t game_info);
void print_highscore(int highscore);
void print_score(int score);
void print_next(int **next);
void print_level(int level);
void print_speed(int speed);
void print_pause();
void print_frames();
void print_game_field(int **field);

void print_square(int start_row, int start_col, int width, int height);
void print_corners(int start_row, int start_col, int width, int height);
void print_lines(int start_row, int start_col, int width, int height);
