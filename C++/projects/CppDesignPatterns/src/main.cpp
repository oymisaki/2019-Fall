#include <SimpleMC8.h>
#include <ParkMiller.h>
#include <iostream>

#include <Vanilla3.h>
#include <MCStatistics.h>
#include <ConvergenceTable.h>
#include <AntiThetic.h>

using namespace std;

int runSimpleMC6()
{
    double Expiry;
    double Strike;
    double Spot;
    double Vol;
    double r;
    unsigned long NumberOfPaths;
    cout << "\nEnter expiry\n";
    cin >> Expiry;
    cout << "\nStrike\n";
    cin >> Strike;
    cout << "\nEnter spot\n";
    cin >> Spot;
    cout << "\nEnter vol\n";
    cin >> Vol;
    cout << "\nr\n";
    cin >> r;
    cout << "\nNumber of paths\n";
    cin >> NumberOfPaths;
    PayOffCall thePayOff(Strike);
    VanillaOption theOption(thePayOff, Expiry);
    ParametersConstant VolParam(Vol);
    ParametersConstant rParam(r);

    StatisticsMean gatherer;
    ConvergenceTable gathererTwo(gatherer);
    RandomParkMiller generator(1);
    AntiThetic GenTwo(generator);

    SimpleMonteCarlo6(theOption,
                      Spot,
                      VolParam,
                      rParam,
                      NumberOfPaths,
                      gathererTwo,
                      GenTwo);
                      
    vector<vector<double>> results =
        gathererTwo.GetResultsSoFar();

    cout << "\nFor the call price the results are \n";
    for (unsigned long i = 0; i < results.size(); i++)
    {
        for (unsigned long j = 0; j < results[i].size(); j++)
            cout << results[i][j] << " ";
        cout << "\n";
    }

    double tmp;
    cin >> tmp;
    return 0;
}

int main()
{
    runSimpleMC6();
    return 0;
}