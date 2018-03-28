#pragma once
#include"square.h"

template<> struct Cmat<float, 4, 4> : public CmatSquare<4>
{//specialization for opengl
	Cmat() = default;
	Cmat(std::initializer_list<float> li) : CmatSquare<4>{li} {}
	Cmat<float, 4, 4> gltranslate(float x, float y, float z) {
		E();
		(*this)[3][0] = x;
		(*this)[3][1] = y;
		(*this)[3][2] = z;
		return *this;
	}
	Cmat<float, 4, 4> glrotateZ(float th) {
		E();
		(*this)[0][0] = cos(th);
		(*this)[1][0] = -sin(th);
		(*this)[0][1] = sin(th);
		(*this)[1][1] = cos(th);
		return *this;
	}
	Cmat<float, 4, 4> glrotateX(float th) {
		E();
		(*this)[1][1] = cos(th);
		(*this)[2][1] = -sin(th);
		(*this)[1][2] = sin(th);
		(*this)[2][2] = cos(th);
		return *this;
	}
	Cmat<float, 4, 4> glrotateY(float th) {
		E();
		(*this)[0][0] = cos(th);
		(*this)[2][0] = -sin(th);
		(*this)[0][2] = sin(th);
		(*this)[2][2] = cos(th);
		return *this;
	}
	Cmat<float, 4, 4> glscale(float x, float y, float z) {
		E();
		(*this)[0][0] = x;
		(*this)[1][1] = y;
		(*this)[2][2] = z;
		return *this;
	}

	Cmat<float, 4, 4> glortho(float left, float right,
			float bottom, float top, float far, float near) {
		E();
		(*this)[0][0] = 2 / (right - left);
		(*this)[1][1] = 2 / (top - bottom);
		(*this)[2][2] = -2 / (far - near);
		(*this)[3][0] = -(left + right) / (right - left);
		(*this)[3][1] = -(top + bottom) / (top - bottom);
		(*this)[3][2] = -(far + near) / (far - near);
		return *this;
	}

	Cmat<float, 4, 4> glprojection(float left, float right, 
			float bottom, float top, float far, float near) {
		E();
		(*this)[0][0] = 2 * near / (right - left);
		(*this)[1][1] = 2 * near / (top - bottom);
		(*this)[2][2] = -(far + near) / (far - near);
		(*this)[2][0] = (left + right) / (right - left);
		(*this)[2][1] = (top + bottom) / (top - bottom);
		(*this)[3][2] = -2 * far * near / (far - near);
		(*this)[2][3] = -1;
		(*this)[3][3] = 0;
		return *this;
	}
};
