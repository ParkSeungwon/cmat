#include"neural.h"
#include<iostream>
using namespace std;

int main()
{
	NeuralNet<3, 4, 5> net;
	auto a = weight<0>(net);
	a[0][0] = 3;
	cout << a << endl;
	cout << layer<1>(net);
	uniform_real_distribution<float> di{-1, 1};
	net.init(di);
	cout << weight<0>(net) << weight<1>(net);
	net.feed_forward({1,1,1});
	cout << layer<0>(net) << layer<1>(net) << layer<2>(net);
}
