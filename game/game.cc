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
{//random generation of a block
	color = static_cast<Color>(di(rd));
}

ostream& operator<<(ostream& o, const Block& b)
{
	char c; 
	switch(b.color) {
		case Block::Color::RED: 	c = 'r'; break;
		case Block::Color::GREEN: 	c = 'g'; break;
		case Block::Color::BLUE: 	c = 'b'; break;
		case Block::Color::GOLD: 	c = 'l'; break;
		case Block::Color::BLACK: 	c = 'd'; break;
	}
	switch(b.level) {
		case Block::Level::EXPLOSIVE: 	c = toupper(c); 						break;
		case Block::Level::DIAMOND: 	c = static_cast<char>(b.color) + '1'; 	break;
		case Block::Level::DELETE: 		c = ' '; 								break;
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
{//initialize board with no match
	turn_finish();
	while(find_match()) {
		transform();
		step_drop();
		turn_finish();
	}
	for(int i=0; i<BOARD_SZ; i++) for(int j=0; j<BOARD_SZ; j++) 
		board_[i][j].level = Block::Level::NORMAL;
	turn_finish();
	score_ = 0;
}

void Board::score()
{//count removed block
	static float sum = 0;
	static int k = 0;
	if(k) {
		cout << "turn : " << k << endl << "score : " << score_ << endl;
		sum += score_;
		cout << "avg : " << sum / k++ << endl;
		score_ = 0;
	} else k++;
}
	
int Board::get_color(int x, int y) const
{//get attribute of block x, y
	if(board_[x][y].level == Block::Level::DELETE) return 5;
	else {
		int r = static_cast<int>(board_[x][y].color);
		if(board_[x][y].level == Block::Level::EXPLOSIVE) r += 6;
		if(board_[x][y].level == Block::Level::DIAMOND) r += 12;
		return r;
	}
}

void Board::turn_finish()
{
	for(int i=0, k=0; i<BOARD_SZ; i++, k=0) for(int j=0; j<BOARD_SZ-1; j++) 
		board_[i][j].x = i, board_[i][j].y = j;
	cout << "---------------------------------" << endl;
}

bool Board::find_match()//rgbyd
{//find vertical or horizontal row with more than 3 -> mark that point
	bool found = false;
	for(int i=0, k=0; i<BOARD_SZ; i++, k=0) for(int j=0; j<BOARD_SZ-1; j++) {
		if(board_[i][j] == board_[i][j+1]) k++;//vertical scan
		else k = 0;
		if(k >= 2) remove(i, j+1), found = true;//remove last one always
		if(k == 2) for(int n=-1; n<1; n++) remove(i, j+n);//remove former 2
		else if(k == 3) board_[i][j+1].change = Block::Level::EXPLOSIVE;
		else if(k == 4) board_[i][j].change = Block::Level::DIAMOND;//explo->diam
	}

	for(int j=0, k=0; j<BOARD_SZ; j++, k=0) for(int i=0; i<BOARD_SZ-1; i++) {
		if(board_[i][j] == board_[i+1][j]) k++;//horizontal scan
		else k = 0;
		if(k >= 2) remove(i+1, j), found = true;//11R1 bug
		if(k == 2) remove(i-1, j), remove(i, j);
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

void Board::remove(int x, int y)
{
	board_[x][y].change = board_[x][y].change == Block::Level::DELETE ? 
		Block::Level::DIAMOND : Block::Level::DELETE;
}

void Board::trigger(int x, int y)
{//recursive removing of blocks
	if(!is_valid(x, y)) return;
	if(board_[x][y].change == Block::Level::NORMAL)
		board_[x][y].change = Block::Level::DELETE;//remove block
	if(board_[x][y].level == Block::Level::EXPLOSIVE) {
		board_[x][y].level = Block::Level::NORMAL;
		for(int i : {-1, 1}) trigger(x+i, y), trigger(x, y+i);
	} else if(board_[x][y].level == Block::Level::DIAMOND) {
		board_[x][y].level = Block::Level::NORMAL;
		for(int i=0; i<BOARD_SZ; i++) for(int j=0; j<BOARD_SZ; j++) 
			if(board_[i][j] == board_[x][y]) trigger(i, j);
	}
}

void Board::transform()
{//remove or change blocks after settlement
	for(int i=0,k=0; i<BOARD_SZ; i++,k=0) for(int j=0; j<BOARD_SZ; j++) 
		if(board_[i][j].change != Block::Level::NORMAL) trigger(i, j);
	for(int i=0,k=0; i<BOARD_SZ; i++,k=0) for(int j=0; j<BOARD_SZ; j++) {
		if(board_[i][j].change != Block::Level::NORMAL)//if changed
			board_[i][j].level = board_[i][j].change, score_++;
		board_[i][j].change = Block::Level::NORMAL;
	}
}

bool Board::step_drop()
{//go up, influenced by puzzle empire
	bool blank = false;
	for(int i=0,k=0; i<BOARD_SZ; i++,k=0) for(int j=0; j<BOARD_SZ; j++) if(board_[i][j].level == Block::Level::DELETE) {
		for(int k=j; k<BOARD_SZ-1; k++) board_[i][k] = board_[i][k+1];
		board_[i][BOARD_SZ-1] = Block{};
		blank = true;
		break;
	}
	return blank;
}

bool Board::swap(int x, int y, char c)
{//drag a piece to change position
	int x2 = x, y2 = y;
	switch(c) {
		case 'd': x2++; break;
		case 'a': x2--; break;
		case 'w': y2--; break;
		case 's': y2++; break;
		default: return false;
	}
	if(!is_valid(x, y) || !is_valid(x2, y2)) return false;

	std::swap(board_[x][y], board_[x2][y2]);
	backup_ = board_;
	bool is_legal_move = find_match();
	board_ = backup_;
	if(is_legal_move) return true;
	else {
		std::swap(board_[x][y], board_[x2][y2]);//recover
		return false;
	}
}
		
bool Board::is_valid(int x, int y) const
{//border check
	if(x >= 0 && x < BOARD_SZ && y >= 0 && y < BOARD_SZ) return true;
	else return false;
}
