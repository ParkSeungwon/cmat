#pragma once
#include"base.h"

template<class T> struct Add_ref {
	using type = T&;
};
template<class T> struct Add_c_ref {
	using type = const T&;
};
template<unsigned Idx, unsigned W, unsigned H> class Weight {
protected:
	Cmat<float, W, H> mat;
};
template<unsigned Idx, unsigned H> class Layer {
protected:
	Cmat<float, 1, H> mat;
};
template<unsigned Idx, unsigned H> class Delta {
protected:
	Cmat<float, 1, H> mat;
};

template<unsigned Idx, unsigned... Ns> struct Neural {};
template<unsigned Idx, unsigned N1, unsigned N2, unsigned... Ns>
struct Neural<Idx, N1, N2, Ns...>
	: Neural<Idx + 1, N2, Ns...>, Weight<Idx, N1, N2>, Layer<Idx, N1>, Delta<Idx, N1>
{
	auto& weight() {
		return Weight<Idx, N1, N2>::mat;
	}
	auto& layer() {
		return Layer<Idx, N1>::mat;
	}
	auto& delta() {
		return Delta<Idx, N1>::mat;
	}
};
template<unsigned Idx, unsigned N> struct Neural<Idx, N>
	: Layer<Idx, N>, Delta<Idx, N>
{
	const int sz = Idx + 1;
	auto& layer() {
		return Layer<Idx, N>::mat;
	}
	auto& delta() {
		return Delta<Idx, N>::mat;
	}
};

template<unsigned Idx, unsigned... Ns> auto& weight(Neural<Idx, Ns...>& r) {
	return r.weight();
}
template<unsigned Idx, unsigned... Ns> auto& layer(Neural<Idx, Ns...>& r) {
	return r.layer();
}
template<unsigned Idx, unsigned... Ns> auto& delta(Neural<Idx, Ns...>& r) {
	return r.delta();
}

template<unsigned... Ns> class NeuralNet : Neural<0, Ns...> {
public:

private:

};

