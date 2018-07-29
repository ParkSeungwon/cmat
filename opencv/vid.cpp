#include<cxcore.hpp>
#include<highgui.h>
#include<iostream>
#include"cvmatrix.h"
using namespace std;
using namespace cv;

int main(int ac, char** av)
{
	VideoCapture vid(0);
//	VideoWriter wr;
//	int w = vid.get(CV_CAP_PROP_FRAME_WIDTH);
//	int h = vid.get(CV_CAP_PROP_FRAME_HEIGHT);
//	wr.open("wr.avi", CV_FOURCC('D','I','V','4'), 30, Size{w,h});
	CVMat frame{Matrix<unsigned char>{4,4}};
	for(int k=0; waitKey(10) == -1; k++) {//wait 10ms, no input during 10ms,then return -1
		vid >> frame;
		frame.noise(k++);
		frame.show("video");
	}
}


