#ifndef SAMPLE_PATH
#define SAMPLE_PATH

#include <vector>
#include "Random.h"
#include <math.h>
#include <memory>

using namespace std;

// SamplePath class to generate sample path

class SamplePath
{
private:
    vector<double> path;
public:
    SamplePath() {}
    virtual ~SamplePath() {}
    virtual void generatePath(int N) = 0;
    virtual const vector<double>& getPath() const = 0;
};

// Sample Path under diffusion model using using some random variables

class DiffusionStockSamplePath: public SamplePath
{
private:
    double S0;
    double dt;
    double mu;
    double sigma;
    
    shared_ptr<RandomBase> rand;

    vector<double> path;

public:
    DiffusionStockSamplePath(double S0_, double dt_, double mu_, double sigma_, RandomBase& rand_);
    virtual ~DiffusionStockSamplePath() {}

    virtual void generatePath(int N);
    virtual const vector<double>& getPath() const;
};

#endif // !SAMPLE_PATH