#include<gtkmm.h>
#include"game/game.h"
#include"view.h"
using namespace std;

int main(int ac, char** av)
{
	Board board;
	auto app = Gtk::Application::create(ac, av);
	Win win;
	return app->run(win);
}
	

