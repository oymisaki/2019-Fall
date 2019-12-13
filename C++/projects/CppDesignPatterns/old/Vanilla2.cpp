#include "Vanilla2.h"

// SECTION Vanilla2.h use ptr to abstract base class PayOff. 
// ANCHOR It was designed to incorporate expriry to an option, and deal with payoff and expiry separately
// NOTE The Vanilla1.h is designed too bad. It used reference to store object, which is very dangerous

VanillaOption::VanillaOption(const PayOff& ThePayOff_, double Expiry_): Expiry(Expiry_)
{
    ThePayOffPtr = ThePayOff_.clone();
}

double VanillaOption::GetExpiry() const
{
    return Expiry;
}

double VanillaOption::OptionPayOff(double Spot) const
{
    return (*ThePayOffPtr)(Spot);
}

VanillaOption::VanillaOption(const VanillaOption& original)
{
    // NOTE copy constructor is required 
    Expiry = original.Expiry;
    ThePayOffPtr = original.ThePayOffPtr->clone();
}

VanillaOption& VanillaOption::operator=(const VanillaOption& original)
{
    // NOTE So, operator = should be reload suitably.
    if(this != &original)
    {
        Expiry = original.Expiry;
        delete ThePayOffPtr;
        ThePayOffPtr = original.ThePayOffPtr->clone();
    }
    return *this;
}

VanillaOption::~VanillaOption()
{
    // NOTE a deconstructor is required.
    delete ThePayOffPtr;
}