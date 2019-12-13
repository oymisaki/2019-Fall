#include "exotic.h"
#include <cmath>
#include "PrintLatex.h"
#include <iostream>

using namespace std;

double exotic_option_price(double k, double r, double s, double u, double d, double T)
{
    int t = 5;
    double dt = T / t;
    double qu = (exp(r * dt) - d) / (u - d);
    double qd = (u - exp(r * dt)) / (u - d);

    int count = pow(2, t + 1) - 1;
    double *C = new double[count];
    double *P = new double[count];
    double *O = new double[count];
    double *S = new double[count];

    S[0] = s;
    for (int i = 1; i < count; i++)
    {
        int flag = i % 2;
        S[i] = S[(i + flag) / 2 - 1] * (flag == 1 ? d : u);
    }

    for (int i = (count + 1) / 2 - 1; i < count; i++)
    {
        C[i] = std::max(S[i] - k, 0.0);
        P[i] = std::max(k - S[i], 0.0);
        O[i] = std::max(C[i], P[i]);
    }

    for (int i = (count + 1) / 2 - 2; i > -1; i--)
    {
        // ! should use exp(-dt * r)
        C[i] = (qu * C[2 * i + 2] + qd * C[2 * i + 1]) / exp(dt * r);
        P[i] = (qu * P[2 * i + 2] + qd * P[2 * i + 1]) / exp(dt * r);
        if (i >= 15)
            O[i] = max(C[i], P[i]);
        else
            O[i] = (qu * O[2 * i + 2] + qd * O[2 * i + 1]) / exp(dt * r);
    }

    double result = O[0];

    std::cout << "******* Binominal tree of stock prices ******" << std::endl;
    printLatex(S, count, t);
    std::cout << std::endl;
    std::cout << "******* Binominal tree of  call option prices ******" << std::endl;
    printLatex(C, count, t);
    std::cout << std::endl;
    std::cout << "******* Binominal tree of  put option prices ******" << std::endl;
    printLatex(P, count, t);
    std::cout << std::endl;
    std::cout << "******* Binominal tree of  exotic option prices ******" << std::endl;
    printLatex(O, count, t);

    delete[] S;
    delete[] C;
    delete[] P;
    delete[] O;

    return result;
}