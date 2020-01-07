#include "ResultGatherer.h"

PNLHedgeGatherer::PNLHedgeGatherer(HedgeErrorGatherer *HedgeErrorGatherer_)
    : HE0(0.0)
{
    innerGatherer = HedgeErrorGatherer_;
}

void PNLHedgeGatherer::gatherResult(const Parameters &p)
{
    p.K; //disable warning of unused parameter 
    // get hedge error from inner HedgeErrorGatherer
    HE0 += *(innerGatherer->getResult().end()-1);
    this->result.push_back(HE0);
}

string PNLHedgeGatherer::getName() const
{
    return "PNL with Hedge";
}

void PNLHedgeGatherer::clear()
{
    HE0 = 0.0;
    this->result.clear();
}
