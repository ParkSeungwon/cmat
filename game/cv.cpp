#include<cv.hpp>
#include<highgui.h>
#include"game.h"
using namespace std;
using namespace cv;

int from_x, from_y, to_x, to_y;
void on_mouse(int event, int x, int y, int, void*)
{
	if(event == CV_EVENT_LBUTTONDOWN) from_x = x, from_y = y;
	else if(event == CV_EVENT_LBUTTONUP) {
		to_x = x, to_y = y;

	}
}

int main()
{
	Mat img(50 * BOARD_SZ, 50 * BOARD_SZ, CV_8UC3, {0,0,0});
	imshow("game", img);
	waitKey(0);
}



