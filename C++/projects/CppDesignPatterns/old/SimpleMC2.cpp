#include "SimpleMC2.h"
#include "Random1.h"
// #include "PayOff2.h"
#include <cmath>

// ---------------------------- SimpleMC using abstract PayOff class ---------------------------- //

double SimpleMonteCarlo2(const PayOff& thePayoff, double Expiry, double Spot, double Vol, double r, unsigned long NumberofPaths)
{
    double variance = Vol * Vol * Expiry;
    double rootVariance = sqrt(variance);
    double itoCorrection = -0.5 * variance;

    double movedSpot = Spot * exp(r * Expiry + itoCorrection);
    double thisSpot;
    double runningSum = 0;

    for(unsigned long i = 0; i < NumberofPaths; ++i )
    {
        double thisGaussian = GetOneGaussianByBoxMuller();
        thisSpot = movedSpot * exp(rootVariance * thisGaussian);
        double thisPayoff = thePayoff(thisSpot);
        runningSum += thisPayoff;
    }

    double mean = runningSum / NumberofPaths;
    mean *= exp(-r *Expiry);
    return mean;
}

