#ifndef RANDOM_H
#define RANDOM_H

#include <boost/random.hpp>

using boost::lagged_fibonacci1279;
using boost::normal_distribution;
using boost::variate_generator;

// random number generator based on boost

class RandomBase
{
private:

public:
    RandomBase() {}
    virtual ~RandomBase() {}

    virtual RandomBase* clone() const = 0;
    virtual double generate() = 0;
};

class RandomNormal: public RandomBase
{
private:
    int seed;
    int mean;
    int sigma;
    lagged_fibonacci1279 rng;
    normal_distribution<> norm;
    variate_generator<lagged_fibonacci1279&, normal_distribution<>> generator;

public:
    RandomNormal(int seed_, int mean_, int sigma_);
    virtual ~RandomNormal() {}

    virtual RandomBase* clone() const;
    virtual double generate();
};

#endif // !RANDOM_H