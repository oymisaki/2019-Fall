#include "Option.h"

void Option::init()
{
    k = 0;
    s = 0;
    r = 0;
    t = 0;
    sigma = 0;
}

Option::Option()
{
    init();
}

Option::Option(double k_,
               double s_,
               double r_,
               double t_,
               double sigma_) : k(k_),
                                s(s_),
                                r(r_),
                                t(t_),
                                sigma(sigma_) {}
