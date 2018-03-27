#pragma once
#include"cmat.h"

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
	CmatSquare<N> I() const {//inverse matrix
		float ad_bc = det();
		if(!ad_bc) throw "no inverse";
		CmatSquare<N> m;
		for(int i=0; i<N; i++) for(int j=0; j<N; j++) 
			m[i][j] = ((i+j) % 2 ? -1 : 1) * M(j, i).det() / ad_bc; //% > +
		return m;
	}

	CmatSquare<N-1> M(int x, int y) const {//get rid of x, y row and column 
		CmatSquare<N-1> m;
		float arr[N * N];
		for(int i=0,k=0; i<N; i++) for(int j=0; j<N; j++) 
			if(i != x && j != y) arr[k++] = (*this)[i][j];
		for(int i=0,k=0; i<N-1; i++) for(int j=0; j<N-1; j++) m[i][j] = arr[k++];
		return m;
	}
	float det() const {//to avoid 1x1 matrix ambiguity, ad-bc calculated at N==2
		if(N == 2) return (*this)[0][0]*(*this)[1][1] - (*this)[1][0]*(*this)[0][1];
		float sum = 0;
		for(int i=0, j=0; i<N; i++, j*=-1) sum += j * (*this)[i][0] * M(i, 0).det();
		return sum;
	}
};

template<unsigned N> struct Cmat<float, N, N> : public CmatSquare<N>
{//partial specialization for same interface
	Cmat() = default;
	Cmat(std::initializer_list<float> li) : CmatSquare<N>{li} {}
};

