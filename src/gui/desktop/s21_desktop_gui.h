
#ifndef CPP3_BRICKGAME_V2_0_1_S21_DESKTOP_GUI_H
#define CPP3_BRICKGAME_V2_0_1_S21_DESKTOP_GUI_H

#include "../../brick_game/s21_api.h"
#include <gtkmm.h>
#include <cairomm/context.h>
#include <iostream>
#include <string>


#define WIDTH 40
#define HEIGHT 40
#define PINK 1
#define GREEN 2



class my_area : public Gtk::DrawingArea{
public:
      my_area() {
          Glib::signal_timeout().connect( sigc::mem_fun(*this, &my_area::on_timeout), 125 );
        set_draw_func(sigc::mem_fun(*this, &my_area::on_draw));
         empty = true;
      };
        virtual ~my_area(){};
        void update_info(const GameInfo_t& info) {
        this->game_info = info;
        empty = false;
    }
  bool  on_timeout()
    {
        update_info(updateCurrentState());
        queue_draw();
        return true;
    }
    void draw_pixel(const Cairo::RefPtr<Cairo::Context>& cr,int width,int height,int i,int j,int color);
   void show_stats(const Cairo::RefPtr<Cairo::Context>& cr,int height,int width);

protected:
    void on_draw(const Cairo::RefPtr<Cairo::Context>& cr, int width, int height) ;
    GameInfo_t game_info;
    bool empty;

};


class BrickGame_Interface : public Gtk::Window {
public:
    BrickGame_Interface() : area(){
        set_title("BrickGames  v2.0  by bellonar");
        set_default_size(400, 600);

        set_child(area);
        auto controller = Gtk::EventControllerKey::create();
        controller->signal_key_pressed().connect(
                sigc::mem_fun(*this, &BrickGame_Interface::on_window_key_pressed), false);
        add_controller(controller);

    }
    virtual ~BrickGame_Interface(){};
protected:
    bool on_window_key_pressed(guint keyval, guint, Gdk::ModifierType state);
private:
    UserAction_t currentAction;
    GameInfo_t info;
    my_area area;
    static BrickGame_Interface* interface_ptr;

};



#endif //CPP3_BRICKGAME_V2_0_1_S21_DESKTOP_GUI_H
