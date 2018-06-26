#pragma once
#include<ostream>
#include<random>
#include"base.h"
#define BOARD_SZ 7

class Block
{
public:
	Block();
	bool operator==(const Block& r) const;
	enum Color { RED, GREEN, BLUE, GOLD, BLACK} color;
	enum Level { DELETE, NORMAL, EXPLOSIVE, DIAMOND } level = NORMAL, change = NORMAL;
	int x, y;
};
std::ostream& operator<<(std::ostream& o, const Block& b);

class Board
{
public:
	Board();
	bool find_match();
	bool step_drop();
	void transform();
	void turn_finish();
private:
	Cmat<Block, BOARD_SZ, BOARD_SZ> board_;//board
	void remove(int x, int y, bool cross);
	friend std::ostream& operator<<(std::ostream& o, const Board& b);
};



