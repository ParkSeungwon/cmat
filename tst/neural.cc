#include<catch.hpp>
#include<chrono>
#include<array>
#include<random>
#include<iostream>
#include<map>
#include<algorithm>
#include<any>
#include"matrix/cmat.h"
#include"plane.h"
#include"matrix/neural.h"
#include"mtl.h"
using namespace std::chrono;
using namespace std;
using mat4 = Cmat<float, 4, 4>;
using vec4 = Cmat<float, 1, 4>;

TEST_CASE("Neural Net test") {
	NeuralNet<3, 5, 5, 2> net;
	uniform_real_distribution<float> di{0,1};
	net.init(di);
	cout << net.feed_forward({1,2,3});
	cout << layer<0>(net) << weight<0>(net) << layer<1>(net) << weight<1>(net) << layer<2>(net) << endl;
	for(int i=0; i<100; i++) {
	net.back_propagation({0,1}, .1);
	//cout << layer<0>(net) << weight<0>(net) << layer<1>(net) << weight<1>(net) << layer<2>(net) << endl;
	cout << "predict" << endl << net.feed_forward({1,2,3});
	}
	cout << delta<0>(net) << delta<1>(net) << endl;
}

TEST_CASE("binding") {
	map<int, string> m;
	m[3] = "hello";
	tuple<int, float, string> tp{2, 0.3, "hello"};
	auto& [a,b,c] = tp;
//	cout << a << b << c;
	if(int i=0; i<10) cout << "ok";
//	std::vector<double> vals = {1.0, 2.0, 3.0, 4.0};
//	auto my_square = [](double value){return value*value;};
//	std::for_each(std::execution::par, vals.begin(), vals.end(), my_square);
//	cout << "한글";
	any an;
	an = 'd';
	an = 3;
}
