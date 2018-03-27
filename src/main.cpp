#include<iostream>
#include"1xN.h"
#include"4x4.h"
using namespace std;

int main()
{
	Cmat<float, 3, 2> m1{
		2,3,2,
		3,3,3
	}, m5;
//	Cmat<float, 2,2> b;
//	Cmat<float, 3,3> c;
	Cmat<float, 4, 4> m2 = {
		1,2,3,4,
		23,2,2,2,
		2,2,3,2,
		0,0,2,1
	};
	Cmat<float, 1, 3> mm{1,2,2}, mm2{2,2,2};
	cout << mm.dot(mm2) << endl;
	cout << m2 * mm.add_one();
	cout << mm.theta(mm2) << endl;
	cout << 4 * m2;
	cout << m2.I();
}

