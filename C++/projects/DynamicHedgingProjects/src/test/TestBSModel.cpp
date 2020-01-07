#include "Test.h"
#include "BSModel.h"

void testBSModel()
{
    cout << "============================================" << endl;
    cout << "Testing: " << BOOST_CURRENT_FUNCTION << endl;

    cout << "BSModelCall::operator(): ";
    BSModelCall bsc;
    testClassFuncFiveParameter(bsc, &BSModelCall::operator(), 300.0, 250.0, 0.03, 1.0, 0.15);

    cout << "BSModelCall::vega: ";
    testClassFuncFiveParameter(bsc, &BSModelCall::vega, 300.0, 250.0, 0.03, 1.0, 0.15);

    cout << "BSModelCall::delta: ";
    testClassFuncFiveParameter(bsc, &BSModelCall::delta, 300.0, 250.0, 0.03, 1.0, 0.15);

    cout << "BSModelCall::impliedVol: ";
    testClassFuncFiveParameter(bsc, &BSModelCall::impliedVol, 300.0, 250.0, 0.03, 1.0, 58.0);

    cout << "BSModelPut::operator(): ";
    BSModelPut bsp;
    testClassFuncFiveParameter(bsp, &BSModelPut::operator(), 300.0, 250.0, 0.03, 1.0, 0.15);

    cout << "BSModelPut::vega: ";
    testClassFuncFiveParameter(bsp, &BSModelPut::vega, 300.0, 250.0, 0.03, 1.0, 0.15);

    cout << "BSModelPut::delta: ";
    testClassFuncFiveParameter(bsp, &BSModelPut::delta, 300.0, 250.0, 0.03, 1.0, 0.15);

    cout << "BSModelPut::impliedVol: ";
    testClassFuncFiveParameter(bsp, &BSModelPut::impliedVol, 300.0, 250.0, 0.03, 1.0, 58.0);
}