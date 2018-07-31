#include<catch.hpp>
#include<iostream>
#include"matrix/cmatstream.h"
#include"matrix/neural.h"
using namespace std;

TEST_CASE("cmatstream") {
	Cmat<float, 3, 3> m;
	CmatStream cs{m, 5};
	for(int i=0; i<5; i++) cout << cs << endl;
}
TEST_CASE("cmatstream2") {
	NeuralNet<3, 4, 2> net;
	NeuralNetStream n{net};
	n.print();
}
