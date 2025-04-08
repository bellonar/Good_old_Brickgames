//
// Created by bellonar on 19.12.24.
//

#include <gtest/gtest.h>

#include "../brick_game/snake/s21_snake_model.h"

using namespace s21;

TEST(S21SnakeTest, bad_signal) {
  model *game = model::get_model();
  userInput(Up, 0);
  EXPECT_EQ(game->get_state(), START);
}

TEST(S21SnakeTest, init) {
  model *game = model::get_model();
  game->

      set_state(START);
  updateCurrentState();
  userInput(Start, 0);
  EXPECT_EQ(game->get_state(), SPAWN);
  updateCurrentState();
  EXPECT_EQ(game->get_state(), MOVING);
}

TEST(S21SnakeTest, pause) {
  model *game = model::get_model();
  userInput(Pause, 0);
  EXPECT_EQ(game->get_state(), PAUSE);
  updateCurrentState();
}

TEST(S21SnakeTest, unpause) {
  model *game = model::get_model();
  userInput(Pause, 0);
  EXPECT_EQ(game->get_state(), MOVING);
  updateCurrentState();
}

TEST(S21SnakeTest, is_faster) {
  model *game = model::get_model();
  userInput(Action, 0);
  EXPECT_EQ(game->is_faster(), true);
  updateCurrentState();
}

TEST(S21SnakeTest, change_dir1) {
  model *game = model::get_model();
  userInput(Up, 0);
  EXPECT_EQ(game->get_dir(), UP);
  updateCurrentState();
}

TEST(S21SnakeTest, change_dir2) {
  model *game = model::get_model();
  userInput(Right, 0);
  EXPECT_EQ(game->get_dir(), RIGHT);
  updateCurrentState();
}

TEST(S21SnakeTest, change_dir3) {
  model *game = model::get_model();
  userInput(Down, 0);
  EXPECT_EQ(game->get_dir(), DOWN);
  updateCurrentState();
}

TEST(S21SnakeTest, change_dir4) {
  model *game = model::get_model();
  userInput(Left, 0);
  EXPECT_EQ(game->get_dir(), LEFT);
  updateCurrentState();
}

TEST(S21SnakeTest, change_dir5) {
  model *game = model::get_model();
  userInput(Right, 0);
  EXPECT_EQ(game->get_dir(), LEFT);
  updateCurrentState();
}

TEST(S21SnakeTest, lose_start) {
  model *game = model::get_model();
  game->

      set_state(GAMEOVER);
  updateCurrentState();
  userInput(Start, 0);
  EXPECT_EQ(game->get_state(), START);
}

TEST(S21SnakeTest, win_start) {
  model *game = model::get_model();
  game->

      set_state(WIN);
  updateCurrentState();
  userInput(Start, 0);
  EXPECT_EQ(game->get_state(), START);
}

TEST(S21SnakeTest, eat_apple) {
  model *game = model::get_model();
  game->

      set_state(ATTACHING);
  game->

      set_apple(point(1, 1));
  game->

      set_tmp_head(point(1, 1));

  GameInfo_t info = updateCurrentState();

  EXPECT_EQ(game->get_state(), MOVING);
  EXPECT_EQ(info.score, 1);
  EXPECT_EQ(info.high_score, 1);
}

TEST(S21SnakeTest, Terminate) {
  model *game = model::get_model();
  userInput(Terminate, 0);
  EXPECT_EQ(game->get_state(), TERMINATE);
  updateCurrentState();
}

TEST(S21SnakeTest, Terminate_from_start) {
  model *game = model::get_model();
  game->set_state(START);
  userInput(Terminate, 0);
  EXPECT_EQ(game->get_state(), TERMINATE);
}

TEST(S21SnakeTest, Terminate_from_gameover) {
  model *game = model::get_model();
  game->

      set_state(GAMEOVER);

  userInput(Terminate, 0);
  EXPECT_EQ(game->get_state(), TERMINATE);
}

TEST(S21SnakeTest, Terminate_from_win) {
  model *game = model::get_model();
  game->

      set_state(WIN);

  userInput(Terminate, 0);
  EXPECT_EQ(game->get_state(), TERMINATE);
}

int main(int argc, char *argv[]) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}