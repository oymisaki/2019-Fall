#ifndef OPTIONPRICE_H
#define OPTIONPRICE_H

#include "Option.h"
#include "PricingMethod.h"
#include <cmath>

class OptionPrice: public Option, public PricingMethod
{
private:

public:
    OptionPrice(double k_, double s_, double r_, double t_, double sigma_, OptionType type_): Option(k_, s_, r_, t_, sigma_, type_){}

    virtual double black_scholes_option_price() const;
    virtual double binominal_option_price(int periods) const;
    virtual double binominal_american_option_price(int periods) const;
};
#endif // !OPTIONPRICE_H