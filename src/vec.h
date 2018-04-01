#pragma once
#include"base.h"

template<class T, unsigned N> struct Cmat<T, 1, N>
{//vector matrix specialization
	Cmat() = default;
	Cmat(std::initializer_list<float> li) : CmatBase<float, 1, N>{li} {}
	Cmat(const CmatBase<T, 1, N>& r) { arr_ = r.arr_; }
	T dot(const Cmat<T, 1, N>& r) const{
		T sum = 0;
		for(int i=0; i<N; i++) sum += (*this)[0][i] * r[0][i];
		return sum;
	}
	Cmat<T, 1, N+1> add_tail() const {//x,y,z -> x,y,z,1
		Cmat<T, N+1> m;
		for(int i=0; i<N; i++) m[0][i] = (*this)[0][i];
		m[0][N] = 1;
		return m;
	}
	Cmat<T, 1, N-1> remove_tail() const {
		Cmat<T, N-1> m;
		for(int i=0; i<N-1; i++) m[0][i] = (*this)[0][i];
		return m;
	}
	T abs() const {
		T sum = 0;
		for(int i=0; i<N; i++) sum += (*this)[0][i] * (*this)[0][i];
		return sqrt(sum);
	}
	Cmat<T, 1, N> normalize() {
		*this /= abs();//why return *this /= abs() not working??
		return *this;
	}
	T angle(const Cmat<T, 1, N>& r) const {
		return acos(dot(r) / (abs() * r.abs()));
	}
	Cmat<T, 1, 3> cross(const Cmat<T, 1, 3>& r) {//cross product not normalized
		return {(*this)[0][1] * r[0][2] - (*this)[0][2] * r[0][1],
				(*this)[0][2] * r[0][0] - (*this)[0][0] * r[0][2],
				(*this)[0][0] * r[0][1] - (*this)[0][1] * r[0][0]};
	}
};
