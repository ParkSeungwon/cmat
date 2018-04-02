#pragma once
#include<GL/glew.h>
#include<GLFW/glfw3.h>
#include<vector>
#include<valarray>
#include<type_traits>
#include"vec.h"
#include"4x4.h"

bool glinit(GLFWwindow* window);
void init_globjects();
unsigned make_shader_program(std::string v, std::string f);

template <class T>
unsigned int gl_transfer_data(T* begin, T* end, GLenum mode = GL_ARRAY_BUFFER)
{
	int sz = end - begin;
	unsigned int vbo;
	glGenBuffers(1, &vbo);
	glBindBuffer(mode, vbo);
	
	T ar[sz];
	memcpy(ar, begin, sizeof(ar));
	glBufferData(mode, sizeof(ar), ar, GL_STATIC_DRAW);
	return vbo;
}

template<typename T> unsigned gl_transfer_index(const T& v, unsigned vbo = 0)
{
	if(!vbo) glGenBuffers(1, &vbo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, v.size() * sizeof(unsigned), 
			v.data(), GL_STATIC_DRAW);
	return vbo;
}

static void mcopy(float* p, const vec4& m) {
	for(int i=0; i<3; i++) *p++ = m[0][i];
}
static void mcopy(float* p, float m) {
	*p = m;
}
template <typename T> unsigned gl_transfer_data(const T& v, unsigned vbo = 0)
{//v should offer operator[], if vbo is not 0, then transfer using it
	int dim = 1;
	int sz = v.size();
	if(!vbo) glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	if(std::is_class<typename T::value_type>::value) dim = 3;//if Matrix
	float ar[sz * dim];
	for(int i=0; i<sz; i++) mcopy(ar+i*dim, v[i]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(ar), ar, GL_STATIC_DRAW);
	return vbo;
}

