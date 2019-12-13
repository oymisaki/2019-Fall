#include "Routine.h"

#include "HedgeSystem.h"
#include "Simulator.h"
#include "Plot.h"

void runHedgeSystem()
{
    try
    {
        HedgeSystem hedge;

        // read data
        hedge.readInterestData("./data/interest.csv", INT32_MAX);
        hedge.readstockData("./data/sec_GOOG.csv", INT32_MAX);
        hedge.readOptionData("./data/op_GOOG.csv", INT32_MAX);

        // set result dir
        hedge.setResultCsvDir("./out/hedge/");

        // init and set stats gatherers
        ImpliedVolGatherer ivg;
        DeltaGatherer dg(&ivg);
        PNLGatherer pg;
        HedgeErrorGatherer heg(&dg);

        vector<ResultGatherer *> gatherers = {&ivg, &dg, &pg, &heg};
        hedge.setResultGatherer(gatherers);

        // run
        hedge.runOne("2011-07-05", "2011-07-29", "2011-09-17", "C", "500");
    }
    catch (const exception &e)
    {
        cerr << e.what() << '\n';
    }
}

void runSimulation()
{
    // set parameters of simulator
    double S0 = 100.0;     // initial value of stock
    double dt = 1.0 / 250; // dt
    double mu = 0.05;      // mu
    double sigma = 0.24;   // sigma

    double K = 105;   // strike price of option
    double T = 0.4;   // initial time to maturity
    double r = 0.025; // annual interest rate

    int N = 100;              // number of periods
    int NumberOfPaths = 1000; // number of paths

    RandomNormal rnorm(89120, 0, 1);                      // random standrad normal
    DiffusionStockSamplePath p(S0, dt, mu, sigma, rnorm); // sample path generator
    BSModelCall call;

    // init simulator
    Simulator sim(&p, &call);

    // x axis values for plot
    vector<double> x(N, 1.0);
    for (int i = 0; i < N; ++i)
        x[i] = i;

    vector<vector<double>> stockPricesVec;
    vector<double> hedgeErrorVec;

    for (int i = 0; i < NumberOfPaths; ++i)
    {
        // simulate one path
        sim.simulate(N, K, T, r, sigma);
        const vector<double> &stockPrices = sim.getStockPrices();
        const vector<double> &hedgeErrors = sim.getHedgeErrors();

        if (i < 100)
            stockPricesVec.push_back(stockPrices);

        hedgeErrorVec.push_back(*(hedgeErrors.end() - 1));
    }

    // plot hedge errors
    plotHistogram(hedgeErrorVec, 20);
    plotTitle("Distribution of Hedging Errors");
    savePlot("./out/pic/hedging errors.png");

    plotClose();

    // plot stock prices paths
    for (auto i = stockPricesVec.begin(); i != stockPricesVec.end(); ++i)
        plotLine(x, *i);
    plotTitle("Paths of Stock Prices");
    savePlot("./out/pic/stock paths.png");

    plotClose();
}