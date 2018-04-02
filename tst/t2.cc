#include<iostream>
#include<catch.hpp>
#include"vec.h"
#include"4x4.h"
using namespace std;

TEST_CASE("vecor specialization test", "[cmat]") {
	SECTION("cross product and dot product") {
		Cmat<int, 1, 3> v1{1,2,3}, v2{2,5,8};
		auto v3 = v1 ^ v2;
		INFO("v1, v2, cross\n" << v1 << v2 << v3);
		REQUIRE((v3 , v1) == 0);
		REQUIRE((v3 , v2) == 0);
		REQUIRE(v1.normalize().abs() == 1);//wrong 
	}
	SECTION("float product and dot product") {
		Cmat<float, 1, 3> v1{1,2,3}, v2{2,5,8};
		auto v3 = v1 ^ v2;
		INFO("v1, v2, cross\n" << v1 << v2 << v3);
		REQUIRE((v3 , v1) == 0);
		REQUIRE((v3 , v2) == 0);
		REQUIRE(v1.normalize().abs() == Approx(1));
	}
}



