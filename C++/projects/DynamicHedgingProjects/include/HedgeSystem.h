#ifndef HEDGE_SYSTEM_H
#define HEDGE_SYSTEM_H

#include "Table.h"
#include "ResultGatherer.h"

class HedgeSystem
{
private:
    Table interestData;
    Table optionData;
    Table stockData;
    vector<string> uniqueOptions; // vector of options with unique strike price, expiration dates and types
    vector<ResultGatherer *> gatherers;

    string outDir;

public:
    HedgeSystem() {}
    ~HedgeSystem() {}

    // run on all the unique options in the data file and construct hedging portfolios
    void runAll(); 
    // run on one specific option in a period
    void runOne(string t0, string tn, string expire, string flag, string K);

    // reading in data
    void readInterestData(string path, int lineNum);
    void readOptionData(string path, int lineNum);
    void readstockData(string path, int lineNum);

    void setResultCsvDir(string path);

    // setting statistics 
    void setResultGatherer(vector<ResultGatherer *> gatherers_);

    // caculate result for one option data
    void getResultForOneOption(Table& optionData_);

    // clear old results
    void clearGatherers();
};

#endif // !SIMULATION_H