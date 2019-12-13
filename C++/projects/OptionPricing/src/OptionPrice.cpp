#include "OptionPrice.h"
#include "NormalDistribution.h"
#include <iostream>
#include "PrintLatex.h"

using namespace std;

double OptionPrice::black_scholes_option_price(double k, double s, double r, double t, double sigma, OptionType type) const
{
    double sigma_sqr = pow(sigma, 2);
    double time_sqrt = sqrt(t);
    double d1 = (log(s / k) + (r + 0.5 * sigma_sqr) * t) / (sigma * time_sqrt);
    double d2 = d1 - sigma * time_sqrt;
    double price;

    switch (type)
    {
    case OptionType::call:
        price = s * N(d1) - k * exp(-r * t) * N(d2);
        break;
    case OptionType::put:
        price = k * exp(-r * t) * N(-d2) - s * N(-d1);
        break;
    default:
        throw "Undefined Option Type";
    }
    return price;
}

double OptionPrice::binominal_option_price(double k, double r, double s, double u, double d, double T, double t, OptionType type) const
{

    double dt = T / t;
    double qu = (1 + r * dt - d) / (u - d);
    double qd = (u - 1 - r * dt) / (u - d);

    int count = pow(2, t + 1) - 1;
    double *C = new double[count];
    double *S = new double[count];
    S[0] = s;
    for (int i = 1; i < count; i++)
    {
        int flag = i % 2;
        S[i] = S[(i + flag) / 2 - 1] * (flag == 1 ? d : u);
    }

    for (int i = (count + 1) / 2 - 1; i < count; i++)
    {
        switch (type)
        {
        case OptionType::call:
            C[i] = std::max(S[i] - k, 0.0);
            break;
        case OptionType::put:
            C[i] = std::max(k - S[i], 0.0);
            break;
        default:
            throw "Undefined Option Type";
        }
    }

    for (int i = (count + 1) / 2 - 2; i > -1; i--)
    {
        // ! should use exp(-dt * r)
        C[i] = (qu * C[2 * i + 2] + qd * C[2 * i + 1]) / (1 + dt * r);
    }

    double result = C[0];

    std::cout << "******* Binominal tree of stock prices ******" << std::endl;
    printLatex(S, count, t);
    std::cout << std::endl;
    std::cout << "******* Binominal tree of option prices ******" << std::endl;
    printLatex(C, count, t);

    delete[] S;
    delete[] C;

    return result;
}

double OptionPrice::binominal_american_option_price(double k, double r, double s, double u, double d, double T, double t, OptionType type) const
{

    double dt = T / t;
    double qu = (1 + r * dt - d) / (u - d);
    double qd = (u - 1 - r * dt) / (u - d);

    int count = pow(2, t + 1) - 1;
    double *C = new double[count];
    double *S = new double[count];
    S[0] = s;
    for (int i = 1; i < count; i++)
    {
        int flag = i % 2;
        S[i] = S[(i + flag) / 2 - 1] * (flag == 1 ? d : u);
    }

    for (int i = (count + 1) / 2 - 1; i < count; i++)
    {
        switch (type)
        {
        case OptionType::call:
            C[i] = std::max(S[i] - k, 0.0);
            break;
        case OptionType::put:
            C[i] = std::max(k - S[i], 0.0);
            break;
        default:
            throw "Undefined Option Type";
        }
    }

    for (int i = (count + 1) / 2 - 2; i > -1; i--)
    {
        // ! should use exp(-dt * r)
        C[i] = (qu * C[2 * i + 2] + qd * C[2 * i + 1]) / (1 + dt * r);
        
        // ! This is for American option
        double execution_early;
        switch (type)
        {
        case OptionType::call:
            execution_early = S[i] - k;
            break;
        case OptionType::put:
            execution_early = k - S[i];
        default:
            break;
        }

        C[i] = std::max(C[i], execution_early);
    }

    double result = C[0];

    std::cout << "******* Binominal tree of stock prices ******" << std::endl;
    printLatex(S, count, t);
    std::cout << std::endl;
    std::cout << "******* Binominal tree of option prices ******" << std::endl;
    printLatex(C, count, t);

    delete[] S;
    delete[] C;

    return result;
}

