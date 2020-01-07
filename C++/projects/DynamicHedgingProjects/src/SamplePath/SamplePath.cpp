#include "SamplePath.h"

DiffusionStockSamplePath::DiffusionStockSamplePath(double S0_, double dt_, double mu_, double sigma_, RandomBase &rand_)
    : S0(S0_),
      dt(dt_),
      mu(mu_),
      sigma(sigma_),
      rand(rand_.clone()),
      path()
{
}

void DiffusionStockSamplePath::generatePath(int N)
{
    this->path = vector<double>();
    for (int i = 0; i < N; i++)
    {
        if (i == 0)
            this->path.push_back(S0);
        else
        {
            this->path.push_back(path[i - 1] + mu * path[i - 1] * dt + sigma * path[i - 1] * sqrt(dt) * rand->generate());
        }
    }
}

const vector<double> &DiffusionStockSamplePath::getPath() const
{
    return this->path;
}