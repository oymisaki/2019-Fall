#include "Test.h"
#include "VanillaOption.h"

void testVanillaOption()
{
    cout << "============================================" << endl;
    cout << "Testing: " << BOOST_CURRENT_FUNCTION << endl;

    cout << "VanillaCallOption: ";
    VanillaCallOption call(250, 58, "2019-01-01");
    testClassFuncThreeParameter(call, &VanillaCallOption::ImpliedVol, 300.0, 0.03, "2018-01-01");

    cout << "VanillaPutOption: ";
    VanillaPutOption put(250, 58, "2019-01-01");
    testClassFuncThreeParameter(put, &VanillaPutOption::ImpliedVol, 300.0, 0.03, "2018-01-01");
}