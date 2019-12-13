#include "Vanilla3.h"

// SECTION Vanilla3.h use bridge pattern to reuse rule of three. 
// ANCHOR It was designed to incorporate expriry to an option, and deal with payoff and expiry separately
// NOTE The Vanilla1.h is designed too bad. It used reference to store object, which is very dangerous

VanillaOption::VanillaOption(const PayOffBridge& ThePayOff_, double Expiry_): ThePayOff(ThePayOff_), Expiry(Expiry_)
{
}

double VanillaOption::GetExpiry() const
{
    return Expiry;
}

double VanillaOption::OptionPayOff(double Spot) const
{
    return ThePayOff(Spot);
}
