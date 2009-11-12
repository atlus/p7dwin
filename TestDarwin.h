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
		
		// <your tests>
		
		// -----
		// suite
		// -----
		
		CPPUNIT_TEST_SUITE(TestDarwin);
		CPPUNIT_TEST_SUITE_END();};
	
} // cs

#endif // TestDarwin_h