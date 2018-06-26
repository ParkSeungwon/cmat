#include<algorithm>
#include<iostream>
#include"plot.h"
#include"neural.h"
using namespace std;
float relu(float x) {
	if(x > 0 ) return x;
	else return 0;
}

template<unsigned N> array<char, N> read_label(string filename) {
	ifstream f(filename);
	char c;
	for(int i=0; i<8; i++) f >> noskipws >> c;
	array<char, N> r;
	for(int i=0; i<N; i++) f >> noskipws >> r[i];
	return r;
}

template<unsigned N>
array<Cmat<unsigned char, 28, 28>, N> read_image(string filename) {
	ifstream f(filename);
	unsigned char c;
	for(int i=0; i<16; i++) f >> noskipws >> c;
	array<Cmat<unsigned char, 28, 28>, N> r;
	for(int k=0; k<N; k++) for(int i=0; i<28; i++) for(int j=0; j<28; j++) 
		f >> noskipws >> r[k][j][i];
	return r;
}

template<unsigned N> auto pool(Cmat<unsigned char, N, N> m) {
	static_assert(N % 2 == 0);
	Cmat<unsigned char, N/2, N/2> r;
	for(int i=0; i<N; i+=2) for(int j=0; j<N; j+=2) 
		r[i/2][j/2] = max(max(m[i][j], m[i][j+1]), max(m[i+1][j], m[i+1][j+1]));
	return r;
}

int main()
{
	const int TRAIN_SET =  1000;
	const int TEST_SET = 100;
	NeuralNet<7*7, 30, 20, 10> net;
	auto label = read_label<TRAIN_SET>("train-label.dat");
	auto image = read_image<TRAIN_SET>("train-image.dat");
	auto tlabel = read_label<TEST_SET>("test-label.dat");
	auto timage = read_image<TEST_SET>("test-image.dat");
	for(auto a : tlabel) cout << +a;
	uniform_real_distribution<float> di{0, 1};
	net.init(di);
//	net.transfer_function(relu);
	vector<float> err;
	for(int k=0; k<10; k++) {
		float sum = 0;
		for(int k=0; k<TRAIN_SET; k++) {
			auto a = pool(pool(image[k]));
			vector<float> v(7*7), d(10);
			for(int i=0; i<7; i++) for(int j=0; j<7; j++) 
				v[j + 7 * i] = a[j][i] / 255.;
			for(int i=0; i<10; i++) d[i] = label[k] == i ? 1 : 0;
			net.feed_forward(v);
			sum += net.back_propagation(d, .1);
		}
		err.push_back(sum/TRAIN_SET);
		cout << k << " epoch : ";
		net.save_weights(to_string(k) + ".txt");

		int correct = 0;
		for(int k=0; k<TEST_SET; k++) {
			auto a = pool(pool(timage[k]));
			vector<float> v(7*7);
			for(int i=0; i<7; i++) for(int j=0; j<7; j++)
				v[j + 7 * i] = a[j][i] / 255.;
			auto result = net.feed_forward(v);
			int n = std::max_element(result.arr_[0].begin(), result.arr_[0].end()) - result.arr_[0].begin();
			cout << n << ' ';
			if(n == tlabel[k]) correct++;
		}
		cout << correct << " correct" << endl;
	}
	auto x = arange(0, 1, 10);
	valarray<float> y{err.data(), 10};
	plot(x, y);
}
