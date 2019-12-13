#include "PayOff3.h"
#include <algorithm>

PayOffCall::PayOffCall(double Strike_): Strike(Strike_){}

double PayOffCall::operator()(double Spot) const
{
    return std::max(Spot-Strike, 0.0);
}

PayOff* PayOffCall::clone() const
{
    // NOTE return ptr pointed to an cloned *this allocated by new
    return new PayOffCall(*this);
}

PayOffPut::PayOffPut(double Strike_): Strike(Strike_){}

double PayOffPut::operator()(double Spot) const
{
    return std::max(Strike - Spot, 0.0);
}

PayOff* PayOffPut::clone() const
{
    // NOTE return ptr pointed to an cloned *this allocated by new
    return new PayOffPut(*this);
}

