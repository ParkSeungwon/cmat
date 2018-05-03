#include<sstream>
#include<map>
#include<tuple>
#include"matrix.h"
#include"base.h"
#include"blender-32x32.xpm"
using namespace std;

array<unsigned char, 4> change(const char* p)
{///change color code 7B543E -> uchar 232,12,122
	unsigned char c[6];
	if(*p == '#') {
		for(int i=0; i<6; i++) {
			if(p[i+1] <= '9') c[i] = p[i+1] - '0';
			else c[i] = p[i+1] - 'A' + 10;
		}
		for(int i=0; i<6; i+=2) c[i] = c[i] * 16 + c[i+1];
		return {c[0], c[2], c[4], 0xff};
	} else return {0,0,0,0};
}

auto read_xpm() {
	CmatBase<array<unsigned char, 3>, ct_stoi(xpm[0]), ct_stoi2(xpm[0])> r;
	map<string, array<unsigned char, 4>> color_map;
	stringstream ss;
	ss << xpm[0];
	int ar[4];
	for(int i=0; i<4; i++) ss >> ar[4];
	for(int i=0; i<ar[2]; i++) 
		color_map[string(xpm[i+1], ar[3])] = change(xpm[i+1] + ar[3] + 3);

	for(int y=0; y<ar[1]; y++) for(int x=0; x<ar[0]; x++) for(int k=0; k<4; k++)
		r[x][y][k] = color_map[string(xpm[ar[3]+1+y] + x*ar[4], ar[3])][k];
	return r;
}



