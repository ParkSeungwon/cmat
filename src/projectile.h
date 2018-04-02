#include"vec.h"
#include"4x4.h"
using vec4 = Cmat<float, 1, 4>;
using mat4 = Cmat<float, 4, 4>;

struct Projectile
{
	Projectile(vec4 pos, vec4 v);
	mat4 time_pass(float dt = 0.1);
	bool out_of_bound();
	vec4 v_, pos_;
};

