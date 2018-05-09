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
	uniform_real_distribution<float> di{0, 1};
	net.init(di);
}
