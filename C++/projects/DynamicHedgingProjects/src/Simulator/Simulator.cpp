#include "Simulator.h"

Simulator::Simulator(SamplePath *generator_, BSModel *model_)
: generator(generator_), model(model_)
{
}

void Simulator::simulate(int N, double K, double T, double r, double sigma)
{
    // clear old results
    this->clear();

    // generate stock paths
    generator->generatePath(N);
    const vector<double> &stockPrices = generator->getPath();

    double dt = T / N;

    optionPrices.push_back((*model)(stockPrices[0], K, r, T, sigma));
    deltas.push_back(model->delta(stockPrices[0], K, r, T, sigma));
    hedgeErrors.push_back(0.0);

    double bond0 = optionPrices[0] - deltas[0] * stockPrices[0];

    // calculate option prices, deltas and hedging errors
    for (int i = 1; i < N; ++i)
    {
        optionPrices.push_back((*model)(stockPrices[i], K, r, T - i * dt, sigma));
        deltas.push_back(model->delta(stockPrices[i], K, r, T - i * dt, sigma));
        hedgeErrors.push_back(deltas[i - 1] * stockPrices[i] + bond0 * exp(r * dt) - optionPrices[i]);
        bond0 = deltas[i - 1] * stockPrices[i] + bond0 * exp(r * dt) - deltas[i] * stockPrices[i];
    }
}

void Simulator::clear()
{
    this->optionPrices.clear();
    this->deltas.clear();
    this->hedgeErrors.clear();
}