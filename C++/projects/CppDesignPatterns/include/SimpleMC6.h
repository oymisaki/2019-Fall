#ifndef SIMPLEMC4_H
#define SIMPLEMC4_H

#include "Vanilla3.h"
#include "Parameters.h"

double SimpleMonteCarlo4(const VanillaOption& TheOption, double Spot, const Parameters& Vol, const Parameters& r, unsigned long NumberofPaths);

#endif // !SIMPLEMC4_H
