#include<fstream>
#include<string>
#include<map>
#include"mtl.h"
using namespace std;

std::map<std::string, MTL> read_mtl(std::string file)
{
	map<string, MTL> r;
	ifstream f(file);
	string s, name;
	float x, y, z;
	int k;
	while(f >> s) {
		if(s == "newmtl") {
			MTL mtl;
			f >> name;
			f >> s;
			switch(s[0]) {
			case 'K':
				switch(s[1]) {
				case 'a': f >> x >> y >> z; mtl.Ka = {x, y, z}; break;
				case 'd': f >> x >> y >> z; mtl.Kd = {x, y, z}; break;
				case 's': f >> x >> y >> z; mtl.Ks = {x, y, z}; break;
				case 'e': f >> x >> y >> z; mtl.Ke = {x, y, z}; break;
				}
			case 'N':
				switch(s[1]) {
				case 's': f >> x; mtl.Ns = x; break;
				case 'i': f >> x; mtl.Ni = x; break;
				}
			case 'i': f >> k; mtl.illum = k; break;
			case 'd': f >> x; mtl.d = x; break;
			case 'm':
				switch(s[4]) {
				case 'a': f >> s; mtl.map_Ka = s; break;
				case 'd': f >> s; mtl.map_Kd = s; break;
				case 's': f >> s; mtl.map_Ks = s; break;
				case 'e': f >> s; mtl.map_Ke = s; break;
				default: f >> s; mtl.map_d = s; break;
				}
			}
			r[name] = mtl;
		}
	}
	return r;
}


	
