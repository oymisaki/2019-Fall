#ifndef NORMALDISTRIBUTION_H
#define NORMALDISTRIBUTION_H

inline double N(double param)
{
    double result;
    double x = param;
    if (param < 0)
        x = -param;

    double z = 1.0 / (1.0 + 0.2316419 * x);
    double a1 = 0.319381530;
    double a2 = -0.356563782;
    double a3 = 1.781477937;
    double a4 = -1.821255978;
    double a5 = 1.330274429;

    double Rz = z * (z * (z * (z * (z * a5 + a4) + a3) + a2) + a1);

    result = 1 - 1 / sqrt(2 * M_PIf32) * exp(-pow(x, 2) / 2) * Rz;

    return param >= 0 ? result : 1 - result;
}

#endif // !NORMALDISTRIBUTION_H