#include<catch.hpp>
#include<iostream>
#include"game/game.h"
using namespace std;

TEST_CASE("game") {
	Board board;
	board.find_match();
	cout << endl << board.board_ << endl;
	board.delete_phase();
	cout << endl << board.board_ << endl;
}

