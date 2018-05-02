#include<chrono>
#include<thread>
#include<iostream>
#include<mutex>
#include<SFML/Audio.hpp>
#include<random>
#include<complex>
#include"projectile.h"
#include"globj.h"
#include"vec.h"
#include"plane.h"
#include"4x4.h"
#define STEP 0.05
using namespace std;
bool glinit(GLFWwindow* window);
void init_globjects();
extern float thz, dest_x, dest_y, dest_z;
extern GLObjs objs;
extern mat4 KeyBindMatrix;
float x, y;//cur xy

deque<Projectile> bullets;
deque<vec4> enemies;
mutex mtx1, mtx2;
bool end_game = false;

void generate_bullet()
{
	unique_lock<mutex> lock{mtx1, defer_lock};
	while(!end_game) {//projectile{position, velocity}
		bullets.push_back(Projectile{{x+0.2*cos(thz), y+0.2*sin(thz), 4}, {0,0,-2}});
		bullets.push_back(Projectile{{x-0.2*cos(thz), y-0.2*sin(thz), 4}, {0,0,-2}});
		lock.lock();
		while(!bullets.empty() && bullets.front().out_of_bound()) bullets.pop_front();
		lock.unlock();
		this_thread::sleep_for(200ms);
	}
}

void generate_enemy()
{
	uniform_real_distribution<float> di{-1, 1};
	uniform_int_distribution<> di2{2, 6};
	random_device rd;
	vec4 pos;
	unique_lock<mutex> lock{mtx2, defer_lock};
	while(!end_game) {//initial x,y,z position
		pos[0][0] = di(rd); 
		pos[0][1] = di(rd); 
		pos[0][2] = -20; 
		pos[0][3] = di2(rd);//enemy type
		enemies.push_back(pos);
		lock.lock();
		while(!enemies.empty() && enemies.front()[0][2] > -5) enemies.pop_front();
		lock.unlock();
		this_thread::sleep_for(2s);
	}
}

void detect_crash()
{
	while(!end_game) {
		lock(mtx1, mtx2);
		for(auto& a : bullets) for(auto& b : enemies) {
			auto m = a.pos_ - b;
			if(m[0][0] * m[0][0] + m[0][1] * m[0][1] + m[0][2] * m[0][2] < 0.1) {
				a.pos_[0][2] = -100;//move out of screen
				b[0][2] = 100;//move out of screen
			}
		}		
		mtx1.unlock();
		mtx2.unlock();
		this_thread::sleep_for(50ms);
	}
}

int main()
{
	sf::Music music;
	if(!music.openFromFile("J.ogg")) return -1;
	music.play();

	TextPlane tp;
	if (!glfwInit()) return -1;
	GLFWwindow* window = glfwCreateWindow(1024, 1024, "Space War", NULL, NULL);
	if (!glinit(window)) return -1;
	init_globjects();

	mat4 proj, m;
	proj.glprojection(-1,1,-1,1,-5,5);
	unique_lock<mutex> lock1{mtx1, defer_lock}, lock2{mtx2, defer_lock};
	thread th1{generate_bullet}, th2{generate_enemy}, th3{detect_crash};

	string text = R"(THIS IS A TEST
HELLO WORLD01293827492874893
LONG LONG TIME AGO IN A  FAR FAR AWAY GALAXY)";
	while (!glfwWindowShouldClose(window)) {
		if(abs(dest_x - x) >= STEP || abs(dest_y - y) >= STEP) {//move ship x,y
			complex<float> to{dest_x, dest_y};
			complex<float> cur{x, y};
			float th = arg(to - cur);
			x += STEP * cos(th) / 3;
			y += STEP * sin(th) / 3;
		}

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		objs.matrix(proj * m.gltranslate(x,y,0) * m.glrotateZ(thz) * objs[0]);
		objs(0);//spaceship
		lock2.lock();
		for(auto& a : enemies) {
			objs.matrix(proj * m.gltranslate(a[0][0],a[0][1],a[0][2]) * objs[a[0][3]]);
			objs(a[0][3]);//enemy
			a[0][2] += 0.05;//advance forward
		}
		lock2.unlock();
		objs.matrix(proj * objs[1]);
		objs(1);//background
		
		lock1.lock();
		for(auto& a : bullets) {
			objs.matrix(proj * a.time_pass() * objs[7]);
			objs(7);//bullet
		}
		lock1.unlock();
		tp.setxy(0, 25);
		for(char c : text) {
			if(('A' <= c && c <= 'Z') || ('0' <= c && c <= '9')) {
				int k = c >= 'A' ? 8 + c - 'A' : 34 + c - '0';
				objs.matrix(proj * KeyBindMatrix * m.gltranslate(0, 0, -10) * 
						tp.next() * objs[k]);
				objs(k);
			} else if(c == '\n') tp.next_line();
			else tp.next();
		}
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	end_game = true;
	th1.join();
	th2.join();
	th3.join();
	glfwTerminate();
}
 
