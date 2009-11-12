/*
 *  Rover.h
 *  cplusplustest
 *
 *  Created by Michael Pena on 11/11/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef ROVER_H
#define ROVER_H
#include "Species.h"
class Rover : public Species {
public:
	const char* commands[11];
	
	Rover();
	char getSymbol();
};
#endif