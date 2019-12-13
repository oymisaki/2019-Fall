#include "TreeEuropean.h"
#include <math.h>

TreeEuropean::TreeEuropean(double FinalTime,
                           const PayOffBridge &ThePayOff_)
    : TreeProduct(FinalTime),
      ThePayOff(ThePayOff_)
{
}

TreeProduct *TreeEuropean::clone() const
{
    return new TreeEuropean(*this);
}

double TreeEuropean::FinalPayOff(double Spot) const
{
    return ThePayOff(Spot);
}

double TreeEuropean::PreFinalValue(double Spot,
                                   double,
                                   // Borland compiler doesnt like unused named variables
                                   double DiscountedFutureValue) const
{
    return DiscountedFutureValue;
}