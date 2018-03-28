#include<iostream>
#include"vec.h"
#include"4x4.h"
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
	cout << m2.I() << m2.I() * m2 ;
	Cmat<int, 2,1> cm{1,2};
	cout << cm;

	Cmat<float, 1, 3> v1{0,0,4}, v2{1,0,0};
	auto v3 = v1.cross(v2).normalize();
	cout << "angle" << v1.angle(v2) << endl;
	cout << v3 << endl;
	cout << v3.dot(v1) << endl;
	cout << v1 / v1.abs() << endl;
	cout << v1.normalize() ;
}

