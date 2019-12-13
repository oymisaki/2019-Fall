#include <iostream>
#include "Test.h"
#include "Routine.h"

using namespace std;

int main()
{
    // run unit tests
    testTable();
    testRandomNormal();
    testDiffusionStockSamplePath();
    testBSModel();
    testRootSolver();
    testVanillaOption();
    testResultGatherer();
    testDayCounter();
    testRoutine();

    // simulation
    runSimulation();

    // hedge portfolio
    runHedgeSystem();

    return 0;
}