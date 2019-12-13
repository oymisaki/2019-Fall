#ifndef SIMPLEMC2_H
#define SIMPLEMC2_H

#include "PayOff2.h"
#include "DoubleDigital.h"

double SimpleMonteCarlo2(const PayOff& thePayoff, double Expiry, double Spot, double Vol, double r, unsigned long NumberofPaths);

#endif // !SIMPLEMC2_H
