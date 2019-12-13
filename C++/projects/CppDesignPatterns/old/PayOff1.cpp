/** This is the original Class Pyaoff with no encasplation*/
#include "PayOff1.h"
#include <algorithm>

PayOff::PayOff(double Strike_, OptionType TheOptionsType_)
    : Strike(Strike_), TheOptionsType(TheOptionsType_)
{
}

double PayOff::operator ()(double spot) const
{
    switch (TheOptionsType)
    {
    case call:
        return std::max(spot - Strike, 0.0);

    case put:
        return std::max(Strike - spot, 0.0);

    default:
        throw("unkown option type found");    
    }
}