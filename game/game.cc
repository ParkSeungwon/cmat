#include<random>
#include"game.h"
using namespace std;

bool Block::operator==(const Block& r) const
{
	return color == r.color;
}

Block::Block()
{
	uniform_int_distribution<int> di{0, 4};
	random_device rd;
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

void Board::find_match()//rgbyd
{//find vertical or horizontal row with more than 3 -> mark that point
	for(int i=0, k=0; i<BOARD_SZ; i++, k=0) for(int j=0; j<BOARD_SZ-1; j++) {
		if(board_[i][j] == board_[i][j+1]) k++;//vertical scan
		else k = 0;
		if(k >= 2) remove(i, j+1, false);//remove last one always
		if(k == 2) for(int n=-1; n<1; n++) remove(i, j+n, false);//remove former 2
		else if(k == 3) board_[i][j+1].change_to = Block::Level::EXPLOSIVE;
		else if(k == 4) board_[i][j].change_to = Block::Level::DIAMOND;//explo->diam
	}

	for(int j=0, k=0; j<BOARD_SZ; j++, k=0) for(int i=0; i<BOARD_SZ-1; i++) {
		if(board_[i][j] == board_[i+1][j]) k++;//horizontal scan
		else k = 0;
		if(k == 2) for(int n=-1; n<2; n++) remove(i+n, j, true);
		else if(k == 3) {
			int max = 0, idx = 0;
			for(int n=-2; n<2; n++) {
				int diff = abs(board_[i+n][j].y - j);
				if(max < diff) max = diff, idx = n;
			}
			remove(i+1, j, true);
			board_[i+idx][j].change_to = Block::Level::EXPLOSIVE;
		} else if(k > 3) {
			int max = 0, idx = 0;
			for(int n=1-k; n<2; n++) {
				remove(i+n, j, true);
				int diff = abs(board_[i+n][j].y - j);
				if(max < diff) max = diff, idx = n;
			}
			board_[i+idx][j].change_to = Block::Level::DIAMOND;
		}
	}
}

void Board::remove(int x, int y, bool cross)
{
	if(cross && board_[x][y].change_to != Block::Level::NORMAL)//for cross delete
		board_[x][y].change_to = Block::Level::DIAMOND;
	else board_[x][y].change_to = Block::Level::DELETE;
	if(board_[x][y].level == Block::Level::EXPLOSIVE) 
		for(int i : {-1, 1}) for(int j : {-1, 1})
			if(x+i >= 0 && x+i < BOARD_SZ && y+j >= 0 && y+j < BOARD_SZ) 
				remove(x+i, y+j, false);
	else if(board_[x][y].level == Block::Level::DIAMOND)
		for(int i=0; i<BOARD_SZ; i++) for(int j=0; j<BOARD_SZ; j++) 
			if(board_[i][j] == board_[x][y]) remove(i, j, false);
}

void Board::delete_phase()
{
	for(int i=0; i<BOARD_SZ; i++) for(int j=0; j<BOARD_SZ; j++) {
		board_[i][j].level = board_[i][j].change_to;
//		switch(board_[i][j].change_to) {
//			case Block::Level::DELETE: board_.level = Block::Level::DELETE; break;
//		case Block::Level::EXPLOSIVE: break;
//		case Block::Level::DIAMOND: break;
//		default: break;
//		}
	}
}
