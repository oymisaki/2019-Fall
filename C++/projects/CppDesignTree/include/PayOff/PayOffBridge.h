#ifndef PAYOFFBRIDGE_H
#define PAYOFFBRIDGE_H

// SECTION Example using Bridge Pattern. This is to reuse the rule of three codes: Copy, Assign, Deconstruct.
#include "PayOff.h"

class PayOffBridge
{
public:
    PayOffBridge(const PayOffBridge& orignal);
    PayOffBridge(const PayOff& innerPayOff);

    inline double operator()(double Spot) const;
    ~PayOffBridge();
    PayOffBridge& operator=(const PayOffBridge& original);

private:
    PayOff* ThePayOffPtr;
};

inline double PayOffBridge::operator()(double Spot) const
{
    return ThePayOffPtr->operator()(Spot);
}
#endif // !PAYOFFBRIDGE_H