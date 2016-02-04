#pragma once
#include "math.h"

ref class CZTable
{
public:
	CZTable(void);

	static double lookupZScore(double z);
	static double probBetweenScores( double z1, double z2) ;

};
