#include<vector>
#include<iostream>
#include<catch.hpp>
#include"4x4.h"
#include"vec.h"
using namespace std;

TEST_CASE("main class test", "[base]") {
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
	}, m;
	SECTION("inverse") {
		auto m3 = m2 * m2.I();
		INFO("E\n" << m3);
		for(int i=0; i<4; i++) for(int j=0; j<4; j++) 
			if(i == j) REQUIRE(m3[i][j] == Approx(1));
			else REQUIRE(m[i][j] == Approx(0));
	}
	SECTION("others") {
		REQUIRE(1 == Approx(1.00001));
		Cmat<int, 2,1> cm{1,2};
		Cmat<float, 1, 3> v1{0,0,4}, v2{1,0,0};
		auto v3 = (v1 ^ v2).normalize();
		REQUIRE((v3 == Cmat<float, 1,3>{0,1,0}));
		CHECK(0 == (v3 , v1));
		REQUIRE(v1 /v1.abs() == v1.normalize());
		for(int i=0; i<10; i++) REQUIRE(i == i);
	}
	SECTION("column first continuous memory align") {
		auto* p = b.arr_.data()->data();
		for(int i=0; i<3; i++) for(int j=0; j<4; j++) REQUIRE(b[i][j] == *p++);
	}
}
