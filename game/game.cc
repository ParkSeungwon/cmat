#include<utility>
#include<iostream>
#include"game.h"
using namespace std;

bool Block::operator==(const Block& r) const
{
	return color == r.color;
}

static uniform_int_distribution<int> di{0,4};
static random_device rd;

Block::Block()
{
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
		case Block::Level::DIAMOND: c = static_cast<char>(b.color) + '1'; break;
		case Block::Level::DELETE: c = ' '; break;
		default: break;
	}					   
	o << c;
	return o;
}

ostream& operator<<(ostream& o, const Board& b)
{
	o << b.board_;
	return o;
}

Board::Board()
{
	turn_finish();
}

void Board::turn_finish()
{
	for(int i=0, k=0; i<BOARD_SZ; i++, k=0) for(int j=0; j<BOARD_SZ-1; j++) 
		board_[i][j].x = i, board_[i][j].y = j;
	cout << "-----------------------------------" << endl;
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
	if(!is_valid(x, y)) return;
	if(cross && board_[x][y].change != Block::Level::NORMAL)//for cross delete
		board_[x][y].change = Block::Level::DIAMOND;//generate diamond
	else board_[x][y].change = Block::Level::DELETE;//remove block
	if(board_[x][y].level == Block::Level::EXPLOSIVE) {
		board_[x][y].level = Block::Level::NORMAL;
		for(int i : {-1, 1}) remove(x+i, y, false), remove(x, y+i, false);
	} else if(board_[x][y].level == Block::Level::DIAMOND) {
		board_[x][y].level = Block::Level::NORMAL;
		for(int i=0; i<BOARD_SZ; i++) for(int j=0; j<BOARD_SZ; j++) 
			if(board_[i][j] == board_[x][y]) remove(i, j, false);
	}
}

void Board::transform()
{
	for(int i=0,k=0; i<BOARD_SZ; i++,k=0) for(int j=0; j<BOARD_SZ; j++) {
		if(board_[i][j].change != Block::Level::NORMAL)//if changed
			board_[i][j].level = board_[i][j].change;
		board_[i][j].change = Block::Level::NORMAL;
	}
}

bool Board::step_drop()
{
	bool blank = false;
	for(int i=0,k=0; i<BOARD_SZ; i++,k=0) for(int j=0; j<BOARD_SZ; j++) 
		if(board_[i][j].level == Block::Level::DELETE) {
			for(int k=j; k<BOARD_SZ-1; k++) board_[i][k] = board_[i][k+1];
			board_[i][BOARD_SZ-1] = Block{};
			blank = true;
			break;
		}
	return blank;
}

bool Board::swap(int x, int y, char c)
{
	int x2 = x, y2 = y;
	switch(c) {
		case 'r': x2++; break;
		case 'l': x2--; break;
		case 'u': y2--; break;
		case 'd': y2++; break;
		default: return false;
	}
	if(!is_valid(x, y) || !is_valid(x2, y2)) return false;
	bool ok = false;
	std::swap(board_[x][y], board_[x2][y2]);
	backup_ = board_;
	if(find_match()) ok = true;
	board_ = backup_;
	if(ok) return true;
	else {
		std::swap(board_[x][y], board_[x2][y2]);//recover
		return false;
	}
}
		
bool Board::is_valid(int x, int y) const {
	if(x >= 0 && x < BOARD_SZ && y >= 0 && y < BOARD_SZ) return true;
	else return false;
}
