#include "Test.h"
#include "Random.h"

void testRandomNormal()
{
    cout << "============================================" << endl;
    cout << "Testing: " << BOOST_CURRENT_FUNCTION << endl;

    cout << "RandomNormal::generate: ";
    RandomNormal rnorm(89120, 0, 1);
    testClassFuncNoParameter(rnorm, &RandomNormal::generate);
}