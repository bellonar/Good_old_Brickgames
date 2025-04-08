#ifndef CPP3_BRICKGAME_V2_0_1_BACKEND_H
#define CPP3_BRICKGAME_V2_0_1_BACKEND_H

#include <cstdlib>
#include <ctime>
#include <fstream>
#include <iostream>
#include <vector>

#include "../s21_api.h"

#define TOP_BORDER -1
#define LOW_BORDER 20
#define LEFT_BORDER -1
#define RIGHT_BORDER 10

namespace s21 {
enum State_t {
  START,
  SPAWN,
  PAUSE,
  MOVING,
  SHIFTING,
  ATTACHING,
  GAMEOVER,
  TERMINATE,
  WIN
};

enum direction { UP, DOWN, LEFT, RIGHT };

class point {
 private:
  int x_;
  int y_;

 public:
  point() : x_(0), y_(0){};
  int x() { return x_; };
  int y() { return y_; };
  point(int y, int x) : x_(x), y_(y){};
  void set(int y, int x) {
    x_ = x;
    y_ = y;
  };
  bool is_equal(point p) { return ((p.x() == x_) && (p.y() == y_)); }
};

class snake {
 private:
  std::vector<point> chain_;
  std::vector<point>::iterator head;

 public:
  snake() { head = chain_.begin(); }
  void clear() {
    this->chain_.clear();
    head = chain_.begin();
  }
  void spawn_default_snake();
  void push_new_head(point head) { chain_.insert(chain_.begin(), head); };
  void pop_tail() { chain_.pop_back(); };
  bool in_chain(point p);
  void set_head_and_tail() { this->head = chain_.begin(); };
  bool is_empty() { return !chain_.empty(); }
  point get_elem() { return *(head); };
  point get_head() { return *(chain_.begin()); };
  point get_tail() { return chain_.back(); };
  void next_elem() { ++this->head; }
};
class model {
 private:
  int** create_matrix(int rows, int cols);
  model() {
    std::srand(std::time(NULL));
    this->info.field = create_matrix(20, 10);
    this->info.next = NULL;
    timer = 0;
  };

  point tmp_head;
  point apple;
  snake model_snake;
  direction dir_ = LEFT;
  State_t state = START;
  GameInfo_t info;
  clock_t timer;
  bool faster = false;
  void delete_field() {
    if (this->info.field != nullptr) {
      for (int i = 0; i < 20; i++) {
        delete[] this->info.field[i];
      }
      delete[] this->info.field;
    }
    this->info.field = nullptr;
  };

 public:
  ~model(){};
  GameInfo_t get_info() { return this->info; }
  State_t get_state() { return state; }
  void init();
  void refresh_model();
  void start();
  void lose();
  void win();
  void spawn_apple();
  static model* get_model() {
    static model model_ptr{};
    return &model_ptr;
  };
  void update(UserAction_t action);
  void update();
  void update_field();
  void update_score();
  void update_high_score();
  void load_high_score();
  void change_dir(UserAction_t action);
  void move();
  void update_timer() {
    this->timer = clock() + 11000 - 1000 * this->info.level;
  };
  bool out_of_border(point p);
  bool snake_collision(point p);
  // getters and setters for tests
  bool is_faster() { return faster; }
  direction get_dir() { return dir_; }
  void set_state(State_t new_state) { this->state = new_state; }
  void set_apple(point p) { this->apple = p; }
  void set_tmp_head(point p) { this->tmp_head = p; }
};

};  // namespace s21

#endif  // CPP3_BRICKGAME_V2_0_1_BACKEND_H
