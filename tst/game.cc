#include<catch.hpp>
#include<iostream>
#include"game/game.h"
using namespace std;

TEST_CASE("game") {
	Board board;
	cout << endl << board.board_ << endl;
	while(board.find_match()) {
		board.transform();
		cout << board.board_ << endl;
		board.drop();
		cout << board.board_ << endl;
	}
}

