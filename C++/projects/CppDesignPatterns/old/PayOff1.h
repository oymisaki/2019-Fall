#ifndef PAYOFF_H
#define PAYOFF_H

// NOTE ---------------------- initial PayOff Class with lower extentionability ---------------------- //

class PayOff
{
public:
    enum OptionType
    {
        call, put
    };
    PayOff(double Strike_, OptionType TheOptionsType_);
    PayOff(PayOff &&) = default;
    PayOff(const PayOff &) = default;
    PayOff &operator=(PayOff &&) = default;
    PayOff &operator=(const PayOff &) = default;
    ~PayOff() = default;

    double operator()(double spot) const;

private:
    double Strike;
    OptionType TheOptionsType;
};
#endif // !PAYOFF_H