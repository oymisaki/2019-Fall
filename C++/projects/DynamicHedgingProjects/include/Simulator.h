#ifndef SIMULATOR_H
#define SIMULATOR_H

#include "SamplePath.h"
#include "BSModel.h"

// hedging simulator

class Simulator
{
private:
    SamplePath *generator;
    BSModel *model;

    vector<double> optionPrices;
    vector<double> hedgeErrors;
    vector<double> deltas;

public:
    Simulator(SamplePath *generator_, BSModel *model_);
    ~Simulator() {}


    void simulate(int N, double K, double T, double r, double sigma);
    void clear();


    inline const vector<double> & getOptionPrices(){return this->optionPrices;}
    inline const vector<double> & getDeltas() {return this->deltas;}
    inline const vector<double> & getHedgeErrors() {return this->hedgeErrors;}
    inline const vector<double> & getStockPrices() {return this->generator->getPath();}
};

#endif // !SIMULATOR_H