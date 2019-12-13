#include "ResultGatherer.h"
#include "DayCounter.h"
#include <math.h>

using std::exp;

HedgeErrorGatherer::HedgeErrorGatherer(DeltaGatherer *DeltaGatherer_)
    : date0(""), delta0(-1.0), bond0(-1.0), r0(-1.0)
{
    innerGatherer = DeltaGatherer_;
}

void HedgeErrorGatherer::gatherResult(const Parameters &p)
{
    if (date0 == "")
    {
        date0 = p.date;
        delta0 = *(innerGatherer->getResult().end() - 1);
        bond0 = p.V - delta0 * p.S;
        r0 = p.r;
        this->result.push_back(0.0);
    }
    else
    {
        BusinessDayCounter counter("%Y-%m-%d");
        double dt = (static_cast<double>(counter.dayCount(date0, p.date))) / 252;

        this->result.push_back(delta0 * p.S + bond0 * exp(r0 * dt) - p.V);

        double delta1 = *(innerGatherer->getResult().end() - 1);
        bond0 = delta0 * p.S + bond0 * exp(r0 * dt) - delta1 * p.S;
        delta0 = delta1;
        date0 = p.date;
        r0 = p.r;
    }
}

string HedgeErrorGatherer::getName() const
{
    return "Hedging Error";
}

void HedgeErrorGatherer::clear()
{
    date0 = "";
    delta0 = -1.0;
    bond0 = -1.0;
    r0 = -1.0;

    this->result.clear();
}
