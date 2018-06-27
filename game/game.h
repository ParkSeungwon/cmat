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
	bool swap(int x, int y, char c);
protected:
	Cmat<Block, BOARD_SZ, BOARD_SZ> board_;//board
private:
	void remove(int x, int y, bool cross);
	bool check(int x, int y, Block b) const;//check del if moved to x, y 
	friend std::ostream& operator<<(std::ostream& o, const Board& b);
	bool is_valid(int x, int y) const;
	int l_check(int x, int y, Block b) const, r_check(int x, int y, Block b) const,
		u_check(int x, int y, Block b) const, d_check(int x, int y, Block b) const;
};



