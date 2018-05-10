#include"neural.h"
#include"plot.h"
#include<algorithm>
#include<iostream>
#define TEST_SET 100
#define TRAIN_SET 1000
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

int main()
{
	NeuralNet<28*28, 300, 100, 20, 10> net;
	auto label = read_label<TRAIN_SET>("train-label.dat");
	auto image = read_image<TRAIN_SET>("train-image.dat");
	auto tlabel = read_label<TEST_SET>("test-label.dat");
	auto timage = read_image<TEST_SET>("test-image.dat");
	for(auto a : tlabel) cout << +a;
	uniform_real_distribution<float> di{0, 1};
	net.init(di);
	vector<float> err;
	for(int k=0; k<10; k++) {
		float sum = 0;
		for(int k=0; k<TRAIN_SET; k++) {
			vector<float> v(28*28), d(10);
			for(int i=0; i<28; i++) for(int j=0; j<28; j++) 
				v[j + 28 * i] = image[k][j][i] / 255.;
			for(int i=0; i<10; i++) d[i] = label[k] == i ? 1 : 0;
			net.feed_forward(v);
			sum += net.back_propagation(d, 1);
		}
		err.push_back(sum/TRAIN_SET);
		cout << k << " epoch : ";
		net.save_weights(to_string(k) + ".txt");

		int correct = 0;
		for(int k=0; k<TEST_SET; k++) {
			vector<float> v(28*28);
			for(int i=0; i<28; i++) for(int j=0; j<28; j++)
				v[j + 28 * i] = timage[k][j][i] / 255.;
			auto result = net.feed_forward(v);
			if(std::max_element(result.arr_[0].data(), result.arr_[0].data() + 10) - result.arr_[0].data() == tlabel[k]) correct++;
		}
		cout << correct << " correct" << endl;
	}
	auto x = arange(0, 1, 10);
	valarray<float> y{err.data(), 10};
	plot(x, y);
}
