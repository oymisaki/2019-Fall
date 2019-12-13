#include "Random.h"

RandomNormal::RandomNormal(int seed_, int mean_, int sigma_)
    : seed(seed_),
      mean(mean_),
      sigma(sigma_),
      rng(seed),
      norm(mean, sigma),
      generator(rng, norm)
{
}

RandomBase* RandomNormal::clone() const
{
    return new RandomNormal(*this);
}

double RandomNormal::generate()
{
    return generator();
}