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
	Cmat<float, 6, 6> m2 = {
		1,2,3,4,2,1,
		3,4,5,1,4,2,
		4,4,7,0,1,2,
		1,0,1,0,3,2,
		0,0,1,2,3,2,
		0,7,1,0,3,2
	};
	cout << m2.I() << m2.I() * m2 ;
	Cmat<int, 2,1> cm{1,2};
	cout << cm;


	Cmat<float, 1, 3> v1{2,1,3}, v2{3,2,2};
	auto v3 = v1.cross(v2);
	cout << v1.dot(v3) << ' ' << v2.dot(v3) << endl;
}

