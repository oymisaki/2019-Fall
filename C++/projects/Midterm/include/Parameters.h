#ifndef PARAMETERS_H
#define PARAMETERS_H

#include <string>
#include <memory>
#include "BSModel.h"
#include "VanillaOption.h"

using namespace std;

// Parameter class designed for gatherer class
class Parameters
{
public:
    const string date;
    const double S;
    const double r;

    const double V;
    const double K;

    shared_ptr<VanillaOption> option;
    const string expire;

    shared_ptr<BSModel> bsm;

    Parameters(string date_, double S_, double r_, BSModel &bsm_, VanillaOption &option_)
        : date(date_),
          S(S_),
          r(r_),
          V(option_.getPrice()),
          K(option_.getStrike()),
          option(option_.clone()),
          expire(option_.getExpire()),
          bsm(bsm_.clone())
    {
    }
};

#endif // !PARAMETERS_H