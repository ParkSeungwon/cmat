#include"projectile.h"
using namespace std;

Projectile::Projectile(vec4 pos, vec4 v)
{
	pos_ = pos;
	v_ = v;
}

mat4 Projectile::time_pass(float dt)
{
	mat4 m;
	pos_ = pos_ + dt * v_;
	return m.gltranslate(pos_[0][0], pos_[0][1], pos_[0][2]);
}

bool Projectile::out_of_bound()
{
	if(abs(pos_[0][2]) > 15) return true;
	return false;
}


