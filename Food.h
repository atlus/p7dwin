/*
 *  Food.h
 *  cplusplustest
 *
 *  Created by Michael Pena on 11/11/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */
#ifndef FOOD_H
#define FOOD_H
#include "Species.h"
class Food : public Species {
public:
	//const char* commands[2];
	
	Food();
	char getSymbol();
};
#endif