#include"globj.h"
using namespace std;

GLObjs objs;
void init_globjects()
{
	GLObject spaceship, background, projectile, ironman, missile, monkey, hare, dummy;

	spaceship.read_obj_file("space_frigate_6.obj");
	spaceship.texture_file("google.jpg");
	mat4 m;
	spaceship.matrix(m.gltranslate(0,0,-6) * m.glrotateY(M_PI/2) * m.glscale(0.6,0.6,0.6) * m.gltranslate(0, 0.8, 0));
	
	ironman.read_obj_file("ironman.obj");
	ironman.texture_file("google.jpg");
	ironman.matrix(m.glrotateX(1.5 * M_PI) * m.glscale(0.8,0.8,0.8));

	vec3 ve[8] = {{0,0,0}, {1,0,0}, {1,1,0}, {0,1,0},
		{0,0,1}, {1,0,1}, {1,1,1}, {0,1,1}};
	vector<vec4> v;
	for(auto a : ve) v.emplace_back(a.add_tail());
	background.vertexes(v);
	background.texture_file("1.jpg");
	background.indices({0,1,2,3, 7,6,5,4, 4,5,1,0, 5,6,2,1, 6,7,3,2, 7,4,0,3});
	background.matrix(m.gltranslate(0,0,-9.9) * m.glscale(1,1,5));
	background.mode(GL_QUADS);

	monkey.read_obj_file("monkey.obj");
	monkey.texture_file("brick.png");
	monkey.matrix(m.glscale(.5,.5,.5));

	projectile.vertexes(v);
	projectile.indices({3,2,1,0, 4,5,6,7, 0,1,5,4, 1,2,6,5, 2,3,7,6, 3,0,4,7});
	projectile.colors(vector<vec4>{24, {1,0,1, 1}});
	projectile.matrix(m.glscale(0.05, 0.05, 0.05));
	projectile.mode(GL_QUADS);

	missile.read_obj_file("missile.obj");
	missile.texture_file("steel.png");
	missile.matrix(m.glscale(.5,.5,.5));

	hare.read_obj_file("hare.obj");
	hare.texture_file("marble.jpg");
	hare.matrix(m.glrotateX(-M_PI/2) * m.glscale(0.6,0.6,0.6));

	auto n = dummy.read_obj_file("dummy_obj.obj");
//	dummy.colors({n,{1,0,0}});
	dummy.texture_file("steel.png");
	dummy.matrix(m.glscale(0.5,0.5,.5));

	objs += spaceship;
	objs += background;
	objs += ironman;
	objs += missile;
	objs += monkey;
	objs += hare;
	objs += dummy;
	objs += projectile;
	objs.transfer_all();
}

