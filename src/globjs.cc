#include<fstream>
#include<GL/glew.h>
#include<highgui.h>
#include<iostream>
#include"globj.h"
bool glinit(GLFWwindow* window);
void init_globjects();
unsigned make_shader_program(std::string v, std::string f);
using namespace std;

static string vshader = R"(
#version 130
uniform mat4 KeyBindMatrix;
in vec3 vertexes_;
in vec3 colors_;
in vec3 normals_;

out vec3 color;
out vec4 normal;
out vec3 vertex;

void main() {
	gl_PointSize = 3;
	gl_Position = KeyBindMatrix * vec4(vertexes_, 1.0);
	normal = KeyBindMatrix * vec4(normals_, 0.0f);
	color = colors_;
	vertex = vertexes_;
}
)";

static string fshader1 = R"(
#version 130
uniform mat4 LIGHT;
uniform mat4 KeyBindMatrix;
uniform mat4 INFO;
)";
static string fshader2 = R"(
in vec3 color;
in vec4 normal;
in vec3 vertex;
out vec4 f_color;

vec3 ambient = LIGHT[0].xyz;
vec3 diffuse = LIGHT[1].xyz;
vec3 specular = LIGHT[2].xyz;
vec3 light_pos = LIGHT[3].xyz;
vec3 view = vec3(0,0,3);

void main() {
	vec4 lp = vec4(light_pos, 1);
//	lp = KeyBindMatrix * lp;
	vec3 N = normalize(normal.xyz);
	vec3 L = normalize(lp.xyz - vertex);
	vec3 V = normalize(view);
	vec3 R = -L + 2 * dot(N, L) * N;

	vec3 shade = ambient + diffuse*max(dot(L, N),0) + specular*pow(dot(V, R), 24);
	
	vec4 col;
	if(INFO[0].x < 0) col = vec4(color, 1.0);
)";
static string fshader3 = R"(
	f_color = vec4(col.rgb * shade, col.a);
}
)";

GLObjs::GLObjs()
{
	glPolygonMode(GL_FRONT, GL_FILL);
}

GLObjs& GLObjs::operator+=(GLObject& r)
{
	if(r.indices_.empty()) for(int i=0; i<r.vertexes_.size(); i++) //if no indices
		r.indices_.push_back(i);
	r.normals();
	r.colors();//if texture, normalize vertex
//	for(int i=0; i<10; i++) cout << r.vertexes_[i] << r.colors_[i] << r.normals_[i];
	auto sz = vertexes_.size();
	vertexes_.insert(vertexes_.end(), r.vertexes_.begin(), r.vertexes_.end());
	colors_.insert(colors_.end(), r.colors_.begin(), r.colors_.end());
	normals_.insert(normals_.end(), r.normals_.begin(), r.normals_.end());
	auto idx = r.indices_;
	for(auto& a : idx) a += sz;
	indices_.insert(indices_.end(), idx.begin(), idx.end());

	index_chunks_.push_back(r.indices_.size());
	modes_.push_back(r.mode_);
	matrixes_.push_back(r.matrix_);
	texture_files_.push_back(r.texture_file_);
}

void GLObjs::fill_texture_num() 
{//assign number according to texture file
	int n = 0;
	vector<string> v;
	for(string s : texture_files_) {
		if(s == "") texture_num_.push_back(-1);
		else {
			auto it = find(v.begin(), v.end(), s);
			if(it == v.end()) {
				v.push_back(s);
				texture_num_.push_back(n++);
			} else texture_num_.push_back(it - v.begin());//appeared before
		}
	}
}

void GLObjs::light(const mat4& light) 
{
	mat4 m = light.transpose();
	m.gltransfer(shader_program_, "LIGHT");
}

void GLObjs::matrix(const mat4& m) 
{
	m.gltransfer(shader_program_, "KeyBindMatrix");
}

unsigned GLObjs::transfer_all()
{
	load_alphabet();
	fill_texture_num();
	unsigned r = read_texture();
	vbo[0] = transfer_data(vertexes_, "vertexes_");
	vbo[1] = transfer_data(colors_, "colors_");
	vbo[2] = transfer_data(normals_, "normals_");
	vbo[3] = indices(indices_);
	cout << "indices size : " << indices_.size() << endl;
	light({//default light
		0.2, 0.2, 0.2, 1, //ambient
		0.7, 0.7, 0.7, 0.5, //diffuse
		1, 1, 1, 1, //specular
		5, 5, -5, 1 //position 1 means a point 0 means a vector light
	});
	return r;
}

unsigned GLObjs::read_texture()
{///cube map texture
	using namespace cv;
	int n = texture_files_.size();
	unsigned vbo[n];
	glGenTextures(n, vbo);
	string s1, s2;
	for(int i=0,j=0; i<n; i++) if(texture_files_[i] != "") {//insert to shader program
		int k = texture_num_[i];
		if(k == j) {//first appearance
			cout << texture_files_[i] << endl;
			s1 += "uniform sampler2D TEXTURE";
			s1 += to_string(k) + ";\n";
			j++;
		}
		s2 += "\telse if(INFO[0].x ==" + to_string(i) + ") col = texture(TEXTURE";
		s2 += to_string(k) + ", color.xy);\n";
	}
	
	string fshader = fshader1 + s1 + fshader2 + s2 + fshader3;
	cout << vshader << fshader;
	shader_program_ = make_shader_program(vshader, fshader);
	if(!shader_program_) return 0;
	glUseProgram(shader_program_);

	for(int i=0,j=0; i<n; i++) if(texture_files_[i] != "") { 
		int k = texture_num_[i];
		if(k == j) {
			Mat im = imread(texture_files_[i]);
			glActiveTexture(GL_TEXTURE0 + j);
			glBindTexture(GL_TEXTURE_2D, vbo[j]);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, im.cols, im.rows, 0, GL_BGR, GL_UNSIGNED_BYTE, im.data);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			string tex = "TEXTURE" + to_string(j);
			glUniform1i(glGetUniformLocation(shader_program_, tex.data()), j++);
		}
	}
	return shader_program_;
}

unsigned GLObjs::indices(const vector<unsigned>& v, unsigned vbo)
{
	if(!vbo) glGenBuffers(1, &vbo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned) * v.size(), 
			v.data(), GL_STATIC_DRAW);
	return vbo;
}

unsigned GLObjs::update(vector<unsigned>&& v)
{//only for michelangelo
	indices(v, vbo[3]);
	indices_ = move(v);
	index_chunks_[0] = indices_.size();
	for(auto& a : normals_) a.O();
	int face;
	switch(modes_[0]) {
		case GL_TRIANGLES: face = 3; break;
		case GL_QUADS: face = 4; break;
		default: face = 3;
	}
	try{
		for(int i=0; i<indices_.size(); i+=face) {
			vec4 v1 = vertexes_[indices_[i+1]] - vertexes_[indices_[i]];
			vec4 v2 = vertexes_[indices_[i+2]] - vertexes_[indices_[i]];
			vec4 n = (v1.remove_tail() ^ v2.remove_tail()).normalize().add_tail();
			for(int j=0; j<face; j++)
				normals_[indices_[i+j]] = normals_[indices_[i+j]] + n;
		}
	} catch(const char* e) { cerr << e << endl; }
	for(auto& a : normals_) a.remove_tail().normalize().add_tail();
	transfer_data(normals_, "normals_", vbo[2]);
}	

mat4 GLObjs::operator[](int n)
{
	return matrixes_[n];
}

void GLObjs::operator()(int n)
{//draw nth object
	unsigned offset = 0;
	for(int i=0; i<n; i++) offset += index_chunks_[i];
	static mat4 info;
//	glActiveTexture(GL_TEXTURE0 + n);//???
	if(texture_files_[n] == "") info[0][0] = -1;
	else info[0][0] = n;
	info.gltransfer(shader_program_, "INFO");//send info for shader prg
	glDrawElements(modes_[n], index_chunks_[n], GL_UNSIGNED_INT, 
			(void*)(offset * sizeof(unsigned)));
}

unsigned GLObjs::transfer_data(const vector<vec4>& v, const char* var,
		unsigned vbo) 
{
	int sz = v.size();
	if(!vbo) glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	float ar[sz * 3];
	for(int i=0; i<sz; i++) for(int j=0; j<3; j++) ar[3*i+j] = v[i][0][j];
	glBufferData(GL_ARRAY_BUFFER, sizeof(ar), ar, GL_STATIC_DRAW);

	unsigned loc = glGetAttribLocation(shader_program_, var);
	glEnableVertexAttribArray(loc);
	cout << var << " : " << loc << ", " << v.size() <<endl;
	glVertexAttribPointer(loc, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
	return vbo;
}

void GLObjs::load_alphabet()
{
	char c[] = "?.obj";
	GLObject alphabet[36];
	int i=0;
	mat4 m;
	for(c[0]='A'; i<26; c[0]++, i++) {
		unsigned n = alphabet[i].read_obj_file(c);
		alphabet[i].colors({n, {0,1,1,0.2}});
		alphabet[i].matrix(m.glrotateX(-0.5 * M_PI));
	}
	for(c[0]='0'; i<36; c[0]++, i++) {
		unsigned n = alphabet[i].read_obj_file(c);
		alphabet[i].colors({n, {0,1,0,0.2}});
	}
	for(i=0; i<36; i++) *this += alphabet[i];
}

