#pragma once
#include<sstream>
#include<iomanip>
#include<cassert>
#include<cmath>

template<class T, unsigned W, unsigned H> class CmatBase
{//compile time matrix base class
public:
	CmatBase() = default;
	CmatBase(std::initializer_list<T> li) {
		assert(li.size() == W * H);
		auto it = li.begin();
		for(int j=0; j<H; j++) for(int i=0; i<W; i++) (*this)[i][j] = *it++;
	}
	const T* operator[](unsigned n) const { return arr[n]; } 
	T* operator[](unsigned n) { return arr[n]; }
	template<unsigned R> CmatBase<T,R,H> operator*(const CmatBase<T,R,W>& r) const {
		CmatBase<T, R, H> m;
		for(int i=0; i<R; i++) for(int j=0; j<H; j++) 
			for(int k=0; k<W; k++) m[i][j] += (*this)[k][j] * r[i][k];
		return m;
	}
	CmatBase<T, W, H> operator+(const CmatBase<T, W, H>& r) const {
		CmatBase<T, W, H> m;
		for(int i=0; i<W; i++) for(int j=0; j<H; j++) 
			m[i][j] = (*this)[i][j] + r[i][j];
		return m;
	}
	CmatBase<T, W, H> operator-(const CmatBase<T, W, H>& r) const {
		CmatBase<T, W, H> m;
		for(int i=0; i<W; i++) for(int j=0; j<H; j++) 
			m[i][j] = (*this)[i][j] - r[i][j];
		return m;
	}
	CmatBase<T, W, H>& operator+=(const CmatBase<T, W, H>& r) {
		return *this = *this + r;
	}
	CmatBase<T, W, H>& operator-=(const CmatBase<T, W, H>& r) {
		return *this = *this - r;
	}
	bool operator==(const CmatBase<T, W, H>& r) {
		for(int i=0; i<W; i++) for(int j=0; j<H; j++) 
			if((*this)[i][j] != r[i][j]) return false;
		return true;
	}
	bool operator!=(const CmatBase<T, W, H>& r) {
		return !(*this == r);
	}
	CmatBase<T, H, W> transpose() const {
		CmatBase<T, H, W> m;
		for(int i=0; i<W; i++) for(int j=0; j<H; j++) m[j][i] = (*this)[i][j];
		return m;
	}
	CmatBase<T, W, H> operator*(T mul) const {
		CmatBase<T, H, W> m{*this};
		for(int i=0; i<W; i++) for(int j=0; j<H; j++) m[i][j] = m[i][j] * mul;
		return m;
	}
	CmatBase<T, W, H>& operator*=(T mul) {
		return *this = *this * mul;
	}
	CmatBase<T, W, H> operator/(T div) const {
		CmatBase<T, H, W> m{*this};
		for(int i=0; i<W; i++) for(int j=0; j<H; j++) m[i][j] = m[i][j] / div;
		return m;
	}
	CmatBase<T, W, H>& operator/=(T div) const {
		return *this = *this / div;
	}
	friend CmatBase<T, W, H> operator*(T mul, const CmatBase<T, W, H>& r) {
		return r * mul;
	}

protected:
	T arr[W][H] = {{0,},};
};

template <class T, unsigned W, unsigned H>
std::ostream& operator<<(std::ostream& o, const CmatBase<T, W, H>& r)
{//stream output
	int gap[W] {0,};
	for(int y=0; y<H; y++) for(int x=0; x<W; x++) {
		std::stringstream ss;
		ss << r[x][y];
		int sz = ss.str().length();
		if(gap[x] < sz) gap[x] = sz;
	}

	o << "\u23a1" << ' ';
	for(int x=0; x<W; x++) o << std::setw(gap[x]) << r[x][0] << ' ';
	o << "\u23a4" << std::endl;
	for(int y=1; y<H-1; y++) {
		o << "\u23a2" << ' ';
		for(int x=0; x<W; x++) o << std::setw(gap[x]) << r[x][y] << ' ';
		o << "\u23a5" << std::endl;
	}
	o << "\u23a3" << ' ';
	for(int x=0; x<W; x++) o << std::setw(gap[x]) << r[x][H-1] << ' ';
	o << "\u23a6" << std::endl;
	
	return o;
}

template<unsigned N> class CmatSquare : public CmatBase<float, N, N>
{//middle base template class, not specialization, I() not possible <- recursion
public:
	CmatSquare(std::initializer_list<float> li) : CmatBase<float,N,N>{li} {}
	CmatSquare() = default;
	CmatSquare<N>& E() {
		for(int i=0; i<N; i++) for(int j=0; j<N; j++) 
			if(i == j) (*this)[i][j] = 1;
			else (*this)[i][j] = 0;
		return *this;
	}
//	CmatSquare<N> I() const {//inverse matrix
//		Matrix<float> m{N, N};
//		for(int i=0; i<n; i++) for(int j=0; j<n; j++) m[i+1][j+1] = (*this)[i][j];
//		try { m = m.I(); }
//		catch(const char* e) { cerr << e << endl; }
//		CmatSquare<N> r;
//		for(int i=0; i<n; i++) for(int j=0; j<n; j++) r[i][j] = m[i+1][j+1];
//		return r;
//	}

};



//template specializations
template<class T, unsigned W, unsigned H> struct Cmat : public CmatBase<T,W,H>
{//made base class to avoid repeatition in class functions
	Cmat() = default;
	Cmat(std::initializer_list<T> li) : CmatBase<T,W,H>{li} {}
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
