#include "VanillaOption.h"
#include "BSModel.h"
#include "DayCounter.h"

VanillaOption::VanillaOption(double k_, double p_, string exp_)
    : k(k_), p(p_), expire(exp_)
{
}

int VanillaOption::getDaysToMaturity(string today) const
{
    BusinessDayCounter counter("%Y-%m-%d");
    return counter.dayCount(today, expire);
}

VanillaOption* VanillaCallOption::clone() const
{
    return new VanillaCallOption(*this);
}

VanillaCallOption::VanillaCallOption(double k_, double p_, string exp_)
    : VanillaOption(k_, p_, exp_)
{
}

double VanillaCallOption::ImpliedVol(double s, double r, string today) const
{
    BSModelCall bsc;
    double t = static_cast<double>(this->getDaysToMaturity(today)) / 252;
    return bsc.impliedVol(s, k, r, t, p);
}