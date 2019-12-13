#include "Distribution.h"
#include <boost/math/distributions.hpp>

double N(double x)
{
    boost::math::normal_distribution<> d(0, 1);
    return cdf(d, x);
}

double NormalDensity(double x)
{
    boost::math::normal_distribution<> d(0, 1);
    return pdf(d, x);
}