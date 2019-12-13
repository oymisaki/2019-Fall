#include "ResultGatherer.h"

DeltaGatherer::DeltaGatherer(ImpliedVolGatherer *ImpliedVolGatherer_)
{
    innerGatherer = ImpliedVolGatherer_;
}

void DeltaGatherer::gatherResult(const Parameters &p)
{
    if (this->innerGatherer->getResult().size() > 0)
    {
        double impliedVol = *(this->innerGatherer->getResult().end() - 1);
        double t = static_cast<double>(p.option->getDaysToMaturity(p.date)) / 252; // time to maturity
        double delta = p.bsm->delta(p.S, p.K, p.r, t, impliedVol);
        this->result.push_back(delta);
    }
}

string DeltaGatherer::getName() const
{
    return "Delta";
}

void DeltaGatherer::clear()
{
    this->result.clear();
}
