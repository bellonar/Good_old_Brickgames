//
// Created by bellonar on 25.07.24.
//
#include "s21_snake_model.h"

namespace s21 {
/*!
 * Function of switching states of the fsm depending on the received signal
 * @param action translated signal from user
 */
void model::update(UserAction_t action) {
  switch (this->state) {
    case START:
      if (action == Start) {
        this->init();
        this->state = SPAWN;
      }
      if (action == Terminate) {
        this->state = TERMINATE;
        this->delete_field();
      }
      break;
    case MOVING:
      if (action == Terminate) {
        this->state = TERMINATE;
        this->delete_field();
      } else if (action == Action) {
        this->faster = true;
      } else if (action == Pause) {
        this->info.pause = 1;
        this->state = PAUSE;
      } else {
        this->change_dir(action);
      }
      break;
    case PAUSE:
      if (action == Terminate) {
        this->state = TERMINATE;
        this->delete_field();
      } else if (action == Pause) {
        this->info.pause = 0;
        this->state = MOVING;
      }
      break;
    case GAMEOVER:
    case WIN:
      if (action == Terminate) {
        this->state = TERMINATE;
        this->delete_field();
      }
      if (action == Start) {
        this->state = START;
      }
      break;
    default:
      break;
  }
}

/*!
 * State switching function of a fsm occurring unconditionally
 */
void model::update() {
  if (this->info.pause != 1) {
    if ((faster) || (this->state == MOVING && clock() >= this->timer)) {
      this->state = SHIFTING;
      this->faster = false;
      this->update_timer();
    }
    if (this->state == SHIFTING) {
      this->move();
    }
    if (this->state == ATTACHING) {
      if (this->apple.is_equal(this->tmp_head)) {
        this->model_snake.push_new_head(tmp_head);
        this->update_score();
        if (this->state != WIN) {
          this->state = SPAWN;
        }
      } else {
        this->state = GAMEOVER;
      }
    }
    if (this->state == SPAWN) {
      this->spawn_apple();
      this->state = MOVING;
    }
    if (this->state != TERMINATE) {
      this->update_field();
    }
  }
}

/*!
 * Changing the direction of the snake movement based on the received signal
 * @param action
 */
void model::change_dir(UserAction_t action) {
  if (dir_ == LEFT || dir_ == RIGHT) {
    if (action == Up) {
      dir_ = UP;
    }
    if (action == Down) {
      dir_ = DOWN;
    }
  } else {
    if (action == Left) {
      dir_ = LEFT;
    }
    if (action == Right) {
      dir_ = RIGHT;
    }
  }
}

//    model* model:: model_ptr = nullptr;

/*!
 * Calculate the hypathetical head, check for collision. If there is no
 * collision, glue on a new head and cut off the tail.
 */
void model::move() {
  point head = this->model_snake.get_head();
  if (dir_ == LEFT) {
    this->tmp_head.set(head.y(), head.x() - 1);
  }
  if (dir_ == RIGHT) {
    this->tmp_head.set(head.y(), head.x() + 1);
  }
  if (dir_ == UP) {
    this->tmp_head.set(head.y() - 1, head.x());
  }
  if (dir_ == DOWN) {
    this->tmp_head.set(head.y() + 1, head.x());
  }

  if (out_of_border(this->tmp_head) || snake_collision(this->tmp_head) ||
      this->apple.is_equal(this->tmp_head)) {
    this->state = ATTACHING;
  } else {
    this->model_snake.push_new_head(tmp_head);
    this->model_snake.pop_tail();
    this->state = MOVING;
  }
}
/*!
 * Initialization of start parameters
 */
void model::init() {
  this->model_snake.clear();
  this->model_snake.spawn_default_snake();
  this->info.level = 1;
  this->info.speed = 1;
  this->info.score = 0;
  this->load_high_score();
  this->dir_ = LEFT;
  this->timer = clock() + 11000 - 2000 * this->info.level;
}
/*!
 *Initialization of the default snake
 */
void snake::spawn_default_snake() {
  this->chain_.push_back(point(4, 4));
  this->chain_.push_back(point(4, 5));
  this->chain_.push_back(point(4, 6));
  this->chain_.push_back(point(4, 7));
}
/*!
 * Check if there is a point in the snake
 * @param p
 * @return true or false
 */
bool snake::in_chain(point p) {
  bool res = false;
  point tmp;
  auto iter = chain_.begin();
  while (!res && iter != chain_.end()) {
    tmp = *iter;
    res = tmp.is_equal(p);
    iter++;
  }
  return res;
}
/*!
 * Generate random apple
 */
void model::spawn_apple() {
  point tmp;
  do {
    tmp.set((std::rand() % 20), (std::rand() % 10));
  } while (this->model_snake.in_chain(tmp));
  this->apple = tmp;
}
/*!
 * Checking the exit of the snake outside the boundary
 * @param p
 * @return
 */
bool model::out_of_border(point p) {
  return (p.x() == LEFT_BORDER || p.y() == TOP_BORDER ||
          p.x() == RIGHT_BORDER || p.y() == LOW_BORDER);
}
/*!
 * Checking the collision of the snake with itself
 * @param p
 * @return
 */
bool model::snake_collision(point p) { return (this->model_snake.in_chain(p)); }
/*!
 * Filling the field depending on the state
 */
void model::update_field() {
  this->refresh_model();
  if (this->state == START) {
    this->start();
  } else if (this->state == GAMEOVER) {
    this->lose();
  } else if (this->state == WIN) {
    this->win();
  } else {
    this->info.field[this->apple.y()][this->apple.x()] = 2;
    if (this->model_snake.is_empty()) {
      this->model_snake.set_head_and_tail();
      point elem = this->model_snake.get_elem();

      point tail = this->model_snake.get_tail();
      while (!elem.is_equal(tail)) {
        this->info.field[elem.y()][elem.x()] = 1;
        this->model_snake.next_elem();
        elem = this->model_snake.get_elem();
      };
      this->info.field[elem.y()][elem.x()] = 1;
    }
  }
};
/*!
 * Allocating memory for the matrix
 * @param rows
 * @param cols
 * @return
 */
int** model::create_matrix(int rows, int cols) {
  int** matrix = new int*[rows];
  for (int i = 0; i < rows; ++i) {
    matrix[i] = new int[cols];
  }
  return matrix;
}

/*!
 * Clearing the field
 */
void model::refresh_model() {
  for (int i = 0; i < 20; i++) {
    for (int j = 0; j < 10; j++) {
      this->info.field[i][j] = 0;
    }
  }
};
/*!
 * Lose banner
 */
void model::lose() {
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
  for (int i = 0; i < 20; i++) {
    for (int j = 0; j < 10; j++) {
      this->info.field[i][j] = lose[i][j];
    }
  }
};
/*!
 * Start banner
 */
void model::start() {
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
  for (int i = 0; i < 20; i++) {
    for (int j = 0; j < 10; j++) {
      this->info.field[i][j] = start[i][j];
    }
  }
}
/*!
 * Win banner
 */
void model::win() {
  int win[20][10] = {
      {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
      {0, 0, 1, 0, 0, 0, 1, 0, 0, 0}, {0, 0, 1, 0, 1, 0, 1, 0, 0, 0},
      {0, 0, 1, 0, 1, 0, 1, 0, 0, 0}, {0, 0, 0, 1, 0, 1, 0, 0, 0, 0},
      {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 1, 1, 1, 0, 0, 0, 0},
      {0, 0, 0, 0, 1, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 1, 0, 0, 0, 0, 0},
      {0, 0, 0, 0, 1, 0, 0, 0, 0, 0}, {0, 0, 0, 1, 1, 1, 0, 0, 0, 0},
      {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 1, 0, 0, 1, 0, 0, 0, 0},
      {0, 0, 1, 1, 0, 1, 0, 0, 0, 0}, {0, 0, 1, 0, 1, 1, 0, 0, 0, 0},
      {0, 0, 1, 0, 0, 1, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
      {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
  };
  for (int i = 0; i < 20; i++) {
    for (int j = 0; j < 10; j++) {
      this->info.field[i][j] = win[i][j];
    }
  }
}
/*!
 * Updating  score
 */
void model::update_score() {
  this->info.score++;
  if (this->info.high_score < this->info.score) {
    this->info.high_score = this->info.score;
    this->update_high_score();
  }
  if (this->info.score % 5 == 0 && this->info.level < 10) {
    this->info.level++;
    this->info.speed = this->info.level;
  }
  if (this->info.score == 196) {
    this->state = WIN;
  }
};
/*!
 * Rewrite hs file
 */
void model::update_high_score() {
  std::ofstream out("snake_hs.txt");
  if (out.is_open()) {
    out << this->info.high_score;
  }
  out.close();
};
/*!
 * Read hs file
 */
void model::load_high_score() {
  std::ifstream in("snake_hs.txt");
  if (in.is_open()) {
    in >> this->info.high_score;
  }
  in.close();
};

}  // namespace s21
