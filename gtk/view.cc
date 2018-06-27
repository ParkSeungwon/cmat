#include<functional>
#include"view.h"
using namespace std;

Win::Win()
{
	add(vb_);
	for(int i=0; i<BOARD_SZ; i++) {
		vb_.add(hb_[i]);
		for(int j=0; j<BOARD_SZ; j++) {
			hb_[i].add(bt_[j][i]);
			bt_[j][i].signal_clicked().connect(bind(&Win::on_click, this, j, i));
			bt_[j][i].signal_key_press_event().connect(sigc::mem_fun(*this, &Win::on_key));
		}
	}
	show_all_children();
}

void Win::on_click(int x, int y)
{
	this->x = x; this->y = y;
}
		
bool Win::on_key(GdkEventKey* e)
{
	switch(e->keyval) {
		case GDK_KEY_uparrow: break;
		case GDK_KEY_downarrow: break;
		case GDK_KEY_rightarrow: break;
		case GDK_KEY_leftarrow: break;
		default: break;
	}
	return false;
}

Win& Win::operator=(const Board& b) {
}
