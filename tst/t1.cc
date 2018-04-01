#include<catch.hpp>
#include"4x4.h"

TEST_CASE("base class test", "[base]") {
	Cmat<float, 2, 2> cm{3,4,5,6};
	CAPTURE(cm);
	REQUIRE(cm[0][0] == 3);
}




