#pragma once
#include<ostream>
#include"base.h"
#define BOARD_SZ 7

struct Block
{
	Block();
	bool operator==(const Block& r) const;
	enum Color { RED, GREEN, BLUE, GOLD, BLACK} color;
	enum Level { 
		DELETE, NORMAL, EXPLOSIVE, DIAMOND
	} level = NORMAL, change_to = NORMAL;
	int x, y;
};
std::ostream& operator<<(std::ostream& o, const Block& b);

class Board
{
public:
	Cmat<Block, BOARD_SZ, BOARD_SZ> board_;//board
	void find_match();
	void delete_phase();
private:
	void remove(int x, int y, bool cross);
};



