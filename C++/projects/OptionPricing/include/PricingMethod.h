#ifndef PRICINGMETHOD_H
#define PRICINGMETHOD_H

enum OptionType
{
    call,
    put
};

class PricingMethod
{
public:
    PricingMethod() = default;
    PricingMethod(const PricingMethod &) = default;
    ~PricingMethod() = default;

    virtual double black_scholes_option_price(double k, double s, double r, double t, double sigma, OptionType type) const = 0;
    virtual double binominal_option_price(double k, double r, double s, double u, double d, double T, double t, OptionType type) const = 0;
    virtual double binominal_american_option_price(double k, double r, double s, double u, double d, double T, double t, OptionType type) const = 0;
private:
};

#endif // !PRICINGMETHOD_H
