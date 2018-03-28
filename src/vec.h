#pragma once
#include"base.h"

template<class T, unsigned N> struct Cmat<T, 1, N> : public CmatBase<T, 1, N>
{//vector matrix specialization
	Cmat() = default;
	Cmat(std::initializer_list<float> li) : CmatBase<float, 1, N>{li} {}
	T dot(const Cmat<T, 1, N>& r) const{
		T sum = 0;
		for(int i=0; i<N; i++) sum += (*this)[0][i] * r[0][i];
		return sum;
	}
	Cmat<T, 1, N+1> add_one() const {//x,y,z -> x,y,z,1
		Cmat<T, 1, N+1> m;
		for(int i=0; i<N; i++) m[0][i] = (*this)[0][i];
		m[0][N] = 1;
		return m;
	}
	Cmat<T, 1, N-1> del_one() const {
		Cmat<T, 1, N-1> m;
		for(int i=0; i<N-1; i++) m[0][i] = (*this)[0][i];
		return m;
	}
	float abs() const {
		float sum = 0;
		for(int i=0; i<N; i++) sum += (*this)[0][i] * (*this)[0][i];
		return sqrt(sum);
	}
	Cmat<T, 1, N>& normalize() {
		return *this /= abs();
	}
	float theta(const Cmat<T, 1, N>& r) const {
		return acos(dot(r) / (abs() * r.abs()));
	}
};

