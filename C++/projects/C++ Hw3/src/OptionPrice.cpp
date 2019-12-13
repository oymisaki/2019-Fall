#include "OptionPrice.h"
#include "NormalDistribution.h"

using namespace std;

double OptionPrice::black_scholes_option_price() const
{
    double k = this->get_strike();
    double s = this->get_spot();
    double r = this->get_risk_free_rate();
    double t = this->get_time_to_maturity();
    double sigma = this->get_volatility();
    OptionType type = this->get_type();

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

double OptionPrice::binominal_option_price(int periods) const
{

    double k = this->get_strike();
    double s = this->get_spot();
    double r = this->get_risk_free_rate();
    double T = this->get_time_to_maturity();
    double sigma = this->get_volatility();
    OptionType type = this->get_type();
    double dt = T / periods;
    double u = exp(sigma * sqrt(dt));
    double d = 1.0 / u;

    double qu = (exp(r * dt) - d) / (u - d);
    double qd = (u - exp(r * dt)) / (u - d);

    int count = (periods + 1) * (periods + 2) / 2;
    double *C = new double[count];
    double *S = new double[count];
    S[0] = s;
    int now = 1;
    for (int i = 1; i < periods + 1; i++)
    {
        S[now] = S[now - i] * d;
        for (int j = 1; j < i + 1; ++j)
        {
            S[now + j] = S[now + j - 1] * u * u;
        }
        now += i + 1;
    }

    for (int i = count - periods - 1; i < count; i++)
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

    int now_count = periods;
    int this_count = now_count;
    for (int i = count - periods - 2; i > -1; --i)
    {
        C[i] = (qu * C[i + now_count + 1] + qd * C[i + now_count]) / exp(dt * r);
        if (--this_count == 0)
        {
            this_count = --now_count;
        }
    }

    double result = C[0];

    delete[] S;
    delete[] C;

    return result;
}

double OptionPrice::binominal_american_option_price(int periods) const
{
    double k = this->get_strike();
    double s = this->get_spot();
    double r = this->get_risk_free_rate();
    double T = this->get_time_to_maturity();
    double sigma = this->get_volatility();
    OptionType type = this->get_type();
    double dt = T / periods;
    double u = exp(sigma * sqrt(dt));
    double d = 1.0 / u;

    double qu = (exp(r * dt) - d) / (u - d);
    double qd = (u - exp(r * dt)) / (u - d);

    int count = (periods + 1) * (periods + 2) / 2;
    double *C = new double[count];
    double *S = new double[count];
    S[0] = s;
    int now = 1;
    for (int i = 1; i < periods + 1; i++)
    {
        S[now] = S[now - i] * d;
        for (int j = 1; j < i + 1; ++j)
        {
            S[now + j] = S[now + j - 1] * u * u;
        }
        now += i + 1;
    }

    for (int i = count - periods - 1; i < count; i++)
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

    int now_count = periods;
    int this_count = now_count;
    for (int i = count - periods - 2; i > -1; --i)
    {

        C[i] = (qu * C[i + now_count + 1] + qd * C[i + now_count]) / exp(dt * r);
        double execution_early;
        switch (type)
        {
        case OptionType::call:
            execution_early = std::max(S[i] - k, 0.0);
            break;
        case OptionType::put:
            execution_early = std::max(k - S[i], 0.0);
            break;
        default:
            break;
        }

        C[i] = std::max(C[i], execution_early);

        if (--this_count == 0)
        {
            this_count = --now_count;
        }
    }

    double result = C[0];

    delete[] S;
    delete[] C;

    return result;
}
