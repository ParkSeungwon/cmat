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
	Cmat<int, 4, 4> m2 = {
		1,2,3,4,
		3,4,5,1,
		4,4,7,0,
		0,0,1,0
	};
//	for(int i=0; i<100; i++) cout << m2 * m2.I();
	vector<Cmat<int, 4,4>> v;
//	for(int i=0; i<100; i++) v.emplace_back(m2.I());
//	for(int i=0; i<99; i++) if(v[i] != v[i+1]) cout << v[i] << v[i+1];
	v.clear();
	for(int i=0; i<100; i++) v.emplace_back(m2 * m2);
	for(int i=0; i<99; i++) if(v[i] != v[i+1]) cout << v[i] << v[i+1] << endl;
//	cout << m2.I() << m2.I() * m2 ;
//	Cmat<int, 2,1> cm{1,2};
//	cout << cm;
//
//	Cmat<float, 1, 3> v1{0,0,4}, v2{1,0,0};
//	auto v3 = (v1 ^ v2).normalize();
//	cout << "angle" << v1.angle(v2) << endl;
//	cout << v3 << endl;
//	cout << (v3 , v1) << endl;//v3 : v1
//	cout << v1;
//	cout << (v1 /= v1.abs()) << endl;
//	cout << v1.normalize() ;
//	cout << v1;
}

