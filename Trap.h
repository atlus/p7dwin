/*
 *  Trap.h
 *  cplusplustest
 *
 *  Created by Michael Pena on 11/11/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef TRAP_H
#define TRAP_H
#include "Species.h"
class Trap : public Species {
public:
	const char* commands[5];
	
	Trap();
	char getSymbol();
};
#endif