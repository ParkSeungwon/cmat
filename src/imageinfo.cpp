#include<iostream>
#include<string>
#include<highgui.h>
#include"base.h"
#include"blender-32x32.xpm"
using namespace std;
using namespace cv;

int main(int ac, char** av)
{
	if(ac < 2) {
		cout << "Usage : " << av[0] << " [image file]" << endl;
		return 0;
	}
	Mat m = imread(av[1]);
	Cmat<array<unsigned, 3>, 1200, 630> cmat{m};
	for(int i=0; i<630; i++) cmat[i][i][2] = 255;
//	Mat m2{Cmat<array<unsigned, 3>, 300, 300>()};
	Mat m2{cmat};
	for(int i=0; i<3; i++) cout << cmat[300][300][i] << ',';
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
	imshow(av[1], m2);
	//CmatXpm<"34 23 324"> cmm{xpm};
	waitKey(0);
}
	

