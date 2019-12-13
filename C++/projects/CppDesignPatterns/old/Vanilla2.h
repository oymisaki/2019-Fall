#ifndef VANILLA2_H
#define VANILLA2_H

#include "PayOff3.h"

class VanillaOption
{
private:
    double Expiry;
    PayOff* ThePayOffPtr;

public:
    VanillaOption(const PayOff& ThePayOff_, double Expiry_);
    VanillaOption(const VanillaOption& original);
    VanillaOption& operator=(const VanillaOption& original);
    ~VanillaOption();

    double GetExpiry() const;
    double OptionPayOff(double Spot) const;

};

#endif // !VANILLA2_H