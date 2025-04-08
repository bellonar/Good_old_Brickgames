//
// Created by bellonar on 27.07.24.
//

#ifndef CPP3_BRICKGAME_V2_0_1_S21_API_H
#define CPP3_BRICKGAME_V2_0_1_S21_API_H

#define FIELD_ROWS 20
#define FIELD_COLS 10

#include <stdbool.h>

typedef enum {
  Start,
  Pause,
  Terminate,
  Left,
  Right,
  Up,
  Down,
  Action
} UserAction_t;

typedef struct {
  int **field;
  int **next;
  int score;
  int high_score;
  int level;
  int speed;
  int pause;
} GameInfo_t;

#ifdef __cplusplus
extern "C" {
#endif
void userInput(UserAction_t action, bool hold);
GameInfo_t updateCurrentState();
#ifdef __cplusplus
}
#endif

#endif  // CPP3_BRICKGAME_V2_0_1_S21_API_H
