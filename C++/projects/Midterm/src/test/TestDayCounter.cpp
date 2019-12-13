#include "Test.h"
#include "DayCounter.h"

void testDayCounter()
{
    cout << "============================================" << endl;
    cout << "Testing: " << BOOST_CURRENT_FUNCTION << endl;
    BusinessDayCounter bdc("%Y-%m-%d");
    cout << "BusinessDayCounter::dayCount: ";
    testClassFuncTwoParameter(bdc, &BusinessDayCounter::dayCount, "2019-01-01", "2019-01-05");

    cout << "BusinessDayCounter::getDates: ";
    testClassFuncTwoParameter(bdc, &BusinessDayCounter::getDates, "2019-01-01", 10);
}