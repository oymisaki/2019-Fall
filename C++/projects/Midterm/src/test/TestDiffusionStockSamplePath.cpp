#include "Test.h"
#include "SamplePath.h"

void testDiffusionStockSamplePath()
{
    cout << "============================================" << endl;
    cout << "Testing: " << BOOST_CURRENT_FUNCTION << endl;

    cout << "DiffusionStockSamplePath::generatePath: ";
    RandomNormal rnorm(89120, 0, 1);
    DiffusionStockSamplePath p(100.0, 1.0/252, 0.05, 0.24, rnorm);
    testClassFuncOneParameter(p, &DiffusionStockSamplePath::generatePath, 100);
    cout << "DiffusionStockSamplePath::getPath: ";
    testClassFuncNoParameter(p, &DiffusionStockSamplePath::getPath);
}