#ifndef CPP3_BRICKGAME_V2_0_1_S21_SNAKE_BACKEND_H
#define CPP3_BRICKGAME_V2_0_1_S21_SNAKE_BACKEND_H

#include "../s21_api.h"
#include "s21_snake_model.h"

#define FIELD_ROWS 20
#define FIELD_COLS 10

using namespace s21;

void start_actions(UserAction_t action);
void moving_actions(UserAction_t action);
void pause_actions(UserAction_t action);
void gameover_actions(UserAction_t action);
void spawn_action();
void attaching_action();
void shifting_action();
void terminate_action();
int **create_matrix(int rows, int cols);

#endif  // CPP3_BRICKGAME_V2_0_1_S21_SNAKE_BACKEND_H
