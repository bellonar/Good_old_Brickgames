// #include "s21_desktop_gui.h"
#include "s21_desktop_gui.h"

int main(int argc, char* argv[]) {
  auto app = Gtk::Application::create("org.gtkmm.examples.base");

  return app->make_window_and_run<BrickGame_Interface>(argc, argv);
}