#include<fstream>
#include<GL/glew.h>
#include<opencv2/highgui.hpp>
#include<iostream>
#include"globj.h"
using namespace std;

GLObject::GLObject() { matrix_.E(); }

void GLObject::matrix(const mat4& m) { matrix_ = m; }
void GLObject::mode(GLenum md) { mode_ = md; }
void GLObject::vertexes(const vector<vec4>& v) { vertexes_ = v; }
void GLObject::vertexes(vector<vec4>&& v) { vertexes_ = move(v); }
void GLObject::colors(const vector<vec4>& v) { colors_ = v; }
void GLObject::colors(vector<vec4>&& v) { colors_ = move(v); }
void GLObject::colors()
{//color from texture, change to fit to texture u,v position
	if(texture_file_ == "") return;
	colors_.clear();
	for(int i=0; i<normals_.size(); i++) {
		float x = normals_[i][0][0];
		float y = normals_[i][0][1];
		float z = normals_[i][0][2];//find biggest abs->vertex coord
		float vx = vertexes_[i][0][0];
		float vy = vertexes_[i][0][1];
		float vz = vertexes_[i][0][2];

//		if(abs(x) > abs(y) && abs(x) > abs(z)) colors_.push_back({x>0?1:-1, vy, vz});
//		else if(abs(y)>abs(z) && abs(y)>abs(x)) colors_.push_back({vx, y>0?1:-1, vz});
//		else colors_.push_back({vx, vy, z>0?1:-1});

		if(abs(x) > abs(y) && abs(x) > abs(z)) //map to 육면체 전개도 
			colors_.push_back({x > 0 ? 0.5 + (1 - vz) / 8 : (vz + 1) / 8, 
					1.0f / 3 + (vy + 1) / 6, 0});
		else if(abs(y)>abs(z) && abs(y)>abs(x)) 
			colors_.push_back({0.25 + (vx + 1) / 8, 
					y > 0 ? (vz + 1) / 6 : 2.0f / 3 + (1 - vz) / 6, 0});
		else colors_.push_back({z > 0 ? 0.25 + (vx + 1) / 8 : 0.75 + (1 - vx) / 8,
				1.0f / 3 + (vy + 1) / 6, 0});
	}

	cout << "colors_ size : " << colors_.size() << endl;
	for(auto& a : colors_) for(int i=0; i<3; i++) 
		assert(a[0][i] >= -1 && a[0][i] <= 1);
}
void GLObject::indices(const vector<unsigned>& v) { indices_ = v; }
void GLObject::indices(vector<unsigned>&& v) { indices_ = move(v); }
void GLObject::texture_file(string f) { texture_file_ = f; }
void GLObject::normals()
{///should come after setting mode
	if(normals_.size() == vertexes_.size()) return;//calculated normal present
	normals_.resize(vertexes_.size());
	int face;
	switch(mode_) {
		case GL_TRIANGLES: face = 3; break;
		case GL_QUADS: face = 4; break;
		default: face = 3;
	}
	try{
		for(int i=0; i<indices_.size(); i+=face) {//calc normals
			vec4 v1 = vertexes_[indices_[i+1]] - vertexes_[indices_[i]];
			vec4 v2 = vertexes_[indices_[i+2]] - vertexes_[indices_[i]];
			vec4 n = (v1.remove_tail() ^ v2.remove_tail()).normalize().add_tail();
			for(int j=0; j<face; j++)
				normals_[indices_[i+j]] = normals_[indices_[i+j]] + n;
		}
	} catch(const char* e) { cerr << e << endl; }
	for(auto& a : normals_) a = a.remove_tail().normalize().add_tail();
}

unsigned GLObject::read_obj_file(string file)
{//return index? size to use in colors
	int face = 0;
	string s;
	ifstream f(file);
	while(getline(f, s)) {
		stringstream ss{s};
		ss >> s;
		if(s == "v") {
			float x,  y, z;
			ss >> x >> y >> z;
			vertexes_.push_back(vec4{x, y, z, 1});
		} else if(s == "f") {
			while(getline(ss, s, '/')) {
				indices_.push_back(stoi(s)-1);
				getline(ss, s, ' ');
				face++;
			}
			if(face == 3) mode(GL_TRIANGLES);
			else if(face == 4) mode(GL_QUADS);
		} else if(s == "vn") {
			float x, y, z;
			ss >> x >> y >> z;
			normals_.push_back(vec4{x, y, z, 1});
		}
	}
	cout << file << "\'s indices size : " << indices_.size() << endl;
	normalize_vertex();
	return vertexes_.size();
}	

unsigned GLObject::read_objmtl(string file)
{
	int face = 0;
	string s;
	ifstream f(file);
	while(getline(f, s)) {
		stringstream ss{s};
		ss >> s;
		if(s == "v") {
			float x,  y, z;
			ss >> x >> y >> z;
			vertexes_.push_back(vec4{x, y, z, 1});
		} else if(s == "f") {
			while(getline(ss, s, '/')) {
				indices_.push_back(stoi(s)-1);
				getline(ss, s, ' ');
				face++;
			}
			if(face == 3) mode(GL_TRIANGLES);
			else if(face == 4) mode(GL_QUADS);
		} else if(s == "vn") {
			float x, y, z;
			ss >> x >> y >> z;
			normals_.push_back(vec4{x, y, z, 1});
		} else if(s == "vt") {
		}			
	}
	cout << file << "\'s indices size : " << indices_.size() << endl;
	normalize_vertex();
	return vertexes_.size();
}

void GLObject::normalize_vertex()
{//make object fit into -1 ~ 1
	float xmin, xmax, ymin, ymax, zmin, zmax;
	xmin = xmax = vertexes_[0][0][0];
	ymin = ymax = vertexes_[0][0][1];
	zmin = zmax = vertexes_[0][0][2];
	for(auto& a : vertexes_) {
		if(xmin > a[0][0]) xmin = a[0][0];
		if(xmax < a[0][0]) xmax = a[0][0];
		if(ymin > a[0][1]) ymin = a[0][1];
		if(ymax < a[0][1]) ymax = a[0][1];
		if(zmin > a[0][2]) zmin = a[0][2];
		if(zmax < a[0][2]) zmax = a[0][2];
	}
	float x = xmax - xmin;
	float y = ymax - ymin;
	float z = zmax - zmin;
	float rate = max(x, max(y,z));
	for(auto& a : vertexes_) {
		a[0][0] -= xmin;
		a[0][1] -= ymin;
		a[0][2] -= zmin;
		for(int i=0; i<3; i++) {//??
			a[0][i] /= rate;
			a[0][i] -= 0.5;
			a[0][i] *= 2;
		}
	}
	for(auto& a : vertexes_) for(int i=0; i<3; i++) 
		assert(a[0][i] >= -1 && a[0][i] <= 1);
}
