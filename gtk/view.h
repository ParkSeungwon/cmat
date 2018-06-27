#include<gtkmm.h>
#include"game/game.h"
using namespace std;

class ColorButton : public Gtk::Button
{
public:
	ColorButton(const Gdk::RGBA color) {
		override_background_color(color);
	}
};

class Win : public Gtk::Window
{
public:
	Win();
	Win& operator=(const Board& board);
protected:
	Gtk::Button bt_[BOARD_SZ][BOARD_SZ];
	Gtk::HBox hb_[BOARD_SZ];
	Gtk::VBox vb_;
	void on_click(int x, int y);
	bool on_key(GdkEventKey* e);
	int x, y;
};

