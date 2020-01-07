#include "Test.h"
#include "ResultGatherer.h"

void testResultGatherer()
{
    cout << "============================================" << endl;
    cout << "Testing: " << BOOST_CURRENT_FUNCTION << endl;

    string date = "2018-01-01";
    string expire = "2019-01-01";
    double S = 300.0;
    double K = 250.0;
    double V = 58.0;
    double r = 0.03;

    BSModelCall bsc;
    VanillaCallOption call(K, V, expire);
    Parameters p(date, S, r, bsc, call);

    cout << "ImpliedVolGatherer::gather: ";
    ImpliedVolGatherer ivg;
    testClassFuncOneParameter(ivg, &ImpliedVolGatherer::gatherResult, p);

    cout << "DeltaGatherer::gather: ";
    DeltaGatherer dg(&ivg);
    testClassFuncOneParameter(dg, &DeltaGatherer::gatherResult, p);

    cout << "PNLGatherer::gather: ";
    PNLGatherer pg;
    testClassFuncOneParameter(pg, &PNLGatherer::gatherResult, p);

    cout << "HedgeErrorGatherer::gather: ";
    HedgeErrorGatherer heg(&dg);
    testClassFuncOneParameter(heg, &HedgeErrorGatherer::gatherResult, p);

    cout << "PNLHedgeGatherer::gather: ";
    PNLHedgeGatherer phg(&heg);
    testClassFuncOneParameter(phg, &PNLHedgeGatherer::gatherResult, p);
}