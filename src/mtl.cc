#include<fstream>
#include<string>
#include<map>
#include"mtl.h"
using namespace std;

static MTL read(istream& is)
{//call this after consuming newmtl line
	MTL mtl;
	string s;
	float x, y, z;
	int k;
	while(is >> s) {//if eof return stored mtl
		if(s == "newmtl") break;//if newmtl return stored mtl
		else {
			switch(s[0]) {
			case 'K':
				switch(s[1]) {
				case 'a': is >> x >> y >> z; mtl.K[0] = {x, y, z}; break;
				case 'd': is >> x >> y >> z; mtl.K[1] = {x, y, z}; break;
				case 's': is >> x >> y >> z; mtl.K[2] = {x, y, z}; break;
				case 'e': is >> x >> y >> z; mtl.K[3] = {x, y, z}; break;
				}
				break;
			case 'N':
				switch(s[1]) {
				case 's': is >> x; mtl.Ns = x; break;
				case 'i': is >> x; mtl.Ni = x; break;
				}
				break;
			case 'i': is >> k; mtl.illum = k; break;
			case 'd': is >> x; mtl.d = x; break;
			case 'm':
				if(s[4] == 'K') {
					switch(s[5]) {
					case 'a': is >> s; mtl.map_K[0] = s; break;
					case 'd': is >> s; mtl.map_K[1] = s; break;
					case 's': is >> s; mtl.map_K[2] = s; break;
					case 'e': is >> s; mtl.map_K[3] = s; break;
					}
				} else getline(is, s);//consume line
				break;
			}
		}
	}
	return mtl;
}

std::map<std::string, MTL> read_mtl(std::string file)
{
	map<string, MTL> r;
	ifstream f(file);
	string s;
	while(f >> s) if(s == "newmtl") break;
	while(f >> s) r[s] = read(f);//s == name
	return r;
}
