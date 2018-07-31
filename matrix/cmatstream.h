#pragma once
#include"neural.h"
#include"cmat.h"

struct CmatStream
{
	std::vector<std::string> linebyline;
	int pos = 0;

	template<class T, unsigned W, unsigned H>
	CmatStream(const Cmat<T, W, H>& mat, int lines) {
		int gap[W] = {0,};
		for(int y=0; y<H; y++) for(int x=0; x<W; x++) {
			std::stringstream ss;
			ss << mat[x][y];
			int sz = ss.str().length();
			if(gap[x] < sz) gap[x] = sz;
		}

		int sum = 0;
		for(int i=0; i<W; i++) sum += gap[i];
		std::string space;
		for(int i=0; i<sum + W + 3; i++) space += ' ';

		for(int i=0; i < (lines - H) / 2; i++) linebyline.push_back(space);
		std::stringstream ss;
		ss << (H-1 ? "\u23a1 " : "[ ");
		for(int x=0; x<W; x++) ss << std::setw(gap[x]) << mat[x][0] << ' ';
		ss << (H-1 ? "\u23a4" : "]");
		linebyline.push_back(ss.str());
		ss.str("");
		ss.clear();

		for(int y=1; y<H-1; y++) {//middle lines
			ss << "\u23a2" << ' ';
			for(int x=0; x<W; x++) ss << std::setw(gap[x]) << mat[x][y] << ' ';
			ss << "\u23a5";
			linebyline.push_back(ss.str());
			ss.str("");
			ss.clear();
		}

		if(H > 1) {//last line
			ss << "\u23a3" << ' ';
			for(int x=0; x<W; x++) ss << std::setw(gap[x]) << mat[x][H-1] << ' ';
			ss << "\u23a6";
			linebyline.push_back(ss.str());
		}
		
		while(linebyline.size() < lines) linebyline.push_back(space);
	}
};

std::ostream& operator<<(std::ostream& o, CmatStream& r) {
	if(r.pos == r.linebyline.size()) r.pos = 0;
	o << r.linebyline[r.pos++];
	return o;
}

class NeuralNetStream
{
public:
	template<unsigned... Ns> NeuralNetStream(const NeuralNet<Ns...>& r) {
		get_parameter(Ns...);
		int line = *max(params.begin(), params.end());
		pretty_print(r, line);
	}
	void print() {
		for(int i=0; i<v[0].linebyline.size(); i++) {
			for(auto& a : v) o << a;
			o << endl;
		}
	}
private:
	std::vector<unsigned> params;
	std::vector<CmatStream> v;
	template<unsigned N, unsigned... Ns> void get_parameter(N, Ns...) {
		params.push_back(N);
		get_parameter(Ns...);
	}
	void get_parameter() {}
	template<unsigned Idx = 0, unsigned... Ns>
	void pretty_print(const Neural<Idx, Ns...>& r, int line) {
		if constexpr(Idx < sizeof...(Ns) - 1) {
			v.push_back(CmatStream{r.input(), line});
			v.push_back(CmatStream{r.weight(), line});
			v.push_back(CmatStream{r.output(), line});
			pretty_print<Idx + 1>(r);
		}
	}

};
