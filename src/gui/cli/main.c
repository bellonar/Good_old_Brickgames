#include "s21_cli_frontend.h"

int main() {
  screen_init();
  game_loop();
  endwin();
}
