#pragma once
#include<ostream>
#include<random>
#include"matrix/base.h"
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
	int get_color(int x, int y) const;
	void score();
protected:
	int score_ = 0;
	Cmat<Block, BOARD_SZ, BOARD_SZ> board_, backup_;//board
private:
	void remove(int x, int y);
	void trigger(int x, int y);
	friend std::ostream& operator<<(std::ostream& o, const Board& b);
	bool is_valid(int x, int y) const;
};



