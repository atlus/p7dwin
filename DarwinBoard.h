#ifndef DARWINBOARD_H
#define DARWINBOARD_H
#include <vector>
using namespace std;
class DarwinBoard
{
private:
    int x;
	int y;
	int turn;
	vector<vector<char> > board;
    DarwinBoard() { } // private default constructor
	
public:
    DarwinBoard(int inX, int inY);
	void printBoard();
			
};

#endif