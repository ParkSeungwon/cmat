#include<iostream>
#include<string>
#include<highgui.h>
using namespace std;
using namespace cv;

int main(int ac, char** av)
{
	if(ac < 2) {
		cout << "Usage : " << av[0] << " [image file]" << endl;
		return 0;
	}
	Mat m = imread(av[1]);
	cout << m.cols << 'x' << m.rows << endl << "channel : " << m.channels() << endl;
	string s;
	switch(m.depth()) {
		case CV_8U: s = "8U"; break;
		case CV_8S: s = "8S"; break;
		case CV_16U: s = "16U"; break;
		case CV_16S: s = "16S"; break;
		case CV_32S: s = "32S"; break;
		case CV_32F: s = "32F"; break;
		case CV_64F: s = "64F"; break;
	}
	cout << "depth : " << s << endl;
	imshow(av[1], m);
	waitKey(0);
}
	

