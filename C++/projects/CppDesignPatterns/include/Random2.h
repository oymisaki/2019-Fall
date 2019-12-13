#ifndef RANDOM2_H
#define RANDOM2_H

#include <Arrays.h>

class RandomBase
{
private:
    unsigned long Dimensionality;
public:
    RandomBase(unsigned long Dimensionality);

    inline unsigned long GetDimensionality() const;

    virtual RandomBase* clone() const = 0;
    virtual void GetUniforms(MJArray& variates) = 0;
    virtual void Skip(unsigned long numberOfPaths) = 0;
    virtual void SetSeed(unsigned long Seed) = 0;
    virtual void Reset() = 0;

    virtual void GetGaussians(MJArray& variates);
    virtual void ResetDimensionality(unsigned long NewDimensionality);

};

unsigned long RandomBase::GetDimensionality() const
{
    return Dimensionality;
}

#endif // !RANDOM2_H