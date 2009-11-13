/*
 *  Hopper.h
 *  cplusplustest
 *
 *  Created by Michael Pena on 11/11/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef HOPPER_H
#define HOPPER_H
#include "Species.h"
class Hopper : public Species {
public:
	//const char* commands[2];
	
	Hopper();
	char getSymbol();
};
#endif