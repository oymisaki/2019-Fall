#include "ResultGatherer.h"

ResultGatherer::ResultGatherer()
    : result()
{
}

const vector<double>& ResultGatherer::getResult() const
{
    return this->result;
}
