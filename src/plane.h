#include"matrix/cmat.h"
using vec3 = Cmat<float, 1, 3>;
using vec4 = Cmat<float, 1, 4>;
using mat4 = Cmat<float, 4, 4>;

class Plane 
{
public:
	Plane(vec3 upper_left, vec3 upper_right, vec3 lower_left);

protected:
	mat4 plane_matrix_;
	vec3 normal_, ul_, ur_, ll_;
};

class TextPlane : public Plane
{
public:
	TextPlane(float font_sz=0.1, vec3 ul={-1,1,0}, vec3 ur={1,1,0}, vec3 ll={-1,-1,0});
	mat4 trans_xy(int x, int y) const;
	mat4 next();
	mat4 next_line();
	void setxy(int x, int y);

protected:
	int x_=0, y_=0;
	float font_size_;
};	
