#ifndef OPTION_H
#define OPTION_H

enum OptionType
{
    call,
    put
};


class Option
{
public:
    Option();
    Option(double k_, double s_, double r_, double t_, double sigma_, OptionType type);
    Option(const Option &) = default;
    ~Option() = default;

    inline double get_strike() const;
    inline double get_spot() const;
    inline double get_risk_free_rate() const;
    inline double get_time_to_maturity() const;
    inline double get_volatility() const;
    inline OptionType get_type() const;

private:
    double k;
    double s;
    double r;
    double t;
    double sigma;
    OptionType type;
    void init();
};

inline double Option::get_strike() const
{
    return k;
}

inline double Option::get_spot() const
{
    return s;
}

inline double Option::get_risk_free_rate() const
{
    return r;
}

inline double Option::get_time_to_maturity() const
{
    return t;
}

inline double Option::get_volatility() const
{
    return sigma;
}

inline OptionType Option::get_type() const
{
    return type;
}

#endif // !OPTION_H
