#ifndef ROOT_SOLVER_H
#define ROOT_SOLVER_H

#include <boost/math/tools/roots.hpp>

// root solver based on boost

template <typename T1, typename T2>
class DerivFunctor
{
private:
    T1 funcProvider;
    T2 derivProvider;

public:
    DerivFunctor(T1 func_, T2 deriv_);
    ~DerivFunctor() {}

    std::pair<double, double> operator()(double const &x);
};

template <typename T1, typename T2>
DerivFunctor<T1, T2>::DerivFunctor(T1 func_, T2 deriv_)
    : funcProvider(func_), derivProvider(deriv_)
{
}

template <typename T1, typename T2>
std::pair<double, double> DerivFunctor<T1, T2>::operator()(double const &x)
{
    double fx = funcProvider(x);
    double dx = derivProvider(x);
    return std::make_pair(fx, dx);
}

template <typename T1, typename T2>
double NewtonRaphson(DerivFunctor<T1, T2> d, double guess, double min, double max, double precision, boost::uintmax_t maxit)
{

    using namespace boost::math::tools;
    const int digits = std::numeric_limits<double>::digits;
    int get_digits = static_cast<int>(digits * precision);

    return newton_raphson_iterate(d, guess, min, max, get_digits, maxit);
}

#endif
