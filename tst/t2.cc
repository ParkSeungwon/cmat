#include<catch.hpp>
#include"4x4.h"

TEST_CASE("compile time matrix", "[cmat]") {
	Cmat<float, 3, 3> cm;
	cm.O();
	CAPTURE(cm);
	REQUIRE(cm[0][0] == 0);
	cm[0][0] = 3;
	REQUIRE(cm[0][0] == 3);

	SECTION("cm") {
		REQUIRE(cm[0][0] == 3);
	}
	SECTION("cm2") {
		REQUIRE(cm[0][0] == 3);
	}

}



