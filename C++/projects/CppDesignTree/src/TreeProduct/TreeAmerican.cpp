#include "TreeAmerican.h"
#include <math.h>

TreeAmerican::TreeAmerican(double FinalTime,
                           const PayOffBridge &ThePayOff_)
    : TreeProduct(FinalTime),
      ThePayOff(ThePayOff_)
{
}

TreeProduct *TreeAmerican::clone() const
{
    return new TreeAmerican(*this);
}

double TreeAmerican::FinalPayOff(double Spot) const
{
    return ThePayOff(Spot);
}

double TreeAmerican::PreFinalValue(double Spot,
                                   double,
                                   // Borland compiler doesnt like unused named variables
                                   double DiscountedFutureValue) const
{
    return std::max(ThePayOff(Spot), DiscountedFutureValue);
}