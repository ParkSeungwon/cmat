#include"game.h"
using namespace std;

bool Block::operator==(const Block& r) const
{
	return color == r.color;
}

uniform_int_distribution<int> Block::di{0,4};
random_device Block::rd;

Block::Block()
{
	for(int i=0; i<BOARD_SZ; i++) for(int j=0; j<BOARD_SZ; j++) 
		color = static_cast<Color>(di(rd));
}

ostream& operator<<(ostream& o, const Block& b)
{
	char c;
	switch(b.color) {
		case Block::Color::RED: c = 'r'; break;
		case Block::Color::GREEN: c = 'g'; break;
		case Block::Color::BLUE: c = 'b'; break;
		case Block::Color::GOLD: c = 'l'; break;
		case Block::Color::BLACK: c = 'd'; break;
	}
	switch(b.level) {
		case Block::Level::EXPLOSIVE: c = toupper(c); break;
		case Block::Level::DIAMOND: c = '+'; break;
		case Block::Level::DELETE: c = ' '; break;
		default: break;
	}					   
	o << c;
	return o;
}

bool Board::find_match()//rgbyd
{//find vertical or horizontal row with more than 3 -> mark that point
	bool found = false;
	for(int i=0, k=0; i<BOARD_SZ; i++, k=0) for(int j=0; j<BOARD_SZ-1; j++) {
		if(board_[i][j] == board_[i][j+1]) k++;//vertical scan
		else k = 0;
		if(k >= 2) remove(i, j+1, false), found = true;//remove last one always
		if(k == 2) for(int n=-1; n<1; n++) remove(i, j+n, false);//remove former 2
		else if(k == 3) board_[i][j+1].change = Block::Level::EXPLOSIVE;
		else if(k == 4) board_[i][j].change = Block::Level::DIAMOND;//explo->diam
	}

	for(int j=0, k=0; j<BOARD_SZ; j++, k=0) for(int i=0; i<BOARD_SZ-1; i++) {
		if(board_[i][j] == board_[i+1][j]) k++;//horizontal scan
		else k = 0;
		if(k >= 2) remove(i+1, j, true), found = true;
		if(k == 2) remove(i-1, j, true), remove(i, j, true);
		else if(k == 3) {
			int max = 0, idx = 0;
			for(int n=-2; n<2; n++) {
				int diff = abs(board_[i+n][j].y - j);
				if(max < diff) max = diff, idx = n;
			}
			board_[i+idx][j].change = Block::Level::EXPLOSIVE;
		} else if(k == 4) {//horizontal 5 or more
			int max = 0, idx = 0;
			for(int n=1-k; n<2; n++) {
				int diff = abs(board_[i+n][j].y - j);
				if(max < diff) max = diff, idx = n;
			}
			board_[i+idx][j].change = Block::Level::DIAMOND;
		}
	}
	return found;
}

void Board::remove(int x, int y, bool cross)
{
	if(cross && board_[x][y].change != Block::Level::NORMAL)//for cross delete
		board_[x][y].change = Block::Level::DIAMOND;
	else board_[x][y].change = Block::Level::DELETE;
	if(board_[x][y].level == Block::Level::EXPLOSIVE) 
		for(int i : {-1, 1}) for(int j : {-1, 1})
			if(x+i >= 0 && x+i < BOARD_SZ && y+j >= 0 && y+j < BOARD_SZ) 
				remove(x+i, y+j, false);
	else if(board_[x][y].level == Block::Level::DIAMOND)
		for(int i=0; i<BOARD_SZ; i++) for(int j=0; j<BOARD_SZ; j++) 
			if(board_[i][j] == board_[x][y]) remove(i, j, false);
}

void Board::transform()
{
	for(int i=0,k=0; i<BOARD_SZ; i++,k=0) for(int j=0; j<BOARD_SZ; j++) {
		board_[i][j].level = board_[i][j].change;
		board_[i][j].change = Block::Level::NORMAL;
	}
}

void Board::drop()
{
	for(int i=0,k=0; i<BOARD_SZ; i++,k=0) for(int j=0; j<BOARD_SZ; j++) 
		if(board_[i][j].level == Block::Level::DELETE) 
			board_[i][j] = get_below(i, j+1);
}

Block Board::get_below(int x, int y)
{
	if(y == BOARD_SZ) return Block{};
	if(board_[x][y].level == Block::Level::DELETE) return get_below(x, y+1);
	else {
		auto b = board_[x][y];
		board_[x][y].level == Block::Level::DELETE;
		return b;
	}
}
