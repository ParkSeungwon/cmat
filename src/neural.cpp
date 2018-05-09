#include"neural.h"
#include<iostream>
using namespace std;
float relu(float x) {
	if(x > 0 ) return x;
	else return 0;
}

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
//	net.transfer_function(relu);
	net.feed_forward({0.1,0.4,1});
	net.back_propagation({1,1,0,0,1});
	cout << weight<0>(net) << weight<1>(net);
	cout << layer<0>(net) << layer<1>(net) << layer<2>(net);
}
