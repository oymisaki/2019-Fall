#include "VanillaOption.h"
#include "BSModel.h"

VanillaPutOption::VanillaPutOption(double k_, double p_, string exp_)
    : VanillaOption(k_, p_, exp_)
{
}

VanillaOption* VanillaPutOption::clone() const
{
    return new VanillaPutOption(*this);
}

double VanillaPutOption::ImpliedVol(double s, double r, string today) const
{
    BSModelPut bsp;
    double t = static_cast<double>(this->getDaysToMaturity(today)) / 252;
    return bsp.impliedVol(s, k, r, t, p);
}