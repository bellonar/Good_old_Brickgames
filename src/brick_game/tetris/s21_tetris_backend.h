#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "../s21_api.h"

#define ESCAPE 27
#define ENTER_KEY 10
#define LEFT_ARROW 4
#define UP_ARROW 3
#define RIGHT_ARROW 5
#define DOWN_ARROW 2
#define PAUSE_BTN 112
#define SPACE 32

#define FIGURE_SIZE 4
#define FIELD_ROWS 20
#define FIELD_COLS 10

typedef enum {
  START,
  SPAWN,
  PAUSE,
  MOVING,
  SHIFTING,
  ATTACHING,
  GAMEOVER,
  TERMINATE
} State_t;
/**
 * Extended game state struct
 */
typedef struct {
  int **field;
  int **next;
  int next_size;
  int **current;
  int current_size;
  int y_pos;
  int x_pos;
  long int timer;
  State_t state;
  GameInfo_t GameInfo;
} FullGameInfo_t;

GameInfo_t updateCurrentState();
void userInput(UserAction_t action, bool hold);

void start_actions(UserAction_t action);
void moving_actions(UserAction_t action);
void pause_actions(UserAction_t action);
void gameover_actions(UserAction_t action);
void attaching_action();
void shifting_action();
void terminate_action();

void attaching();
int gameover();
void check_and_destroy_filled_lines();
int check_line(int line);
void shifting_lines();
void clear_field();
void set_figure_on_field(int **field, int **figure, int x_pos, int y_pos);
void update_score(int destroyed_lines);
void update_level();
void load_highscore();
void save_highscore();

int **create_matrix(int rows, int cols);
void copy_matrix(int **old, int **new, int rows, int cols);
void set_next_figure(int (*sample)[4], int **next);
FullGameInfo_t *get_FullGameInfo();
void init_FullGameInfo();
void destroy_FullGameInfo();

void moving_down();
void moving_left();
void moving_right();
int check_collision();
int check_borders_collision();
int check_field_collision();
void rotate();

void spawn();
void generate_figure(int **figure, int *size);
void get_figure(int figure_type, int **figure, int *size);

void set_start_field(int **field);
void set_gameover_field(int **field);

void update_timer();
