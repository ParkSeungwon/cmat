#include"square.h"
#include<array>
#include<fstream>
using namespace std;

array<char, 60000> read_train_label() {
	ifstream f("train-label.dat");
	char c;
	for(int i=0; i<8; i++) f >> noskipws >> c;
	array<char, 60000> r;
	for(int i=0; i<60000; i++) f >> noskipws >> r[i];
	return r;
}

array<Cmat<unsigned char, 28, 28>, 60000> read_train_image() {
	ifstream f("train-image.dat");
	unsigned char c;
	for(int i=0; i<16; i++) f >> noskipws >> c;
	array<Cmat<unsigned char, 28, 28>, 60000> r;
	for(int k=0; k<60000; k++) for(int i=0; i<28; i++) for(int j=0; j<28; j++) 
		f >> noskipws >> r[k][j][i];
	return r;
}
