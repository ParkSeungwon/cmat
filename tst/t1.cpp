#include<vector>
#include<iostream>
#include<cmath>
#include<cassert>
using namespace std;

vector<float> M(vector<float> v, int x, int y)
{//v = linearized nXn size matrix, return a matrix after removing col x, row y 
	int n = sqrt(v.size());
	auto it = v.begin();
	for(int j=0; j<n; j++) for(int i=0; i<n; i++) 
		if(x==i || y==j) it = v.erase(it);
		else it++;
	return v;
}

float det(vector<float> v)
{//ad - bc
	if(v.size() == 1) return v[0];
	int n = sqrt(v.size());
	float sum = 0;
	for(int i=0, j=1; i<n; i++, j*=-1) sum += j * v[i] * det(M(v, i, 0));
	return sum;
}

vector<float> I(vector<float> v) 
{
	float ad_bc = det(v);
	if(!ad_bc) throw "no inverse";
	int n = sqrt(v.size());
	vector<float> r;
	for(int j=0; j<n; j++) for(int i=0; i<n; i++)
		r.push_back( ((i+j) % 2 ? -1 : 1) * det(M(v, i, j)) / ad_bc );
	return r;
}

int main()
{
	vector<float> r1 {1,2,3,1,0,8,7,8,9};
	vector<float> r3 {1,2,3,3,4,5,4,4,7};
	assert((M(r1,1,1) == vector<float>{1,3,7,9}));
	assert((M(r1,2,1) == vector<float>{1,2,7,8}));
	vector<float> r2 {1,2,3,4};
	cout << det(r1) << ' ' << det(r2) << endl;
	for(auto a : I(r1)) cout << a << ' ';
	cout << endl;
	for(auto a : I(r3)) cout << a << ' ';
	cout << endl;
//	for(auto a : M(r, 1,0)) cout << a << ' ';

}



