#include"4x4.h"
#include"vec.h"
#include"plane.h"
using namespace std;
using mat4 = Cmat<float, 4, 4>;
using vec4 = Cmat<float, 1, 4>;
using vec3 = Cmat<float, 1, 3>;

Plane::Plane(vec3 ul, vec3 ur, vec3 ll)
{//set a matrix that makes vec3{0,0,1} to a normal of plane defined by 3 points
	ul_ = ul; ur_ = ur; ll_ = ll;
	vec4 st_norm{0,0,1,1}, st_ul{-1,-1,0,1};//standard plane vectors
	mat4 m;
	vec3 w = ur - ul;
	vec3 h = ll - ul;
	normal_ = h ^ w; //normal_ of the plane
	vec3 yz = normal_; yz[0][0] = 0;//projection to yz plane
	vec3 xz = normal_; xz[0][1] = 0;
	float xth = yz.angle({0,0,1});//angles to rotate
	float yth = xz.angle({0,0,1});//xth_ * yth_ * 001 -> parallel this plane

	//inter angle doesn't care +-, so test with this and choose
	vec4 clock_x = m.glrotateX(xth) * st_norm - normal_.add_tail();
	vec4 cc_x = m.glrotateX(-xth) * st_norm - normal_.add_tail();
	vec4 clock_y = m.glrotateY(yth) * st_norm - normal_.add_tail();
	vec4 cc_y = m.glrotateY(-yth) * st_norm - normal_.add_tail();
	xth = clock_x.abs() < cc_x.abs() ? xth : -xth;
	yth = clock_y.abs() < cc_y.abs() ? yth : -yth;
	
	mat4 rotate = m.glrotateX(xth) * m.glrotateY(yth);
	vec4 tr = ul.add_tail() - rotate * st_ul;
	plane_matrix_ = m.gltranslate(tr[0][0], tr[0][1], tr[0][2]) * rotate;
}

TextPlane::TextPlane(float f_sz, vec3 ul, vec3 ur, vec3 ll) : Plane{ul, ur, ll}
{
	font_size_ = f_sz;
}

mat4 TextPlane::trans_xy(int x, int y) const
{//return a matrix that will draw at coordinate x,y on plane
	mat4 m;
	return plane_matrix_ * m.gltranslate(-1 + x * font_size_, 1 - y * font_size_, 0)
		* m.glscale(font_size_ * 0.5, font_size_ * 0.5, font_size_ * 0.5);
}

mat4 TextPlane::next()
{
	if(font_size_ * (x_+2) > vec3(ur_ - ul_).abs()) return next_line();
	else return trans_xy(++x_, y_);
}

mat4 TextPlane::next_line()
{
	return trans_xy(x_=1, ++y_);
}

void TextPlane::setxy(int x, int y)
{
	x_ = x; y_ = y;
}
