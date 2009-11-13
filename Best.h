/*
 *  Best.h
 *  cplusplustest
 *
 *  Created by Michael Pena on 11/11/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */
#ifndef BEST_H
#define BEST_H
#include "Species.h"
class Best : public Species {
public:
	//const char* commands[2];
	
	Best();
	char getSymbol();
};
#endif