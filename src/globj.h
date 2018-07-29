#pragma once
#include<deque>
#include<vector>
#include"matrix/cmat.h"
using vec3 = Cmat<float, 1, 3>;
using vec4 = Cmat<float, 1, 4>;
using mat4 = Cmat<float, 4, 4>;

class GLObject
{
public:
	GLObject();
	unsigned read_obj_file(std::string filename);
	unsigned read_objmtl(std::string filename);

	//setters
	void mode(GLenum md);
	void matrix(const mat4& m);
	void vertexes(const std::vector<vec4>& verts);
	void vertexes(std::vector<vec4>&& verts);
	void colors(const std::vector<vec4>& cols);
	void colors(std::vector<vec4>&& cols);
	void indices(const std::vector<unsigned>& ids);
	void indices(std::vector<unsigned>&& ids);
	void texture_file(std::string filename);

protected:
	void normals();
	void colors();//for texture mapping if filename is specified
	std::vector<vec4> vertexes_, colors_, normals_;
	std::vector<unsigned> indices_;
	mat4 matrix_, light_;
	GLenum mode_ = GL_TRIANGLES;
	std::string texture_file_;
	friend class GLObjs;

private:
	void normalize_vertex();
	static std::array<int, 3> parse_f(std::string numberNslash);
};

class GLObjs : protected GLObject
{
public:
	GLObjs();
	void operator()(int n);
	mat4 operator[](int n);
	GLObjs& operator+=(GLObject& r);
	unsigned transfer_all();
	void light(const mat4& light);//after transfer 
	void matrix(const mat4& r);
	unsigned transfer_data(const std::vector<vec4>& v, const char* var,
			unsigned vbo=0);
	unsigned update(std::vector<unsigned>&& v);

protected:
	unsigned indices(const std::vector<unsigned>& v, unsigned vbo=0);
	unsigned vbo[4];
	std::vector<unsigned> index_chunks_;
	unsigned shader_program_;
	std::vector<mat4> matrixes_;
	std::vector<GLenum> modes_;
	std::vector<std::string> texture_files_;
	std::vector<int> texture_num_;

private:
	unsigned read_texture();
	void fill_texture_num();
	void load_alphabet();
};


