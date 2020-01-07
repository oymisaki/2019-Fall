#include "ResultGatherer.h"

ImpliedVolGatherer::ImpliedVolGatherer()
{
}

void ImpliedVolGatherer::gatherResult(const Parameters& p)
{
    double impliedVol = p.option->ImpliedVol(p.S, p.r, p.date);
    this->result.push_back(impliedVol);
}

string ImpliedVolGatherer::getName() const
{
    return "Implied Volatility";
}

void ImpliedVolGatherer::clear()
{
    this->result.clear();
}
