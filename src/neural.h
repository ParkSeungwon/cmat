#pragma once
#include<random>
#include"base.h"

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
{//recursive specialization
	auto& weight() { return Weight<Idx, N1, N2>::mat; }
	auto& layer() { return Layer<Idx, N1>::mat; }
	auto& delta() { return Delta<Idx, N1>::mat; }
};
template<unsigned Idx, unsigned N> struct Neural<Idx, N>
	: Layer<Idx, N>, Delta<Idx, N>
{//termination
	const int sz_ = Idx + 1;//parameter size
	auto& layer() { return Layer<Idx, N>::mat; }
	auto& delta() { return Delta<Idx, N>::mat; }
};

template<unsigned Idx, unsigned... Ns> auto& weight(Neural<Idx, Ns...>& r) {
	return r.weight();//tuple like getter
}
template<unsigned Idx, unsigned... Ns> auto& layer(Neural<Idx, Ns...>& r) {
	return r.layer();
}
template<unsigned Idx, unsigned... Ns> auto& delta(Neural<Idx, Ns...>& r) {
	return r.delta();
}

template<unsigned... Ns> class NeuralNet : public Neural<0, Ns...> {
public:
	template<class Di, unsigned Idx = 0> void init(Di di) {
		auto& a = weight<Idx>(*this);
		for(int i=0; i<a.width_; i++) for(int j=0; j<a.height_; j++) 
			a[i][j] = di(rd_);
		init<Di, Idx + 1>(di);
	}

private:
	std::random_device rd_;
	template<class Di>  void init<Di, sizeof...(Ns) - 1>(Di di) {}
};

