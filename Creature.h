#ifndef CREATURE_H
#define CREATURE_H
#include "Species.h"
class Creature {
private:
	int programCount;
	int direction;
	Species* spointer;
public:
	Creature(int pc, int dir, Species &spIn);
	Species* getSpecies();
	char getSymbol();
};
#endif