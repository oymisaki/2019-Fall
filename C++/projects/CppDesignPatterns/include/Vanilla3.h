#ifndef VANILLA3_H
#define VANILLA3_H

#include "PayOffBridge.h"

class VanillaOption
{
private:
    PayOffBridge ThePayOff;
    double Expiry;

public:
    VanillaOption(const PayOffBridge& ThePayOff_, double Expiry_);

    double GetExpiry() const;
    double OptionPayOff(double Spot) const;
};

#endif // !VANILLA3_H