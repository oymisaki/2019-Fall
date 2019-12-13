#include "SimpleMC4.h"
#include "Random1.h"
#include <cmath>

// NOTE Simple MC using

double SimpleMonteCarlo3(const VanillaOption& TheOption, double Spot, double Vol, double r, unsigned long NumberofPaths)
{
    double Expiry = TheOption.GetExpiry();

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
        double thisPayoff = TheOption.OptionPayOff(thisSpot);
        runningSum += thisPayoff;
    }

    double mean = runningSum / NumberofPaths;
    mean *= exp(-r *Expiry);
    return mean;
}

