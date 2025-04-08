#include "s21_snake_backend.h"
using namespace s21;

GameInfo_t updateCurrentState() {
  model *game = model::get_model();
  game->update();
  GameInfo_t info = game->get_info();
  return info;
}

void userInput(UserAction_t action, bool hold) {
  if (hold) {
  };
  model *game = model::get_model();
  game->update(action);
}
