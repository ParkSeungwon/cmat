#pragma once
#include<random>
#include<functional>
#include<algorithm>
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
	template<class Di, unsigned Idx = 0> void init(Di di) {//di = random distribution
		if constexpr(Idx < sizeof...(Ns) - 1) {
			auto& a = weight<Idx>(*this);
			for(int i=0; i<a.width_; i++) for(int j=0; j<a.height_; j++) 
				a[i][j] = di(rd_);
			init<Di, Idx + 1>(di);
		}
	}
	void transfer_function(std::function<float(float)> f) { transfer_function_ = f; }
	auto feed_forward(std::vector<float> v) {
		for(int i=0; i<v.size(); i++) layer<0>(*this)[0][i] = v[i];
		feed_forward();
		return layer<sizeof...(Ns) - 1>(*this);
	}
	float back_propagation(std::vector<float> v, float alpha = 0.1) {
		float err = 0;
		auto& d = delta<sizeof...(Ns) - 1>(*this);
		for(int i = 0; i < d.height_; i++) {
			float o = layer<sizeof...(Ns) - 1>(*this)[0][i];
			d[0][i] = (v[i] - o) * o * ( 1 - o);
			err += (v[i] - o ) * (v[i] - o);
		}
		calc_delta();
		update_weight(alpha);
		return 0.5 * err;
	}

private:
	std::random_device rd_;
	std::function<float(float)> transfer_function_
		= [](float x) {return 1 / (1 + exp(-x));};//sigmoid default
	template<unsigned Idx = 0> void feed_forward() {
		if constexpr(Idx < sizeof...(Ns) - 1) {
			layer<Idx + 1>(*this) = weight<Idx>(*this) * layer<Idx>(*this);
			float* p = reinterpret_cast<float*>(layer<Idx + 1>(*this).arr_.data());
			transform(p, p + layer<Idx + 1>(*this).height_, p, transfer_function_);
			feed_forward<Idx + 1>();
		}
	}
	template<unsigned Idx = sizeof...(Ns) - 1> void calc_delta() {
		if constexpr(Idx > 0) {
			const auto& w = weight<Idx - 1>(*this);
			for(int i=0; i<w.width_; i++) {
				float o = layer<Idx>(*this)[0][i], sum = 0;
				for(int j=0; j<w.height_; j++)
					sum += w[i][j] * delta<Idx>(*this)[0][j];
				delta<Idx - 1>(*this)[0][i] = sum * o * (1 - o);
			}
			calc_delta<Idx - 1>();
		}
	}
	template<unsigned Idx = 0> void update_weight(float a) {
		if constexpr(Idx < sizeof...(Ns) - 1) {
			auto& w = weight<Idx>(*this);
			for(int i=0; i<w.width_; i++) for(int j=0; j<w.height_; j++) 
				w[i][j] += a * delta<Idx + 1>(*this)[0][i] * layer<Idx>(*this)[0][j];
			update_weight<Idx + 1>(a);
		}
	}
};

