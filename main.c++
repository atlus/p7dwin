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
#define SOUTH 2
#define EAST 3

//
// CREATURE.H
//

#include "Creature.h"
Creature::Creature(int pc, int dir, Species& spIn){
	programCount = pc;
	direction = dir;
	spointer = &spIn;
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

void Creature::process(int data){
	//const char* command = spointer->commands[programCount];
	std::string command = spointer->commands[programCount];
	doCommand(command, data);
	//std::cout << command << std::endl;
	
}

int Creature::getDirection(){
	return direction;
}

void Creature::doCommand(std::string c, int data){
		int pos = int(c.find(" "));
		std::string left = c.substr(0, pos);
		std::string right= c.substr(pos + 1, c.size());
		//std::cout << "CREATURE----| Dir:" << direction << " Species:" << getSymbol() << std::endl;
		//std::cout << "LEFT:" << left << " RIGHT:" << right << " POS:" << pos << " DATA:" << data << std::endl;
		if(pos == -1){ // Perform Action
			if(left == "left"){
			
			}else if(left == "right"){
		
			}else if(left == "hop"){
		
			}else if(left == "infect"){
			}
			programCount++;
		}else{ // Perform Control
			int p = atoi(right.c_str());		
			if(left == "if_empty"){
				programCount = p;
			}else if(left == "if_wall"){
				programCount = p;
			}else if(left == "if_random"){
				
			}else if(left == "if_enemy"){
				programCount = p;
			}else if(left == "go"){
				programCount = p;
			}
		}
}
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
			if(board[i][j] != NULL){
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
				board[i][j]->process(data);
			}	
		}	
	}
	++turn;
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
	
	try {
		cout << "*** Darwin 8x8 ***" << endl;
		DarwinBoard eightbyeight(8,8);
		//eightbyeight.printBoard();
		Food f;
		Hopper h;
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
		eightbyeight.printBoard();
		eightbyeight.doTurn();
		eightbyeight.printBoard();
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
		sevenbynine.printBoard();
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
