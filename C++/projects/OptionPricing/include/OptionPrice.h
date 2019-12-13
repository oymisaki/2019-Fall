#ifndef OPTIONPRICE_H
#define OPTIONPRICE_H

#include "Option.h"
#include "PricingMethod.h"
#include <cmath>

class OptionPrice: public Option, public PricingMethod
{
private:

public:
    OptionPrice(double k_, double s_, double r_, double t_, double sigma_): Option(k_, s_, r_, t_, sigma_){}

    virtual double black_scholes_option_price(double k, double s, double r, double t, double sigma, OptionType type) const;
    virtual double binominal_option_price(double k, double r, double s, double u, double d, double T, double t, OptionType type) const;
    virtual double binominal_american_option_price(double k, double r, double s, double u, double d, double T, double t, OptionType type) const;
};
#endif // !OPTIONPRICE_H