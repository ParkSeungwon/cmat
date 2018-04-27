#include<map>
#include"vec.h"
using vec3 = Cmat<float, 1, 3>;

struct MTL 
{
	vec3 K[4];//Ka, Kd, Ks, Ke;
	float Ns, Ni, d;
	int illum;
	std::string map_K[5];//map_Ka, map_Kd, map_Ks, map_Ke, map_d;
};

std::map<std::string, MTL> read_mtl(std::string file);

	
