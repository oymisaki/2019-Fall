#ifndef PRICINGMETHOD_H
#define PRICINGMETHOD_H

class PricingMethod
{
public:
    PricingMethod() = default;
    PricingMethod(const PricingMethod &) = default;
    virtual ~PricingMethod() = default;

    virtual double black_scholes_option_price() const = 0;
    virtual double binominal_option_price(int periods) const = 0;
    virtual double binominal_american_option_price(int periods) const = 0;
private:
};

#endif // !PRICINGMETHOD_H
