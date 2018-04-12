#include<vector>
#include<iostream>
#include<catch.hpp>
#include"4x4.h"
#include"vec.h"
using namespace std;

TEST_CASE("main class test", "[base]") {
	Cmat<float, 4, 2> m1{
		2,3,2,2,
		3,3,1,1
	};
	Cmat<float, 3,4> m2{
		1,2,3,
		23,2,2,
		2,2,3,
		0,0,2
	};
	Cmat<float, 4, 4> m3 = {
		1,2,3,4,
		3,4,5,1,
		4,4,7,0,
		0,0,1,0
	};
	SECTION("basic mathmatical opeartion") {
		auto m4 = m1 * m2;
	}

	SECTION("inverse") {
		auto m4 = m3 * m3.I();
		INFO("E\n" << m4);
		for(int i=0; i<4; i++) for(int j=0; j<4; j++) 
			if(i == j) REQUIRE(m4[i][j] == Approx(1));
			else REQUIRE(m4[i][j] == Approx(0));
	}
	SECTION("others") {
		REQUIRE(1 == Approx(1.00001));
		Cmat<float, 1, 3> v1{0,0,4}, v2{1,0,0};
		auto v3 = (v1 ^ v2).normalize();
		REQUIRE((v3 == Cmat<float, 1,3>{0,1,0}));
		CHECK(0 == (v3 , v1));
		REQUIRE(v1 /v1.abs() == v1.normalize());
		for(int i=0; i<10; i++) REQUIRE(i == i);
	}
	SECTION("column first continuous memory align") {
		auto* p = m2.arr_.data()->data();
		for(int i=0; i<3; i++) for(int j=0; j<4; j++) REQUIRE(m2[i][j] == *p++);
	}
}
