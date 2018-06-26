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
private:
	static std::uniform_int_distribution<int> di;
	static std::random_device rd;
};
std::ostream& operator<<(std::ostream& o, const Block& b);

class Board
{
public:
	Board();
	Cmat<Block, BOARD_SZ, BOARD_SZ> board_;//board
	bool find_match();
	void drop();
	void transform();
private:
	void remove(int x, int y, bool cross);
	Block get_below(int x, int y);
};



