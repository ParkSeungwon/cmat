#include<catch.hpp>
#include"4x4.h"

TEST_CASE("base class test", "[base]") {
	Cmat<float, 2, 2> cm{3,4,5,6};
	CAPTURE(cm);
	REQUIRE(cm[0][0] == 3);
}

TEST_CASE("class protected member access", "[other]") {
	struct A {
		int x_;
	};
	struct B : public A {
		void x() {return x_;}
	};

	B b;
	b.x();
}



