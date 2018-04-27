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
				case 'a': f >> x >> y >> z; mtl.K[0] = {x, y, z}; break;
				case 'd': f >> x >> y >> z; mtl.K[1] = {x, y, z}; break;
				case 's': f >> x >> y >> z; mtl.K[2] = {x, y, z}; break;
				case 'e': f >> x >> y >> z; mtl.K[3] = {x, y, z}; break;
				}
			case 'N':
				switch(s[1]) {
				case 's': f >> x; mtl.Ns = x; break;
				case 'i': f >> x; mtl.Ni = x; break;
				}
			case 'i': f >> k; mtl.illum = k; break;
			case 'd': f >> x; mtl.d = x; break;
			case 'm':
				if(s[4] == 'K') {
					switch(s[5]) {
						case 'a': f >> s; mtl.map_K[0] = s; break;
						case 'd': f >> s; mtl.map_K[1] = s; break;
						case 's': f >> s; mtl.map_K[2] = s; break;
						case 'e': f >> s; mtl.map_K[3] = s; break;
					}
				} else {
					f >> s; 
					mtl.map_K[4] = s;
				}
			}
			r[name] = mtl;
		}
	}
	return r;
}


	
