// ----------------------------
// projects/darwin/TestDarwin.h
// Copyright (C) 2009
// Glenn P. Downing
// ----------------------------

#ifndef TestDarwin_h
#define TestDarwin_h

// --------
// includes
// --------

#include "cppunit/TestFixture.h"             // TestFixture
#include "cppunit/extensions/HelperMacros.h" // CPPUNIT_TEST, CPPUNIT_TEST_SUITE, CPPUNIT_TEST_SUITE_END
#include "DarwinBoard.h"
#include "Creature.h"
#include "Food.h"
#include "Rover.h"
// ----------
// namespaces
// ----------

namespace cs {
	
	// ----------
	// TestDarwin
	// ----------
	
	struct TestDarwin : CppUnit::TestFixture {
		// -----
		// tests
		// -----
		
		void testInit1(){
		 DarwinBoard fourbyfour(4,4);
		 ostringstream oss;
		 oss << "Turn = 0";
		 oss << endl;
		 oss << "  0123" << endl;
		 oss << "0 ...." << endl;
		 oss << "1 ...." << endl;
		 oss << "2 ...." << endl;
		 oss << "3 ...." << endl;
		 CPPUNIT_ASSERT(oss.str() == fourbyfour.boardToString());
		}
    void testInit2(){
		 DarwinBoard onebyfour(1,4);
		 ostringstream oss;
		 oss << "Turn = 0";
		 oss << endl;
		 oss << "  0123" << endl;
		 oss << "0 ...." << endl;
		 CPPUNIT_ASSERT(oss.str() == onebyfour.boardToString());
		}
    void testInit3(){
		 DarwinBoard fourbyfourwithFood(4,4);
		 Food f;
		 Creature c(0,0,f);
		 fourbyfourwithFood.putCreature(1,1,c);
		 ostringstream oss;
		 oss << "Turn = 0";
		 oss << endl;
		 oss << "  0123" << endl;
		 oss << "0 ...." << endl;
		 oss << "1 .f.." << endl;
		 oss << "2 ...." << endl;
		 oss << "3 ...." << endl;
		 CPPUNIT_ASSERT(oss.str() == fourbyfourwithFood.boardToString());
		}
    void testInit4(){
		 DarwinBoard fivebytwo(5,2);
		 ostringstream oss;
		 oss << "Turn = 0";
		 oss << endl;
		 oss << "  01" << endl;
		 oss << "0 .." << endl;
		 oss << "1 .." << endl;
		 oss << "2 .." << endl;
		 oss << "3 .." << endl;
		 oss << "4 .." << endl;
		 CPPUNIT_ASSERT(oss.str() == fivebytwo.boardToString());
		}
    void NoOverWrite(){
		 DarwinBoard fourbyfourwithFood(4,4);
		 Food f;
		 Rover r;
		 Creature f1(0,0,f);
		 Creature r1(0,0,r);
		 fourbyfourwithFood.putCreature(1,1,f1);
     fourbyfourwithFood.putCreature(1,1,r1);
		 ostringstream oss;
		 oss << "Turn = 0";
		 oss << endl;
		 oss << "  0123" << endl;
		 oss << "0 ...." << endl;
		 oss << "1 .f.." << endl;
		 oss << "2 ...." << endl;
		 oss << "3 ...." << endl;
		 CPPUNIT_ASSERT(oss.str() == fourbyfourwithFood.boardToString());
		}
		// -----
		// suite
		// -----
		
		CPPUNIT_TEST_SUITE(TestDarwin);
		CPPUNIT_TEST(testInit1);
		CPPUNIT_TEST(testInit2);
		CPPUNIT_TEST(testInit3);
		CPPUNIT_TEST(testInit4);
		CPPUNIT_TEST(NoOverWrite);
		CPPUNIT_TEST_SUITE_END();};
	
} // cs

#endif // TestDarwin_h