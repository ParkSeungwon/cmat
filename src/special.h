#include<cmath>
#include"cmat.h"

template<class T, unsigned W, unsigned H> struct Cmat : public CmatBase<T,W,H>
{//made base class to avoid repeatition in class functions
	Cmat() = default;
	Cmat(std::initializer_list<T> li) : CmatBase<T,W,H>{li} {}
};

template<unsigned N> struct CmatSquare : public CmatBase<float, N, N>
{//template class, not specialization
	CmatSquare(std::initializer_list<float> li) : CmatBase<float,N,N>{li} {}
	CmatSquare() = default;
	CmatSquare<N>& E() {
		for(int i=0; i<N; i++) for(int j=0; j<N; j++) 
			if(i == j) (*this)[i][j] = 1;
			else (*this)[i][j] = 0;
		return *this;
	}
};

template<unsigned N> struct Cmat<float, N, N> : public CmatSquare<N>
{//partial specialization for same interface
	Cmat() = default;
	Cmat(std::initializer_list<float> li) : CmatSquare<N>{li} {}
};

template<class T, unsigned N> struct Cmat<T, 1, N> : public CmatBase<T, 1, N>
{//vector matrix specialization
	Cmat() = default;
	Cmat(std::initializer_list<float> li) : CmatBase<float, 1, N>{li} {}
	T dot(const Cmat<T, 1, N>& r) const{
		T sum = 0;
		for(int i=0; i<N; i++) sum += (*this)[0][i] * r[0][i];
		return sum;
	}
	Cmat<T, 1, N+1> add_one() const {
		Cmat<T, 1, N+1> m;
		for(int i=0; i<N; i++) m[0][i] = (*this)[0][i];
		m[0][N] = 1;
		return m;
	}
	float abs() const {
		float sum = 0;
		for(int i=0; i<N; i++) sum += (*this)[0][i] * (*this)[0][i];
		return sqrt(sum);
	}
	Cmat<T, 1, N> normalize() {
		return *this /= abs();
	}
	float theta(const Cmat<T, 1, N>& r) const {
		return acos(dot(r) / (abs() * r.abs()));
	}
};

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
