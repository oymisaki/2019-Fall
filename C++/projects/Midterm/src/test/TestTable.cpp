#include "Test.h"
#include "Table.h"

void testTable()
{
    cout << "============================================" << endl;
    cout << "Testing: " << BOOST_CURRENT_FUNCTION << endl;
    Table test;
    cout << "Table::readFromCsv: ";
    testClassFuncTwoParameter(test, &Table::readFromCsv, "./src/test/test_in.csv", 20000);

    cout << "Table::writeCsv: ";
    testClassFuncOneParameter(test, &Table::writeCsv, "./src/test/test_out.csv");

    cout << "Table::setHeaders: ";
    vector<string> headers = {"date", "rate"};
    testClassFuncOneParameter(test, &Table::setHeaders, headers);

    cout << "Table::tableInRange: ";
    testClassFuncTwoParameter(test, &Table::tableInRange, "2011-01-07", "2011-01-11");

    cout << "Table::getDbl: ";
    string row = "2011-01-07";
    string col = "rate";
    testClassFuncTwoParameter(test, &Table::getDbl, row, col);

    cout << "Table::setMultiIndex: ";
    vector<int> indexes = {1, 2};
    testClassFuncOneParameter(test, &Table::setMultiIndex, indexes);

    cout << "Table::tableEqualRange: ";
    testClassFuncOneParameter(test, &Table::tableEqualRange, "2011-01-07");

}
