#ifndef DARWINBOARD_H
#define DARWINBOARD_H
#include <vector>
#include "Creature.h"
using namespace std;
class DarwinBoard
{
private:
   int x;
	int y;
	int turn;
	int bestCounter;
	int roverCounter;
	int totalC;
	vector<vector<Creature*> > board;
   DarwinBoard() { } // private default constructor
	
public:
   DarwinBoard(int inX, int inY);
	void printBoard();
	bool putCreature(int newX, int newY, Creature& c);
	void doTurn();
	void run(int times, int print);
};

#endif