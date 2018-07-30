#pragma once
#include<random>
#include<functional>
#include<fstream>
#include<algorithm>
#include"cmat.h"

template<unsigned Idx, unsigned W, unsigned H> class Weight {
protected:
	Cmat<float, W+1, H> mat;//+1 = bias
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
			for(int i=0; i<a.width(); i++) for(int j=0; j<a.height(); j++) 
				a[i][j] = di(rd_);
			init<Di, Idx + 1>(di);
		}
	}
	void transfer_function(std::function<float(float)> f) { activation_function_ = f; }
	void diff_transfer(std::function<float(float)> f) { diff_activation_ = f; }
	auto feed_forward(std::vector<float> v) {
		for(int i=0; i<v.size(); i++) layer<0>(*this)[0][i] = v[i];
		feed_forward();
		return layer<sizeof...(Ns) - 1>(*this);
	}
	float back_propagation(std::vector<float> v, float alpha = 0.1) {
		float err = 0;
		auto& d = delta<sizeof...(Ns) - 1>(*this);
		for(int i = 0; i < d.height(); i++) {
			float o = layer<sizeof...(Ns) - 1>(*this)[0][i];
			d[0][i] = (v[i] - o) * diff_activation_(o);//??? (o)
			err += (v[i] - o ) * (v[i] - o);
		}
		calc_delta();
		update_weight(alpha);
		return 0.5 * err;
	}
	void save_weights(std::string filename) {
		std::ofstream f(filename);
		save_weights(f);
	}
	void load_weights(std::string filename) {
		std::ifstream f(filename);
		load_weights(f);
	}
protected:
	std::function<float(float)>
		activation_function_ = sigmoid, diff_activation_ = diff_sigmoid;
private:
	std::random_device rd_;
	static float sigmoid(float x) { return 1 / (1 + exp(-x)); }
	static float diff_sigmoid(float x) { return sigmoid(x) * (1 - sigmoid(x)); }
	template<unsigned Idx = 0> void save_weights(std::ostream& o) {
		if constexpr(Idx < sizeof...(Ns) - 1) {
			o << weight<Idx>(*this) << std::endl;
			save_weights<Idx + 1>(o);
		}
	}
	template<unsigned Idx = 0> void load_weights(std::istream& is) {
		if constexpr(Idx < sizeof...(Ns) - 1) {
			std::string s; float f;
			auto& w = weight<Idx>(*this);
			for(int j=0; j<w.height(); j++) {
				is >> s;
				for(int i=0; i<w.width(); i++) is >> w[i][j];
				is >> s;
			}
			load_weights<Idx + 1>(is);
		}
	}
	template<unsigned H> static auto add_bias(Cmat<float, 1, H> m) {
		Cmat<float, 1, H+1> r;
		for(int i=0; i<H; i++) r[0][i] = m[0][i];
		r[0][H] = 1;
		return r;
	}
	template<unsigned Idx = 0> void feed_forward() {
		if constexpr(Idx < sizeof...(Ns) - 1) {
			layer<Idx + 1>(*this) = weight<Idx>(*this) * add_bias(layer<Idx>(*this));
			auto& a = layer<Idx + 1>(*this).arr_[0];
			if(Idx + 1 != sizeof...(Ns) - 1) 
				transform(a.begin(), a.end(), a.begin(), activation_function_);
			feed_forward<Idx + 1>();
		}
	}
	template<unsigned Idx = sizeof...(Ns) - 1> void calc_delta() {
		if constexpr(Idx > 0) {
			const auto& w = weight<Idx - 1>(*this);
			for(int i=0; i<w.width(); i++) {
				float o = layer<Idx>(*this)[0][i], sum = 0;
				for(int j=0; j<w.height(); j++)
					sum += w[i][j] * delta<Idx>(*this)[0][j];
				delta<Idx - 1>(*this)[0][i] = sum * diff_activation_(layer<Idx>(*this)[0][i]);
			}
			calc_delta<Idx - 1>();
		}
	}
	template<unsigned Idx = 0> void update_weight(float a) {
		if constexpr(Idx < sizeof...(Ns) - 1) {
			auto& w = weight<Idx>(*this);
			for(int i=0; i<w.width(); i++) for(int j=0; j<w.height(); j++) 
				w[i][j] += a * delta<Idx + 1>(*this)[0][i] * layer<Idx>(*this)[0][j];
			update_weight<Idx + 1>(a);
		}
	}
};

