#include "SimpleMC7.h"
#include "Random1.h"
#include <cmath>

// NOTE Simple MC using

void SimpleMonteCarlo5(const VanillaOption& TheOption, double Spot, const Parameters& Vol, const Parameters& r, unsigned long NumberofPaths, StatisticsMC& gatherer)
{
    double Expiry = TheOption.GetExpiry();

    double variance = Vol.IntegralSquare(0, Expiry);
    double rootVariance = sqrt(variance);
    double itoCorrection = -0.5 * variance;

    double movedSpot = Spot * exp(r.Integral(0, Expiry) + itoCorrection);
    double thisSpot;
    double discounting = exp(-r.Integral(0, Expiry));

    for(unsigned long i = 0; i < NumberofPaths; ++i )
    {
        double thisGaussian = GetOneGaussianByBoxMuller();
        thisSpot = movedSpot * exp(rootVariance * thisGaussian);
        double thisPayoff = TheOption.OptionPayOff(thisSpot);
        gatherer.DumpOneResult(thisPayoff * discounting);
    }

    return;
}

