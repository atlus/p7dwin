// ------------------------
// projects/darwin/main.c++
// Copyright (C) 2009
// Glenn P. Downing
// ------------------------

/*
 To run the tests:
 % g++ -ansi -pedantic -lcppunit -ldl -Wall -DTEST main.c++ -o main.app
 % valgrind main.app
 
 To run the program:
 % g++ -ansi -pedantic -Wall main.c++ -o main.app
 % valgrind main.app > Darwin.out
 
 To configure Doxygen:
 doxygen -g
 That creates the file Doxyfile.
 Make the following edits:
 EXTRACT_ALL            = YES
 EXTRACT_PRIVATE        = YES
 EXTRACT_STATIC         = YES
 GENERATE_LATEX         = NO
 
 To document the program:
 doxygen Doxyfile
 */

// --------
// includes
// --------

#include <cassert>   // assert
#include <iostream>  // cout, endl
#include <stdexcept> // invalid_argument, out_of_range


#ifdef TEST
#include "cppunit/TestSuite.h"      // TestSuite
#include "cppunit/TextTestRunner.h" // TestRunner

#include "TestDarwin.h"

#endif // TEST

//enum dir{
//	WEST, NORTH, SOUTH, EAST
//}

#define WEST 0
#define NORTH 1
#define EAST 2
#define SOUTH 3

//
// CREATURE.H
//

#include "Creature.h"
Creature::Creature(int pc, int dir, Species& spIn){
	programCount = pc;
	direction = dir;
	spointer = &spIn;
	moved = false;
}

Species* Creature::getSpecies(){
	return spointer;
}

char Creature::getSymbol(){
	if(spointer != NULL){
		return spointer->getSymbol();
	}else{
		return '.';
	}
}

int Creature::process(int data){
	//const char* command = spointer->commands[programCount];
	
	int returnInt = -1;
	while(moved == false){
		std::string command = spointer->commands[programCount];
		//std::cout << command << std::endl;
		returnInt = doCommand(command,data);
	}
	return returnInt;
	//return doCommand(command, data);
	//std::cout << command << std::endl;
	
}

int Creature::getDirection(){
	return direction;
}

int Creature::doCommand(std::string c, int data){
		int pos = int(c.find(" "));
		std::string left = c.substr(0, pos);
		std::string right= c.substr(pos + 1, c.size());
		//std::cout << "CREATURE----| Dir:" << direction << " Species:" << getSymbol() << std::endl;
		//std::cout << "LEFT:" << left << " RIGHT:" << right << " POS:" << pos << " DATA:" << data << std::endl;
		if(pos == -1){ // Perform Action
			if(left == "left"){
				if(direction == 0){
					direction = 3;
				}else{
					direction = direction - 1;
				}
				moved = true;
			}else if(left == "right"){
				if(direction == 3){
					direction = 0;
				}else{
					direction = direction + 1;
				}
				moved = true;
			}else if(left == "hop" && data == 1){
				moved = true;
				programCount++;
				return 0;	// tell darwin to hop
			}else if(left == "infect"){
				moved = true;
				programCount++;
				return 1;	// tell darwin to infect
			}
			moved = true;
			programCount++;
		}else{ // Perform Control
			int p = atoi(right.c_str());		
			if(left == "if_empty"){
				if(data == 1){
					programCount = p;
				}else{
					programCount++;				
				}
			}else if(left == "if_wall"){
				if(data == 0){
					programCount = p;
				}else{
					programCount++;				
				}
			}else if(left == "if_random"){
				if((rand() & 1) == 1){
					programCount = p;
				}else{
					programCount++;				
				}
			}else if(left == "if_enemy"){
				if(data == 3){
					programCount = p;
				}else{
					programCount++;				
				}
			}else if(left == "go"){
				programCount = p;
			}
		}
		return -1;	// if we did not hop or infect
}

void Creature::infect(Species* sp){
		programCount = 0;
		spointer = sp;
}

void Creature::setMoved(bool b){
	moved = b;
}

bool Creature::getMoved(){
	return moved;
}

//END
//CREATURE.H
//

//
// DARINBOARD.H
//

#include "DarwinBoard.h"
DarwinBoard::DarwinBoard(int inX, int inY){
	turn = 0;
	x = inX;
	y = inY;
	board.resize(x);
	
	//cout << board.capacity() << endl;
	//cout << board[0].capacity() << endl;
	
	for(int i = 0; i < x; i++){
		for(int j = 0; j < y; j++){
//			cout << j << endl;
			board[i].push_back(NULL);
		}
	}
}
void DarwinBoard::printBoard(){
	cout << "Turn = " << turn << "." << endl;
	cout << "  ";
	for(int i = 0; i < y; i++){
		cout<< i ;
	}
	cout << endl;
	for(int i = 0; i < x; i++){
		cout << i<< " ";
		for(int j = 0; j < y; j++){
			if(board[i][j] != NULL){
				cout << board[i][j]->getSymbol();
			}else{
				cout << ".";
			}
		}
		cout << endl;
		
	}
}

void DarwinBoard::putCreature(int newX, int newY, Creature& c){
	board[newX][newY] = &c;
}

void DarwinBoard::doTurn(){
	for(int i = 0; i < x; i++){
		for(int j = 0; j < y; j++){
			if(board[i][j] != NULL && !board[i][j]->getMoved()){
				int dir = board[i][j]->getDirection();
				int checkX = i;
				int checkY = j;
				switch(dir){
					case WEST:
						checkY-=1;
						break;
					case NORTH:
						checkX-=1;
						break;
					case EAST:
						checkY+=1;
						break;
					case SOUTH:
						checkX+=1;
						break;
				}
				int data = -1;
				if((checkX >= x || checkX < 0) || (checkY >= y || checkY < 0)){
					data = 0;	// wall ahead
				}else{
					//std::cout << "checkX:" << checkX << " checkY:" << checkY << std::endl;
					if(board[checkX][checkY] == NULL){
						data = 1; // empty ahead
					}else if(board[checkX][checkY]->getSymbol() == board[i][j]->getSymbol()){
						data = 2; // same species ahead					
					}else{
						data = 3; // different species ahead
					}
				}
				int res = board[i][j]->process(data);
				if(res == 0){	// hop
					board[checkX][checkY] = board[i][j];
					board[i][j] = NULL;
				}else if(res == 1){ // infect
					board[checkX][checkY]->infect(board[i][j]->getSpecies());
				}
			}	
		}	
	}
	for(int r = 0; r < x; r++){
		for(int c = 0; c < y; c++){
			if(board[r][c] != NULL){
				board[r][c]->setMoved(false);
			}		
		}	
	}
	++turn;
}

void DarwinBoard::run(int times){
	while(--times >= 0){
		printBoard();
		doTurn();
	}
	printBoard();
}
// END
// DARINBOARD.H
//

//
//SPECIES.H
//
char Species::getSymbol(){
 return '.';
}

//
//FOOD.H
//
#include "Food.h"
Food::Food(){
commands[0] = "left";
commands[1] = "go 0";
}

char Food::getSymbol(){
	return 'f';
}
//END
//FOOD.H
//

//
//HOPPER.H
//
#include "Hopper.h"
Hopper::Hopper(){
commands[0] = "hop";
commands[1] = "go 0";
}

char Hopper::getSymbol(){
	return 'h';
}
//END
//HOPPER.H
//

//
//ROVER.H
//
#include "Rover.h"
Rover::Rover(){
	 commands[0] = "if_enemy 9";
	 commands[1] = "if_empty 7";
	 commands[2] = "if_random 5";
	 commands[3] = "left";
	 commands[4] = "go 0";
	 commands[5] = "right";
	 commands[6] = "go 0";
	 commands[7] = "hop";
	 commands[8] = "go 0";
	 commands[9] = "infect";
	 commands[10] =  "go 0";
}

char Rover::getSymbol(){
	return 'r';
}
//END
//ROVER.H
//

//
//TRAP.H
//
#include "Trap.h"
Trap::Trap(){
	 commands[0] = "if_enemy 3";
	 commands[1] = "left";
	 commands[2] = "go 0";
	 commands[3] = "infect";
	 commands[4] = "go 0";
}

char Trap::getSymbol(){
	return 't';
}
//END
//TRAP.H
//

// ----
// main
// ----

int main () {
    using namespace std;
    ios_base::sync_with_stdio(false); // turn off synchronization with C I/O
	
#ifdef TEST
	// ----------
	// unit tests
	// ----------
	
	CppUnit::TextTestRunner tr;
	tr.addTest(cs::TestDarwin::suite());
	tr.run();
#else
	// -------
	// program
	// -------
	
	// ----
	// food
	// ----
	
	/*
	 0: left
	 1: go 0
	 */
	
	// ------
	// hopper
	// ------
	
	/*
	 0: hop
	 1: go 0
	 */
	
	// -----
	// rover
	// -----
	
	/*
	 0: if_enemy 9
	 1: if_empty 7
	 2: if_random 5
	 3: left
	 4: go 0
	 5: right
	 6: go 0
	 7: hop
	 8: go 0
	 9: infect
	 10: go 0
	 */
	
	// ----
	// trap
	// ----
	
	/*
	 0: if_enemy 3
	 1: left
	 2: go 0
	 3: infect
	 4: go 0
	 */
	
	// ----------
	// darwin 8x8
	// ----------
	Food f;
	Hopper h;
	Trap t;
	Rover r;
	//Best b;
	try {
		cout << "*** Darwin 8x8 ***" << endl;
		DarwinBoard eightbyeight(8,8);
		//eightbyeight.printBoard();
		Creature f1(0, EAST, f);
		Creature f2(0, NORTH, f);
		Creature h1(0, NORTH, h);
		Creature h2(0, EAST, h);
		Creature h3(0, SOUTH, h);
		Creature h4(0, WEST, h);
		eightbyeight.putCreature(0, 0, f1);
		eightbyeight.putCreature(7, 7, f2);
		eightbyeight.putCreature(3, 3, h1);
		eightbyeight.putCreature(3, 4, h2);
		eightbyeight.putCreature(4, 4, h3);
		eightbyeight.putCreature(4, 3, h4);
		eightbyeight.run(5);
		/*
		 8x8 Darwin
		 Food,   facing east,  at (0, 0)
		 Hopper, facing north, at (3, 3)
		 Hopper, facing east,  at (3, 4)
		 Hopper, facing south, at (4, 4)
		 Hopper, facing west,  at (4, 3)
		 Food,   facing north, at (7, 7)
		 Simulate 5 moves.
		 Print every grid.
		 */
	}
	catch (const invalid_argument&) {
		assert(false);}
	catch (const out_of_range&) {
		assert(false);}
	
	// ----------
	// darwin 7x9
	// ----------
	
	try {
		cout << "*** Darwin 7x9 ***" << endl;
		DarwinBoard sevenbynine(7,9);
		Creature t1(0, SOUTH, t);
		Creature h1(0, EAST, h);
		Creature r1(0, NORTH, r);
		Creature t2(0, WEST, t);
		sevenbynine.putCreature(0, 0, t1);
		sevenbynine.putCreature(3, 2, h1);
		sevenbynine.putCreature(5, 4, r1);
		sevenbynine.putCreature(6, 8, t2);
		sevenbynine.run(5);
		/*
		 7x9 Darwin
		 Trap,   facing south, at (0, 0)
		 Hopper, facing east,  at (3, 2)
		 Rover,  facing north, at (5, 4)
		 Trap,   facing west,  at (6, 8)
		 Simulate 5 moves.
		 Print every grid.
		 */
	}
	catch (const invalid_argument&) {
		assert(false);}
	catch (const out_of_range&) {
		assert(false);}
	
	// ------------
	// darwin 72x72
	// without best
	// ------------
	
	try {
		cout << "*** Darwin 72x72 without Best ***" << endl;
		/*
		 Randomly place the following creatures facing randomly.
		 Call rand(), mod it with 5184 (72x72), and use that for the position
		 in a row-major order grid.
		 Call rand() again, mod it with 4 and use that for it's direction with
		 the ordering: 0 = west, 1 = north, 2 = east, 3 = south.
		 Do that for each kind of creature.
		 10 Food
		 10 Hopper
		 10 Rover
		 10 Trap
		 Simulate 1000 moves.
		 Print every 100th grid.
		 */
	}
	catch (const invalid_argument&) {
		assert(false);}
	catch (const out_of_range&) {
		assert(false);}
	
	// ------------
	// darwin 72x72
	// with best
	// ------------
	
	try {
		cout << "*** Darwin 72x72 with Best ***" << endl;
		/*
		 Randomly place the following creatures facing randomly.
		 Call rand(), mod it with 5184 (72x72), and use that for the position
		 in a row-major order grid.
		 Call rand() again, mod it with 4 and use that for it's direction with
		 the ordering: west, north, east, south.
		 Do that for each kind of creature.
		 10 Food
		 10 Hopper
		 10 Rover
		 10 Trap
		 10 Best
		 Simulate 1000 moves.
		 Print every 100th grid.
		 */
	}
	catch (const invalid_argument&) {
		assert(false);}
	catch (const out_of_range&) {
		assert(false);}
#endif // NDEBUG
	
    return 0;}
