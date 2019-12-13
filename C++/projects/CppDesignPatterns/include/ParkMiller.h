#ifndef PARKER_MILLER_H
#define PARKER_MILLER_H

#include "Random2.h"

class ParkMiller    
{
private:
    long Seed;
public:
    ParkMiller(long seed = 1);

    long GetOneRandomInteger();
    void SetSeed(long Seed_);
    static unsigned long Max();
    static unsigned long Min();
};

class RandomParkMiller : public RandomBase
{
private:
    ParkMiller InnerGenerator;
    unsigned long InitialSeed;
    double Reciprocal;
    
public:
    RandomParkMiller(unsigned long Dimensionality, unsigned long Seed = 1);

    virtual RandomBase* clone() const;
    virtual void GetUniforms(MJArray& variates);
    virtual void Skip(unsigned long numberOfPaths);
    virtual void SetSeed(unsigned long Seed);
    virtual void Reset();
    virtual void ResetDimensionality(unsigned long NewDimensonality);
};

#endif // !PARKER_MILLER_H