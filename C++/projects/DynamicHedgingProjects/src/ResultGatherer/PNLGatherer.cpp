#include "ResultGatherer.h"

PNLGatherer::PNLGatherer()
    : P0(-1.0)
{
}

void PNLGatherer::gatherResult(const Parameters &p)
{
    if (P0 == -1.0)
        P0 = p.V;
    this->result.push_back(p.V - P0);
}

string PNLGatherer::getName() const
{
    return "PNL/Wealth";
}

void PNLGatherer::clear()
{
    P0 = -1.0;
    this->result.clear();
}
