#pragma once
#include"cmat.h"
//#include"matrix.h"

template<unsigned N> class CmatSquare : public CmatBase<float, N, N>
{//template class, not specialization
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

template<unsigned N> struct Cmat<float, N, N> : public CmatSquare<N>
{//partial specialization for same interface
	Cmat() = default;
	Cmat(std::initializer_list<float> li) : CmatSquare<N>{li} {}
};

