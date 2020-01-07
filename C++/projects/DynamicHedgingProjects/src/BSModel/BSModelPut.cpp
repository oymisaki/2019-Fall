#include "BSModel.h"
#include <math.h>
#include <boost/bind.hpp>
#include <boost/function.hpp>
#include "RootSolver.h"
#include "Distribution.h"

using boost::bind;
using boost::function;

using namespace std;

BSModel* BSModelPut::clone() const
{
    return new BSModelPut(*this);
}

double BSModelPut::operator()(double s, double k, double r, double t, double sigma) const
{
    double sigma_sqr = pow(sigma, 2);
    double time_sqrt = sqrt(t);
    double d1 = (log(s / k) + (r + 0.5 * sigma_sqr) * t) / (sigma * time_sqrt);
    double d2 = d1 - sigma * time_sqrt;

    return k * exp(-r * t) * N(-d2) - s * N(-d1);
}

double BSModelPut::delta(double s, double k, double r, double t, double sigma) const
{
    double sigma_sqr = pow(sigma, 2);
    double time_sqrt = sqrt(t);
    double d1 = (log(s / k) + (r + 0.5 * sigma_sqr) * t) / (sigma * time_sqrt);

    return -N(-d1);
}

double BSModelPut::impliedVol(double s, double k, double r, double t, double price) const
{
    boost::function<double(double, double, double, double, double, double)> lambda = [this](double s, double k, double r, double t, double sigma, double price) -> double {
        return (*this)(s, k, r, t, sigma) - price;
    };

    boost::function<double(double)> func_;
    func_ = bind(lambda, s, k, r, t, _1, price);

    boost::function<double(double)> deriv_;
    deriv_ = bind(&BSModelPut::vega, this, s, k, r, t, _1);

    double impliedVol;
    DerivFunctor<boost::function<double(double)>, boost::function<double(double)>> functor(func_, deriv_);
    try
    {
        impliedVol = NewtonRaphson(functor, 5.0, 0.0, INT32_MAX, 10, 20);
    }
    catch (exception &e)
    {
    }
    return impliedVol;
}