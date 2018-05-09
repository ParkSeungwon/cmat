#include"neural.h"
#include<iostream>
using namespace std;
float relu(float x) {
	if(x > 0 ) return x;
	else return 0;
}
array<char, 60000> read_train_label();
array<Cmat<unsigned char, 28, 28>, 60000> read_train_image();

int main()
{
	NeuralNet<28*28, 300, 100, 20, 10> net;
	auto label = read_train_label();
	auto image = read_train_image();
	uniform_real_distribution<float> di{-1, 1};
	net.init(di);
	for(int k=0; k<60; k++) {
		vector<float> v(28*28), d(10);
		for(int i=0; i<28; i++) for(int j=0; j<28; j++) v[j] = image[k][i][j] / 255.;
		for(int i=0; i<10; i++) d[i] = label[k] == i ? 1 : 0;
		net.feed_forward(v);
		net.back_propagation(d);
	}
	net.save_weights("mnist_weight.txt");
}
