#include "DoubleDigital2.h"

// -------- Extend the opiton type to cover double digital option using base class PayOff ------- //

PayOffDoubleDigital::PayOffDoubleDigital(double LowerLevel_, double UpperLevel_): LowerLevel(LowerLevel_), UpperLevel(UpperLevel_)
{
}

double PayOffDoubleDigital::operator()(double Spot) const
{
    if(Spot <= LowerLevel)
        return 0;
    if(Spot >= UpperLevel)
        return 0;
    return 1;
}

PayOff* PayOffDoubleDigital::clone() const
{
    return new PayOffDoubleDigital(*this);
}

