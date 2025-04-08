#include "s21_cli_frontend.h"

void game_loop() {
  UserAction_t action = Start;
  while (action != Terminate) {
    if (get_action(&action) == 1) {
      userInput(action, 0);
    }
    if (action != Terminate) {
      GameInfo_t info = updateCurrentState();
      print_current_state(info);
    }
  }
}

void screen_init() {
  initscr();
  noecho();
  curs_set(0);
  timeout(10);
  keypad(stdscr, TRUE);
}

void print_current_state(GameInfo_t game_info) {
  erase();
  print_game_stage(game_info);
  if (game_info.pause == 1) {
    print_pause();
  }
}

void print_game_stage(GameInfo_t game_info) {
  print_stats_field(game_info);

  print_game_field(game_info.field);
}

void print_stats_field(GameInfo_t game_info) {
  print_frames();

  print_stats(game_info);
}

void print_stats(GameInfo_t game_info) {
  print_highscore(game_info.high_score);
  print_score(game_info.score);

  if (game_info.next != NULL) {
    print_next(game_info.next);
  }

  print_level(game_info.level);
  print_speed(game_info.speed);
}

void print_highscore(int highscore) {
  mvprintw(1, 26, "High_Score");
  mvprintw(2, 26, "%d", highscore);
}

void print_score(int score) {
  mvprintw(3, 26, "Score");
  mvprintw(4, 26, "%d", score);
}

void print_next(int **next) {
  mvprintw(5, 26, "Next");
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      if (next[i][j] == 1) {
        mvaddch(6 + i, 26 + j * PIXEL_MULT, '[');
        mvaddch(6 + i, 26 + j * PIXEL_MULT + HALF_PIXEL, ']');
      }
    }
  }
}

void print_level(int level) {
  mvprintw(10, 26, "Level");
  mvprintw(11, 26, "%d", level);
}

void print_speed(int speed) {
  mvprintw(10, 32, "Speed");
  mvprintw(11, 32, "%d", speed);
}

void print_frames() {
  print_square(0, 0, 23, 23);
  print_square(1, 1, 21, 21);
}

void print_square(int start_row, int start_col, int width, int height) {
  print_corners(start_row, start_col, width, height);
  print_lines(start_row, start_col, width, height);
}

void print_corners(int start_row, int start_col, int width, int height) {
  mvaddch(start_row, start_col, ACS_ULCORNER);
  mvaddch(start_row, start_col + width, ACS_URCORNER);
  mvaddch(start_row + height, start_col, ACS_LLCORNER);
  mvaddch(start_row + height, start_col + width, ACS_LRCORNER);
}

void print_lines(int start_row, int start_col, int width, int height) {
  for (int i = start_row + 1; i < start_row + height; i++) {
    mvaddch(i, start_col, ACS_VLINE);
    mvaddch(i, start_col + width, ACS_VLINE);
  }
  for (int i = start_col + 1; i < start_col + width; i++) {
    mvaddch(start_row, i, ACS_HLINE);
    mvaddch(start_row + height, i, ACS_HLINE);
  }
}

void print_game_field(int **field) {
  for (int i = START_FIELD_ROW; i < FIELD_ROWS; i++) {
    for (int j = START_FIELD_COL; j < FIELD_COLS; j++) {
      if (field[i][j] != 0) {
        mvaddch(2 + i, 2 + j * PIXEL_MULT, '[');
        mvaddch(2 + i, 2 + j * PIXEL_MULT + HALF_PIXEL, ']');
      }
      if (field[i][j] == 2) {
        mvaddch(2 + i, 2 + j * PIXEL_MULT, '{');
        mvaddch(2 + i, 2 + j * PIXEL_MULT + HALF_PIXEL, '}');
      }
    }
  }
}

void print_pause() { mvprintw(13, 26, "PAUSE"); }

int get_action(UserAction_t *action) {
  int status = 1;
  char sig = getch();
  switch (sig) {
    case LEFT_ARROW:
      *action = Left;
      break;
    case RIGHT_ARROW:
      *action = Right;
      break;
    case DOWN_ARROW:
      *action = Down;
      break;
    case UP_ARROW:
      *action = Up;
      break;
    case PAUSE_BTN:
      *action = Pause;
      break;
    case ENTER_KEY:
      *action = Start;
      break;
    case SPACE:
      *action = Action;
      break;
    case ESCAPE:
      *action = Terminate;
      break;
    default:
      status = 0;
  }
  return status;
}
