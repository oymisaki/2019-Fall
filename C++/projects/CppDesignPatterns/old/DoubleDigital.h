#ifndef DOUBLEDIGITAL_H
#define DOUBLEDIGITAL_H

// NOTE ------------------ Use Abstract Payoff Class to realize more functionability ----------------- //

#include "PayOff2.h"

class PayOffDoubleDigital: public PayOff                
{
private:
    double LowerLevel;
    double UpperLevel;
public:
    PayOffDoubleDigital(double LowerLevel_, double UpperLevel_);
    virtual double operator()(double Spot) const;
    virtual ~PayOffDoubleDigital() = default;
};

#endif // !DOUBLEDIGITAL_H