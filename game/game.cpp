#include"game.h"
#include<iostream>
using namespace std;

int main()
{
	int x, y; char c;
	Board board;
	cout << endl << board << endl;
	while(1) {
		while(board.find_match()) {
			board.transform();
			cout << board << endl;
			while(board.step_drop()) cout << board << endl;
			board.turn_finish();
		}
		do {
			cout << board << endl;
			cout << "enter x, y coordinate and direction[rlud]" << endl;
			cout << "example : 0 5 r" << endl;
			cin >> x >> y >> c;
		} while(!board.swap(x, y, c));
	}
}
