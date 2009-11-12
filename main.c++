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
			board[i].push_back('.');
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
			cout << board[i][j];
		}
		cout << endl;
		
	}
}
// END
// DARINBOARD.H
//

//
//FOOD.H
//
#include "Food.h"
Food::Food(){
commands[0] = "left";
commands[1] = "go 0";
}
//END
//FOOD.H
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
		eightbyeight.printBoard();
		Food f;
		cout << f.commands[1] << endl;
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
		 the ordering: west, north, east, south.
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
