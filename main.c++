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
#include <sstream>
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

// wall 0
// empty 1
// same 2
// diff 3

// hop 0
// infect 1

//
// CREATURE.H
//

#include "Creature.h"
/**
 * Creature Constructor
 * @pc Creatures Program Counter
 * @dir Creatures direction
 * @spIn Species pointer
 */
Creature::Creature(int pc, int dir, Species& spIn){
	programCount = pc;
	direction = dir;
	spointer = &spIn;
	moved = false;
}
/**
 * @return Returns the creatures species pointer
 */
Species* Creature::getSpecies(){
	return spointer;
}
/**
 * @return Returns the creatures species symbol or '.' if it doesn't have one
 */
char Creature::getSymbol(){
	if(spointer != NULL){
		return spointer->getSymbol();
	}else{
		return '.';
	}
}
/**
 * Processes the current command
 * @data Given to us to help process the command
 * @return Returns information to darwin
 */
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
/**
 * @return Returns the creatures current direction
 */
int Creature::getDirection(){
	return direction;
}
/**
 * Carrys out the command
 * @c Command
 * @data Data used to carry out command
 * @return Returns int to darwin
 */
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
			}else if(left == "infect" && data == 3){
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
/**
 * We have been infected, method changes us to the other species
 * @sp Species to change us to
 */
void Creature::infect(Species* sp){
		//std::cout << spointer << " " << sp->getSymbol() << std::endl;
		programCount = 0;
		spointer = sp;
}
/**
 * Set moved
 * b used to change value
 */
void Creature::setMoved(bool b){
	moved = b;
}
/**
 * @return Returns the state of if we have moved
 */
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
/**
 * Darwin Board Constructor
 * @inX X size of the board
 * @inY Y size of the board
 */
DarwinBoard::DarwinBoard(int inX, int inY){
	turn = 0;
	x = inX;
	y = inY;
	bestCounter = 0;
	roverCounter = 0;
	totalC = 0;
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
/**
 * Prints out the board
 */
void DarwinBoard::printBoard(){
	//cout << "Turn = " << turn << ". TOTAL:" << totalC << " Best:" << bestCounter << " Rover:" << roverCounter << endl;
	cout << "Turn = " << turn <<endl;
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
/**
 * ToString of the board
 * @return the board in string form
 */
std::string DarwinBoard::boardToString(){
  ostringstream oss;
	//oss << "Turn = " << turn << ". TOTAL:" << totalC << " Best:" << bestCounter << " Rover:" << roverCounter << endl;
	oss << "Turn = " << turn <<endl;
	oss << "  ";
	for(int i = 0; i < y; i++){
		oss<< i ;
	}
	oss << endl;
	for(int i = 0; i < x; i++){
		oss << i<< " ";
		for(int j = 0; j < y; j++){
			if(board[i][j] != NULL){
				oss << board[i][j]->getSymbol();
			}else{
				oss << ".";
			}
		}
		oss << endl;
	}
	return oss.str();
}
/**
 * Places a creature on the board
 * @newX Place the creature's X here
 * @newY Place the creature's Y here
 * @c Creature to be placed
 * @return Returns true if placed on the board, else false
 */
bool DarwinBoard::putCreature(int newX, int newY, Creature& c){
	if(board[newX][newY] == NULL){
		board[newX][newY] = &c;
		totalC++;
		return true;
	}else{
		return false;
	}
}
/**
 * Processes the next Turn
 */
void DarwinBoard::doTurn(){
	bestCounter = 0;
	roverCounter = 0;
	//std::cout << "-----------------------------" << std::endl;
	for(int i = 0; i < x; i++){
		for(int j = 0; j < y; j++){
			if(board[i][j] != NULL && !board[i][j]->getMoved()){
				//std::cout << board[i][j]->getSymbol() << " " << i << "," << j << std::endl;
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
						//std::cout << "empty" << std::endl;
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
	//std::cout << "-----------------------------" << std::endl;
	for(int r = 0; r < x; r++){
		for(int c = 0; c < y; c++){
			if(board[r][c] != NULL){
			   //if(board[r][c] -> getSymbol() == 'b')
					//std::cout << board[r][c] << std::endl;
				board[r][c]->setMoved(false);
				if(board[r][c]->getSymbol() == 'b') bestCounter++;
				if(board[r][c]->getSymbol() == 'r') roverCounter++;
			}
		}
	}
	++turn;
}
/**
 * Runs the board and prints it out
 * @times Number of times to run the board
 * @print Prints the board
 */
void DarwinBoard::run(int times, int print){
	printBoard();
	while(--times >= 0){
		doTurn();
		if(times % print == 0){
			if(bestCounter == 0){
				//std::cout << "Best is dead!" << std::endl;
				//times = -1;
			}
			printBoard();
		}
	}
	//printBoard();
}
// END
// DARINBOARD.H
//

//
//SPECIES.H
//
/**
 * @return Returns a blank symbol
 */
char Species::getSymbol(){
 return '.';
}

//
//FOOD.H
//
#include "Food.h"
/**
 * Food Constructor
 */
Food::Food(){
commands[0] = "left";
commands[1] = "go 0";
}
/**
 * @return Food Symbol
 */
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
/**
 * Hopper Constructor
 */
Hopper::Hopper(){
commands[0] = "hop";
commands[1] = "go 0";
}
/**
 * @return Hopper Symbol
 */
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
/**
 * Rover Constructor
 */
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
/**
 * @return Hopper Symbol
 */
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
/**
 * Trap Constructor
 */
Trap::Trap(){
	 commands[0] = "if_enemy 3";
	 commands[1] = "left";
	 commands[2] = "go 0";
	 commands[3] = "infect";
	 commands[4] = "go 0";
}
/**
 * @return Trap Symbol
 */
char Trap::getSymbol(){
	return 't';
}
//END
//TRAP.H
//

//
//BEST.H
//
#include "Best.h"
/**
 * Best Constructor
 */
Best::Best(){
	/*
	commands[0] = "if_enemy 8";
	commands[1] = "if_empty 3";
	commands[2] = "if_wall 5";
	commands[3] = "hop";
	commands[4] = "go 0";
	commands[5] = "if_random 9";
	commands[6] = "left";
	commands[7] = "go 0";
	commands[8] = "infect";
	commands[9] = "go 0";
	commands[10] = "right";
	commands[11] = "go 0";
	*/
	/*
	 commands[0] = "if_wall 8";
	 commands[1] = "if_enemy 11";
	 commands[2] = "left";
	 commands[3] = "if_enemy 11";
	 commands[4] = "left";
	 commands[5] = "if_enemy 11";
	 commands[6] = "left";
	 commands[7] = "if_enemy 11";
	 commands[8] = "left";
	 commands[9] = "hop";
	 commands[10] =  "if_wall 8";
	 commands[11] = "infect";
	 commands[12] = "go 0";
	 */
	 commands[0] = "if_wall 8";
	 commands[1] = "if_enemy 12";
	 commands[2] = "left";
	 commands[3] = "if_enemy 12";
	 commands[4] = "left";
	 commands[5] = "if_enemy 12";
	 commands[6] = "left";
	 commands[7] = "if_enemy 12";
	 commands[8] = "left";
	 commands[9] = "if_wall 14";
	 commands[10] = "hop";
	 commands[11] = "go 0";
	 commands[12] = "infect";
	 commands[13] = "go 0";
	 commands[14] = "right";
	 commands[15] = "go 9";

}
/**
 * @return Best Symbol
 */
char Best::getSymbol(){
	return 'b';
}
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
	Best b;
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
		eightbyeight.run(5, 1);
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
		sevenbynine.run(5, 1);
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
		DarwinBoard s2bys2(72,72);
		vector<Creature> fcreatures;
		int row = -1;
		int col = -1;
  		for (int i = 0; i < 10; ++i) {
			int dir = rand() % 4;
    		fcreatures.push_back(Creature(0, dir, f));
  		}
  		for(int i = 0; i < 10; i++){
  			do{
  		  		int rd = rand() % 5184;
				row = rd / 72;
				col = rd % 72;
			}while(!s2bys2.putCreature(row, col, fcreatures[i]));
  		}
  		vector<Creature> hcreatures;
  		for (int i = 0; i < 10; ++i) {
			int dir = rand() % 4;
    		hcreatures.push_back(Creature(0, dir, h));
  		}
  		for(int i = 0; i < 10; i++){
  			do{
  		  		int rd = rand() % 5184;
				row = rd / 72;
				col = rd % 72;
			}while(!s2bys2.putCreature(row, col, hcreatures[i]));
  		}
		vector<Creature> rcreatures;
		for (int i = 0; i < 10; ++i) {
			int dir = rand() % 4;
    		rcreatures.push_back(Creature(0, dir, r));
  		}
  		for(int i = 0; i < 10; i++){
  			do{
  		  		int rd = rand() % 5184;
				row = rd / 72;
				col = rd % 72;
			}while(!s2bys2.putCreature(row, col, rcreatures[i]));
  		}
  		vector<Creature> tcreatures;
  		for (int i = 0; i < 10; ++i) {
			int dir = rand() % 4;
    		tcreatures.push_back(Creature(0, dir, t));
  		}
  		for(int i = 0; i < 10; i++){
  			do{
  		  		int rd = rand() % 5184;
				row = rd / 72;
				col = rd % 72;
			}while(!s2bys2.putCreature(row, col, tcreatures[i]));
  		}
		s2bys2.run(1000, 100);
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
		DarwinBoard s2bys2(72,72);
		vector<Creature> fcreatures;
		int row = -1;
		int col = -1;
  		for (int i = 0; i < 10; ++i) {
			int dir = rand() % 4;
    		fcreatures.push_back(Creature(0, dir, f));
  		}
  		for(int i = 0; i < 10; i++){
  			do{
  		  		int rd = rand() % 5184;
				row = rd / 72;
				col = rd % 72;
			}while(!s2bys2.putCreature(row, col, fcreatures[i]));
  		}
  		vector<Creature> hcreatures;
  		for (int i = 0; i < 10; ++i) {
			int dir = rand() % 4;
    		hcreatures.push_back(Creature(0, dir, h));
  		}
  		for(int i = 0; i < 10; i++){
  			do{
  		  		int rd = rand() % 5184;
				row = rd / 72;
				col = rd % 72;
			}while(!s2bys2.putCreature(row, col, hcreatures[i]));
  		}
		vector<Creature> rcreatures;
		for (int i = 0; i < 10; ++i) {
			int dir = rand() % 4;
    		rcreatures.push_back(Creature(0, dir, r));
  		}
  		for(int i = 0; i < 10; i++){
  			do{
  		  		int rd = rand() % 5184;
				row = rd / 72;
				col = rd % 72;
			}while(!s2bys2.putCreature(row, col, rcreatures[i]));
  		}
  		vector<Creature> tcreatures;
  		for (int i = 0; i < 10; ++i) {
			int dir = rand() % 4;
    		tcreatures.push_back(Creature(0, dir, t));
  		}
  		for(int i = 0; i < 10; i++){
  			do{
  		  		int rd = rand() % 5184;
				row = rd / 72;
				col = rd % 72;
			}while(!s2bys2.putCreature(row, col, tcreatures[i]));
  		}
  		vector<Creature> bcreatures;

  		for (int i = 0; i < 10; ++i) {
			int dir = rand() % 4;
    		bcreatures.push_back(Creature(0, dir, b));
  		}
  		for(int i = 0; i < 10; i++){
  			do{
  		  		int rd = rand() % 5184;
				row = rd / 72;
				col = rd % 72;
			}while(!s2bys2.putCreature(row, col, bcreatures[i]));
  		}
		s2bys2.run(1000, 100);
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
