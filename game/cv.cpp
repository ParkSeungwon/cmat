#include<cv.hpp>
#include<highgui.h>
#include"game.h"
using namespace std;
using namespace cv;

Scalar clr[6] = {
	{0,0,255},{0,255,0}, {255,0,0}, {0, 255, 255}, {0,0,0}, {255,255,255}};
class CVBoard : public Mat
{
public:
	CVBoard() : Mat(50 * BOARD_SZ, 50 * BOARD_SZ, CV_8UC3, {255,255,255}) {}
	CVBoard& operator=(const Board& b) {
		for(int i=0; i<BOARD_SZ; i++) for(int j=0; j<BOARD_SZ; j++) {
			int k = b.get_color(i, j);
			rectangle(*this, {i*50, j*50}, {i*50+50, j*50+50}, clr[k % 6], CV_FILLED);
			if(k > 5) rectangle(*this, {i*50+5, j*50+5}, {i*50+45, j*50+45}, clr[5]);
			if(k > 11) rectangle(*this,{i*50+10, j*50+10}, {i*50+40, j*50+40}, clr[5]);
		}
		imshow("game", *this);
		cout << b << endl;
		waitKey(200);
	}
};
int from_x, from_y;
void on_mouse(int event, int x, int y, int, void*)
{
	if(event == CV_EVENT_LBUTTONDOWN) {
		from_x = x / 50, from_y = y / 50;
		cout << "selected " << from_x << " and " << from_y << endl;
	}
}

int main()
{
	Board board;
	CVBoard b;
	b = board;
	setMouseCallback("game", on_mouse, 0);
	for(char c; c != 'q';) {
		while(board.find_match()) {
			board.transform();
			b = board;
			while(board.step_drop()) b = board;
			board.turn_finish();
		}
		cout << "score : " << board.score << endl;
		board.score = 0;
		do {
			c = waitKey(0);
			if(c == 'q') break;
		} while(!board.swap(from_x, from_y, c));
		b = board;
	}
}



