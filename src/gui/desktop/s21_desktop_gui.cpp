#include "s21_desktop_gui.h"

void my_area::on_draw(const Cairo::RefPtr<Cairo::Context>& cr, int width,
                      int height) {
  double radius = height / 60;
  double degrees = M_PI / 180.0;

  cr->rectangle(0, 0, width, height);
  cr->set_source_rgb(0, 250, 154);  // 139,0,139
  cr->fill();

  cr->begin_new_sub_path();
  cr->arc(width * 1 / 2 - radius, radius, radius, -90 * degrees, 0 * degrees);
  cr->arc(width * 1 / 2 - radius, height - radius, radius, 0 * degrees,
          90 * degrees);
  cr->arc(radius, height - radius, radius, 90 * degrees, 180 * degrees);
  cr->arc(radius, radius, radius, 180 * degrees, 270 * degrees);
  cr->close_path();

  cr->set_source_rgb(0, 0, 0);
  cr->fill_preserve();
  cr->set_source_rgba(1, 0.2, 1, 0);
  cr->set_line_width(10.0);
  cr->stroke();
  if (!empty) {
    for (int i = 0; i < 20; i++) {
      for (int j = 0; j < 10; j++) {
        if (game_info.field[i][j] != 0) {
          draw_pixel(cr, width, height, i, j, game_info.field[i][j]);  // 1,0,1
        }
      }
    }
    show_stats(cr, height, width);
  }
};

void my_area::draw_pixel(const Cairo::RefPtr<Cairo::Context>& cr, int width,
                         int height, int i, int j, int color) {
  double x = j * width / 20, y = i * height / 20;

  double radius = height / 60;
  double degrees = M_PI / 180.0;

  cr->begin_new_sub_path();
  cr->arc(x + width / 20 - radius, y + radius, radius, -90 * degrees,
          0 * degrees);
  cr->arc(x + width / 20 - radius, y + height / 20 - radius, radius,
          0 * degrees, 90 * degrees);
  cr->arc(x + radius, y + height / 20 - radius, radius, 90 * degrees,
          180 * degrees);
  cr->arc(x + radius, y + radius, radius, 180 * degrees, 270 * degrees);
  cr->close_path();
  int r = 1, g = 0, b = 1;
  if (color == GREEN) {
    r = 0;
    g = 255;
    b = 0;
  }
  cr->set_source_rgb(r, g, b);
  cr->fill_preserve();
  cr->set_source_rgba(0.5, 0, 0, 0.5);
  cr->set_line_width(5.0);
  cr->stroke();
}

void my_area::show_stats(const Cairo::RefPtr<Cairo::Context>& cr, int height,
                         int width) {
  cr->set_source_rgb(0.0, 0.0, 0.0);
  cr->set_font_size((height / 20 + width / 20) / 2);
  int start_width = width / 20 * 11;
  int start_height = height / 20 * 2;
  cr->move_to(start_width, start_height);
  cr->show_text("Score:" + std::to_string(game_info.score));
  cr->move_to(start_width, start_height * 2);
  cr->show_text("H_Score:" + std::to_string(game_info.high_score));
  cr->move_to(start_width, start_height * 3);
  cr->show_text("Level:" + std::to_string(game_info.level));
  cr->move_to(start_width, start_height * 4);
  cr->show_text("Speed:" + std::to_string(game_info.speed));

  if (game_info.next != NULL) {
    cr->move_to(start_width, start_height * 5);  // устанавливаем позицию текста
    cr->show_text("Next:");
    for (int i = 0; i < 4; i++) {
      for (int j = 0; j < 4; j++) {
        if (game_info.next[i][j] != 0) {
          draw_pixel(cr, width, height, 11 + i, 11 + j, PINK);
        }
      }
    }
  }
  if (game_info.pause) {
    cr->move_to(start_width, start_height * 8);  // устанавливаем позицию текста
    cr->show_text("PAUSE");
  }
}

bool BrickGame_Interface::on_window_key_pressed(guint keyval, guint,
                                                Gdk::ModifierType state) {
  if (!(bool)state) {
  };
  bool status = true;
  switch (keyval) {
    case GDK_KEY_Left:
      currentAction = Left;
      break;
    case GDK_KEY_Right:
      currentAction = Right;
      break;
    case GDK_KEY_Down:
      currentAction = Down;
      break;
    case GDK_KEY_Up:
      currentAction = Up;
      break;
    case GDK_KEY_p:
      currentAction = Pause;
      break;
    case GDK_KEY_Return:
      currentAction = Start;
      break;
    case GDK_KEY_space:
      currentAction = Action;
      break;
    case GDK_KEY_Escape:
      currentAction = Terminate;
      break;
    default:
      status = false;
  }
  if (status) {
    userInput(currentAction, 0);
  }
  if (currentAction == Terminate) {
    close();
  }
  return true;
}
