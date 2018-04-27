#include<catch.hpp>
#include<chrono>
#include<array>
#include"vec.h"
#include"4x4.h"
#include"plane.h"
#include"mtl.h"
using namespace std::chrono;
using namespace std;
using mat4 = Cmat<float, 4, 4>;
using vec4 = Cmat<float, 1, 4>;

//class TPlane : public Plane
//{
//public:
//	TPlane(vec3 x, vec3 y, vec3 z) : Plane(x,y,z) {}
//	float xth() {return xth_;}
//	float yth() {return yth_;}
//	vec3 normal() {return normal_;}
//};

TEST_CASE("plane test", "[plane]") {
//	TPlane p{{0,0,0},{1,0,0},{0,-1,2.3}};
//	REQUIRE(vec3{1,0,0}.angle({1,1,0}) == Approx(M_PI/4));//inter angle
//	REQUIRE(vec3{1,0,0}.angle({1,-1,0}) == Approx(M_PI/4));//same inter angle
//	REQUIRE((vec3{1,0,0} ^ vec3{0,1,0}) == vec3{0,0,1});//left turn upward
//	REQUIRE((vec3{1,0,0} ^ vec3{0,-1,0}) == vec3{0,0,-1});
//	mat4 rx, ry;
//	rx.glrotateX(p.xth());
//	ry.glrotateY(p.yth());
//	auto n = rx * ry * vec4{0,0,1,1};

//	WARN("normal" << n << p.normal());
	Cmat<float, 2, 2> c, d;
	auto e = c - d;
	WARN(e-c);
}

TEST_CASE("array init") {
	array<int ,3> ar;
	for(int i=0; i<3; i++) REQUIRE(ar[i] == 0);
	int k = 3;
	REQUIRE(k / 1.5f == 2);
}

TEST_CASE("read_mtl") {
	auto sNm = read_mtl("Handgun_obj.mtl");
}
