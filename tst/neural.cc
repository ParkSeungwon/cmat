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
	uniform_real_distribution<float> di{-1,1};
	net.init(di);
	cout << net.feed_forward({1,2,3});
	cout << input<0>(net) << weight<0>(net) << output<0>(net) << input<1>(net) << weight<1>(net) << output<1>(net) << endl;
	for(int i=0; i<100; i++) {
		net.back_propagation({1,0}, 1);
//		cout << output<0>(net) << weight<0>(net) << output<1>(net) << weight<1>(net) << output<2>(net) << endl;
//		cout << "predict" << endl << net.feed_forward({1,2,3});
	}
//	cout << delta<0>(net) << delta<1>(net) << endl;
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

TEST_CASE("근의 공식") {
	NeuralNet<3, 5, 5, 2> net;
	uniform_real_distribution<float> di{-1,1};
	net.init(di);
	uniform_int_distribution<int> dii{1, 9};
	random_device rd;
	int a = dii(rd), b = 2*dii(rd), c = dii(rd);
	net.feed_forward({a,b,c});
	for(int i=0; i<1000; i++) {
		if(b*b - 4*a*c >= 0) net.back_propagation({1, 0});
		else net.back_propagation({0,1});
	}
	int r = 0;
	for(int i=0; i<10; i++) {
		a = dii(rd), b = 2*dii(rd), c = dii(rd);
		auto m = net.feed_forward({a,b,c});
		if((b*b - 4*a*c >= 0) == (m[0][0] > m[0][1])) r++; 
		cout << b*b - 4*a*c << endl << m << endl;
	}
	cout <<"적중률"<< r << endl;
}

TEST_CASE("xor") {
	NeuralNet<2, 4, 4, 2> net;
	uniform_real_distribution<float> di{-1,1};
	net.init(di);
	for(int i=0; i<1000; i++) {
		net.feed_forward({1, 0});
		net.back_propagation({1, 0});
		net.feed_forward({0, 1});
		net.back_propagation({1, 0});
		net.feed_forward({1, 1});
		net.back_propagation({0, 1});
		net.feed_forward({0, 0});
		net.back_propagation({0, 1});
	}
	cout << "true" << endl << net.feed_forward({1,0});
	cout << "true" << endl << net.feed_forward({0,1});
	cout << "false" << endl << net.feed_forward({1,1});
	cout << "false" << endl << net.feed_forward({0,0});
}
