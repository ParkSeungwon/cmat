#include<iostream>
#include"cmat.h"
using namespace std;

int main()
{
	Cmat<float, 2, 2> m1{
		2,3,
		3,3
	}, m5;
	Cmat<float, 3,4> b{
		1,2,3,
		23,2,2,
		2,2,3,
		0,0,2
	};
//	Cmat<float, 3,3> c;
	Cmat<float, 4, 4> m2 = {
		1,2,3,4,
		3,4,5,1,
		4,4,7,0,
		0,0,1,0
	};
	cout << m2.I() * m2 ;
}

