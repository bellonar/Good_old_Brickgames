#include "s21_tetris_backend.h"
/**
 * Translate extended game state to short
 * @return
 */
GameInfo_t updateCurrentState() {
  FullGameInfo_t *infoFull = get_FullGameInfo();
  switch (infoFull->state) {
    case MOVING:
      if (clock() >= infoFull->timer) {
        infoFull->state = SHIFTING;
        shifting_action();
      }
      break;
    case SPAWN:
      spawn();
      break;
    case ATTACHING:
      attaching_action();
      break;
    default:
      break;
  }
  copy_matrix(infoFull->field, infoFull->GameInfo.field, FIELD_ROWS,
              FIELD_COLS);
  set_figure_on_field(infoFull->GameInfo.field, infoFull->current,
                      infoFull->x_pos, infoFull->y_pos);
  copy_matrix(infoFull->next, infoFull->GameInfo.next, FIGURE_SIZE,
              FIGURE_SIZE);
  return infoFull->GameInfo;
}
/**
 * Transition between fsm states based on user actions
 * @param action User action
 * @param hold pressed key
 */
void userInput(UserAction_t action, bool hold) {
  if (hold) {
    ;
  }
  FullGameInfo_t *info = get_FullGameInfo();
  switch (info->state) {
    case START:
      start_actions(action);
      break;
    case MOVING:
      moving_actions(action);
      break;
    case PAUSE:
      pause_actions(action);
      break;
    case GAMEOVER:
      gameover_actions(action);
      break;
    case TERMINATE:
      terminate_action();
      break;
    default:
  }
}
/**
 * Transfer from Start state
 * @param action
 */
void start_actions(UserAction_t action) {
  FullGameInfo_t *info = get_FullGameInfo();
  switch (action) {
    case Start:
      init_FullGameInfo();
      info->state = SPAWN;
      break;
    case Terminate:
      info->state = TERMINATE;
      break;
    default:
  }
}
/**
 * Transfer from Moving state
 * @param action
 */
void moving_actions(UserAction_t action) {
  FullGameInfo_t *info = get_FullGameInfo();
  switch (action) {
    case Down:
      while (info->state != ATTACHING) {
        moving_down();
      }
      break;
    case Left:
      moving_left();
      break;
    case Right:
      moving_right();
      break;
    case Action:
      rotate();
      break;
    case Pause:
      info->GameInfo.pause = 1;
      info->state = PAUSE;
      break;
    case Terminate:
      info->state = TERMINATE;
      break;
    default:
  }
}
/**
 * Transfer from Pause state
 * @param action
 */
void pause_actions(UserAction_t action) {
  FullGameInfo_t *info = get_FullGameInfo();
  if (action == Pause) {
    info->GameInfo.pause = 0;
    info->state = MOVING;
    update_timer();
  }
  if (action == Terminate) {
    info->state = TERMINATE;
  }
}
/**
 * Transfer from Gameover state
 *
 * @param action
 */
void gameover_actions(UserAction_t action) {
  FullGameInfo_t *info = get_FullGameInfo();
  if (action == Terminate) {
    info->state = TERMINATE;
  }
  if (action == Start) {
    set_start_field(info->field);
    info->state = START;
  }
}
/**
 * Transfer from Attaching state
 */
void attaching_action() {
  FullGameInfo_t *info = get_FullGameInfo();
  attaching();
  check_and_destroy_filled_lines();
  int clear[4][4] = {0};
  set_next_figure(clear, info->current);
  if (gameover() == 0) {
    info->state = SPAWN;
  } else {
    info->state = GAMEOVER;
    set_gameover_field(info->field);
  }
}
/**
 * Transfer from Shifting state
 */
void shifting_action() {
  update_timer();
  moving_down();
}
/**
 * Transfer from Terminate state
 */
void terminate_action() { destroy_FullGameInfo(); }

/**
 * @brief Checking if a figure leaves the boundaries of the field and if the
 * figure collides with objects on the field
 * @return '0' no collision  '1' collision
 */
int check_collision() {
  int collision = 0;
  collision = check_borders_collision();
  if (collision == 0) {
    collision = check_field_collision();
  }
  return collision;
}
/**
 * @brief Check_collision() subfunction that checks if the figure is outside the
 * field boundaries
 * @return '0' no collision  '1' collision
 */
int check_borders_collision() {
  int collision = 0;
  FullGameInfo_t *info = get_FullGameInfo();
  if (info->x_pos < 0) {
    for (int i = 0; i < FIGURE_SIZE && collision == 0; i++) {
      if (info->current[i][-1 - info->x_pos] == 1) {
        collision = 1;
      }
    }
  }
  if (info->y_pos < 0) {
    for (int i = 0; i < FIGURE_SIZE && collision == 0; i++) {
      if (info->current[-1 - info->y_pos][i] == 1) {
        collision = 1;
      }
    }
  }
  if (info->x_pos + FIGURE_SIZE > FIELD_COLS) {
    for (int i = 0; i < FIGURE_SIZE && collision == 0; i++) {
      if (info->current[i][(FIELD_COLS - info->x_pos)] == 1) {
        collision = 1;
      }
    }
  }
  if (info->y_pos + FIGURE_SIZE > FIELD_ROWS) {
    for (int i = 0; i < FIGURE_SIZE && collision == 0; i++) {
      if (info->current[FIELD_ROWS - info->y_pos][i] == 1) {
        collision = 1;
      }
    }
  }
  return collision;
}
/**
 *@brief Check_collision() subfunction that checks for collision of a figure
 *with objects on the field
 * @return '0' no collision  '1' collision
 */
int check_field_collision() {
  int collision = 0;
  FullGameInfo_t *info = get_FullGameInfo();
  for (int i = 0; i < FIGURE_SIZE && collision == 0; i++) {
    for (int j = 0; j < FIGURE_SIZE; j++) {
      if ((info->current[i][j] == 1) &&
          (info->field[info->y_pos + i][info->x_pos + j] == 1)) {
        collision = 1;
      }
    }
  }
  return collision;
}

/**
 * @brief Checks the top row of the field for filled cells. If there is at least
 * one filled cell, the game is over
 * @return '0' no gameover  '1' gameover
 */
int gameover() {
  int status = 0;
  FullGameInfo_t *info = get_FullGameInfo();
  for (int j = 0; j < FIELD_COLS && status == 0; j++) {
    if (info->field[0][j] == 1) {
      status = 1;
    }
  }
  return status;
}
/**
 * Checking filled lines, destroying them and shifting them
 */
void check_and_destroy_filled_lines() {
  int destroyed_lines = 0;
  for (int i = 19; i >= 0; i--) {
    if (check_line(i) == 1) {
      destroyed_lines++;
      shifting_lines(i);
      i++;
    }
  }
  update_score(destroyed_lines);
  update_level();
}
/**
 * Checking filled lines
 * @param line number of line
 * @return '0' not filled '1' filled
 */
int check_line(int line) {
  int status = 0;
  FullGameInfo_t *info = get_FullGameInfo();
  for (int j = 0; j < FIELD_COLS && info->field[line][j] != 0; j++) {
    if (j == 9 && info->field[line][j] == 1) {
      status = 1;
    }
  }
  return status;
}
/**
 * Shifting lines down
 * @param start_line
 */
void shifting_lines(int start_line) {
  FullGameInfo_t *info = get_FullGameInfo();
  for (int i = start_line; i > 0; i--) {
    for (int j = 0; j < FIELD_COLS; j++) {
      info->field[i][j] = info->field[i - 1][j];
    }
  }
  for (int j = 0; j < FIELD_COLS; j++) {
    info->field[0][j] = 0;
  }
}

/**
 * @brief Function for dynamic memory allocation for matrices
 * @param rows Matrix rows
 * @param cols Matrix cols
 * @return Matrix address
 */
int **create_matrix(int rows, int cols) {
  int **matrix =
      (int **)calloc(1, rows * sizeof(int *) + cols * rows * sizeof(int));

  matrix[0] = (int *)(matrix + rows);
  for (int i = 1; i < rows; i++) {
    matrix[i] = matrix[0] + i * cols;
  }
  return matrix;
}
/**
 *@brief Function for copying values from one matrix to another
 * @param old Matrix we copy from
 * @param new Matrix we copy into
 * @param rows matrix rows
 * @param cols matrix cols
 */
void copy_matrix(int **old, int **new, int rows, int cols) {
  for (int i = 0; i < rows; i++) {
    for (int j = 0; j < cols; j++) {
      new[i][j] = old[i][j];
    }
  }
}
/**
 * @brief The function of inserting a sample figure into the desired field
 * @param sample sample figure
 * @param next next or current figure
 */
void set_next_figure(int (*sample)[4], int **next) {
  for (int i = 0; i < FIGURE_SIZE; i++) {
    for (int j = 0; j < FIGURE_SIZE; j++) {
      next[i][j] = sample[i][j];
    }
  }
}
/**
 * @brief Initialization of extended structure for game state description
 */
void init_FullGameInfo() {
  FullGameInfo_t *info = get_FullGameInfo();
  srand(time(NULL));
  clear_field();
  generate_figure(info->next, &info->next_size);
  info->GameInfo.level = 1;
  info->GameInfo.score = 0;
  info->GameInfo.speed = 1;
  info->GameInfo.pause = 0;
  update_timer();
  load_highscore();
}
/**
 * @brief Setting the figure on the field
 * @param field Gaming field
 * @param figure Current figure
 * @param x_pos x position of figure
 * @param y_pos y position of figure
 */
void set_figure_on_field(int **field, int **figure, int x_pos, int y_pos) {
  for (int i = 0; i < FIGURE_SIZE; i++) {
    for (int j = 0; j < FIGURE_SIZE; j++) {
      if (figure[i][j] == 1) {
        field[y_pos + i][x_pos + j] = 1;
      }
    }
  }
}
/**
 * @brief Zeroing out all elements in the field
 */
void clear_field() {
  FullGameInfo_t *info = get_FullGameInfo();
  for (int i = 0; i < FIELD_ROWS; i++) {
    for (int j = 0; j < FIELD_COLS; j++) {
      info->field[i][j] = 0;
    }
  }
}
/**
 * @brief Level updates based on the player's score
 */
void update_level() {
  FullGameInfo_t *info = get_FullGameInfo();
  if (info->GameInfo.score >= 600) {
    info->GameInfo.level = info->GameInfo.score / 600 + 1;
  }
  if (info->GameInfo.level > 10) {
    info->GameInfo.level = 10;
  }
}
/**
 * @brief Updates the player's scores based on the number of destroyed lines.
 * If the current score exceeds the highscore , the highscore is overwritten.
 * @param destroyed_lines number of destroyed lines
 */
void update_score(int destroyed_lines) {
  FullGameInfo_t *info = get_FullGameInfo();
  switch (destroyed_lines) {
    case 1:
      info->GameInfo.score += 100;
      break;
    case 2:
      info->GameInfo.score += 300;
      break;
    case 3:
      info->GameInfo.score += 700;
      break;
    case 4:
      info->GameInfo.score += 1500;
      break;
    default:
  }
  if (info->GameInfo.score > info->GameInfo.high_score) {
    info->GameInfo.high_score = info->GameInfo.score;
    save_highscore();
  }
}
/**
 * @brief Saving highscore to a file
 */
void save_highscore() {
  FullGameInfo_t *info = get_FullGameInfo();
  FILE *file = NULL;
  file = fopen("tetris_high_score.txt", "w");
  if (file) {
    fprintf(file, "%d", info->GameInfo.high_score);
    fclose(file);
  }
}
/**
 * @brief Load highscore from a file
 */
void load_highscore() {
  FullGameInfo_t *info = get_FullGameInfo();
  FILE *file = NULL;
  int high_score = 0;
  file = fopen("tetris_high_score.txt", "r");
  if (file) {
    fscanf(file, "%d", &high_score);
    info->GameInfo.high_score = high_score;
    fclose(file);
  } else {
    info->GameInfo.high_score = 0;
  }
}
/**
 * @brief Getting a static instance of the extended structure describing the
 * game state
 * @return Address of the extended structure
 */
FullGameInfo_t *get_FullGameInfo() {
  static FullGameInfo_t info = {0};
  if (info.field == NULL) {
    info.GameInfo.field = create_matrix(FIELD_ROWS, FIELD_COLS);
    info.GameInfo.next = create_matrix(FIGURE_SIZE, FIGURE_SIZE);
    info.field = create_matrix(FIELD_ROWS, FIELD_COLS);
    set_start_field(info.field);
    info.current = create_matrix(FIGURE_SIZE, FIGURE_SIZE);
    info.next = create_matrix(FIGURE_SIZE, FIGURE_SIZE);
  }
  return &info;
}

void set_start_field(int **field) {
  int start[20][10] = {
      {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 1, 1, 1, 0, 0, 0, 0, 0, 0},
      {0, 1, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 1, 1, 1, 0, 0, 0, 0, 0, 0},
      {0, 0, 0, 1, 0, 0, 1, 1, 1, 0}, {0, 1, 1, 1, 0, 0, 0, 1, 0, 0},
      {0, 0, 0, 0, 0, 0, 0, 1, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 1, 0, 0},
      {0, 0, 1, 0, 0, 0, 0, 0, 0, 0}, {0, 1, 0, 1, 0, 0, 0, 0, 0, 0},
      {0, 1, 1, 1, 0, 0, 0, 0, 0, 0}, {0, 1, 0, 1, 0, 0, 1, 1, 1, 0},
      {0, 1, 0, 1, 0, 0, 1, 0, 1, 0}, {0, 0, 0, 0, 0, 0, 1, 1, 1, 0},
      {0, 0, 0, 0, 0, 0, 1, 1, 0, 0}, {0, 0, 0, 0, 0, 0, 1, 0, 1, 0},
      {0, 1, 1, 1, 0, 0, 0, 0, 0, 0}, {0, 0, 1, 0, 0, 0, 0, 0, 0, 0},
      {0, 0, 1, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 1, 0, 0, 0, 0, 0, 0, 0},
  };
  for (int i = 0; i < FIELD_ROWS; i++) {
    for (int j = 0; j < FIELD_COLS; j++) {
      field[i][j] = start[i][j];
    }
  }
}

void set_gameover_field(int **field) {
  int lose[20][10] = {
      {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 1, 0, 0, 0, 0, 0, 0, 0, 0},
      {0, 1, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 1, 0, 0, 0, 0, 0, 0, 0, 0},
      {0, 1, 1, 1, 0, 0, 1, 1, 1, 0}, {0, 0, 0, 0, 0, 0, 1, 0, 1, 0},
      {0, 0, 0, 0, 0, 0, 1, 0, 1, 0}, {0, 0, 0, 0, 0, 0, 1, 1, 1, 0},
      {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 1, 1, 1, 0, 0, 0, 0, 0, 0},
      {0, 1, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 1, 1, 1, 0, 0, 0, 0, 0, 0},
      {0, 0, 0, 1, 0, 0, 0, 0, 0, 0}, {0, 1, 1, 1, 0, 0, 1, 1, 1, 0},
      {0, 0, 0, 0, 0, 0, 1, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 1, 1, 1, 0},
      {0, 0, 0, 0, 0, 0, 1, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 1, 1, 1, 0},
      {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}};
  for (int i = 0; i < FIELD_ROWS; i++) {
    for (int j = 0; j < FIELD_COLS; j++) {
      field[i][j] = lose[i][j];
    }
  }
}
/**
 * @brief Freeing allocated memory for fields of extended structure
 */
void destroy_FullGameInfo() {
  FullGameInfo_t *info = get_FullGameInfo();
  free(info->GameInfo.field);
  free(info->GameInfo.next);
  free(info->field);
  free(info->current);
  free(info->next);
}

/**
 * Move figure down if it possible
 */
void moving_down() {
  FullGameInfo_t *info = get_FullGameInfo();
  int temp_y_pos = info->y_pos;
  info->y_pos++;
  if (check_collision() == 0) {
    info->state = MOVING;
  } else {
    info->y_pos = temp_y_pos;
    info->state = ATTACHING;
  }
}
/**
 * Move figure left if it possible
 */
void moving_left() {
  FullGameInfo_t *info = get_FullGameInfo();
  int temp_x_pos = info->x_pos;
  info->x_pos--;
  if (check_collision() == 1) {
    info->x_pos = temp_x_pos;
  }
}
/**
 * Move figure right if it possible
 */
void moving_right() {
  FullGameInfo_t *info = get_FullGameInfo();
  int temp_x_pos = info->x_pos;
  info->x_pos++;
  if (check_collision() == 1) {
    info->x_pos = temp_x_pos;
  }
}
/**
 * Rotate figure if it possible
 */
void rotate() {
  FullGameInfo_t *info = get_FullGameInfo();
  int **temp = create_matrix(FIGURE_SIZE, FIGURE_SIZE);
  copy_matrix(info->current, temp, FIGURE_SIZE, FIGURE_SIZE);
  if (info->current_size == 4) {
    for (int i = 0; i < FIGURE_SIZE; i++) {
      int t = info->current[1][i];
      info->current[1][i] = info->current[i][1];
      info->current[i][1] = t;
    }
  }
  if (info->current_size == 3) {
    for (int i = 0; i < 3; ++i) {
      for (int j = 1; j < 4; ++j) {
        info->current[j - 1][3 - i] = temp[i][j];
      }
    }
  }
  if (check_collision() == 1) {
    copy_matrix(temp, info->current, FIGURE_SIZE, FIGURE_SIZE);
  }
  free(temp);
}
/**
 * Attaching figure with field
 */
void attaching() {
  FullGameInfo_t *info = get_FullGameInfo();
  for (int i = 0; i < FIGURE_SIZE; i++) {
    for (int j = 0; j < FIGURE_SIZE; j++) {
      if (info->current[i][j] == 1) {
        info->field[info->y_pos + i][info->x_pos + j] = 1;
      }
    }
  }
}

/**
 * Spawn new next figure, old next figure set on current
 */
void spawn() {
  FullGameInfo_t *info = get_FullGameInfo();
  copy_matrix(info->next, info->current, FIGURE_SIZE, FIGURE_SIZE);
  info->current_size = info->next_size;
  if (info->current_size == 4) {
    info->y_pos = -1;
  } else {
    info->y_pos = 0;
  }
  info->x_pos = 3;
  generate_figure(info->next, &info->next_size);
  info->state = MOVING;
}
/**
 * randomly generate figure
 * @param figure
 * @param size
 */
void generate_figure(int **figure, int *size) {
  int figure_type = rand() % 7;
  get_figure(figure_type, figure, size);
}
/**
 * set randomly generated figure in next field
 * @param figure_type
 * @param figure
 * @param size
 */
void get_figure(int figure_type, int **figure, int *size) {
  switch (figure_type) {
    case 0:
      *size = 2;
      int O[4][4] = {{0, 1, 1, 0}, {0, 1, 1, 0}, {0}, {0}};
      set_next_figure(O, figure);
      break;
    case 1:
      *size = 4;
      int I[4][4] = {{0}, {1, 1, 1, 1}, {0}, {0}};
      set_next_figure(I, figure);
      break;
    case 2:
      *size = 3;
      int J[4][4] = {{0, 1, 0, 0}, {0, 1, 1, 1}, {0}, {0}};
      set_next_figure(J, figure);
      break;
    case 3:
      *size = 3;
      int L[4][4] = {{0, 0, 0, 1}, {0, 1, 1, 1}, {0}, {0}};
      set_next_figure(L, figure);
      break;
    case 4:
      *size = 3;
      int Z[4][4] = {{0, 1, 1, 0}, {0, 0, 1, 1}, {0}, {0}};
      set_next_figure(Z, figure);
      break;
    case 5:
      *size = 3;
      int S[4][4] = {{0, 0, 1, 1}, {0, 1, 1, 0}, {0}, {0}};
      set_next_figure(S, figure);
      break;
    case 6:
      *size = 3;
      int T[4][4] = {{0, 0, 1, 0}, {0, 1, 1, 1}, {0}, {0}};
      set_next_figure(T, figure);
      break;
    default:
  }
}
void update_timer() {
  FullGameInfo_t *info = get_FullGameInfo();
  info->timer = clock() + 11000 - 1000 * info->GameInfo.level;
}