#include "MCStatistics.h"

using namespace std;

StatisticsMean::StatisticsMean(): RunningSum(0), PathsDone(0) {}

void StatisticsMean::DumpOneResult(double result)
{
    PathsDone++;
    RunningSum += result;
}

vector<vector<double> > StatisticsMean::GetResultsSoFar() const
{
    vector<vector< double> > Results(1);

    Results[0].resize(1);
    Results[0][0] = RunningSum / PathsDone;

    return Results;
}

StatisticsMC* StatisticsMean::clone() const
{
    return new StatisticsMean(*this);
}