#ifndef RESULT_GATHERER
#define RESULT_GATHERER

#include <string>
#include <vector>
#include "Parameters.h"

using namespace std;

// collecting results for a set of Parameters

class ResultGatherer
{
protected:
    vector<double> result;

public:
    ResultGatherer();
    virtual ~ResultGatherer() {}

    virtual void gatherResult(const Parameters& p) = 0;
    virtual string getName() const = 0;
    virtual void clear() = 0;

    const vector<double>& getResult() const;
};


class ImpliedVolGatherer: public ResultGatherer
{
private:

public:
    ImpliedVolGatherer();
    virtual ~ImpliedVolGatherer() {}

    virtual void gatherResult(const Parameters& p);
    virtual string getName() const;
    virtual void clear();
};

class DeltaGatherer: public ResultGatherer
{
private:
    ImpliedVolGatherer* innerGatherer;
public:
    DeltaGatherer(ImpliedVolGatherer* ImpliedVolGatherer_);
    virtual ~DeltaGatherer() {}

    virtual void gatherResult(const Parameters& p);
    virtual string getName() const;
    virtual void clear();
};

class PNLGatherer: public ResultGatherer
{
private:
    double P0;
public:
    PNLGatherer();
    virtual ~PNLGatherer() {}

    virtual void gatherResult(const Parameters& p);
    virtual string getName() const;
    virtual void clear();
};


class HedgeErrorGatherer: public ResultGatherer
{
    private:
    string date0;
    double delta0;
    double bond0;
    double r0;

    ResultGatherer* innerGatherer;

public:
    HedgeErrorGatherer(DeltaGatherer* DeltaGatherer_);
    virtual ~HedgeErrorGatherer() {}

    virtual void gatherResult(const Parameters& p);
    virtual string getName() const;
    virtual void clear();
};

class PNLHedgeGatherer: public ResultGatherer
{
private:
    double HE0;

    ResultGatherer* innerGatherer;

public:
    PNLHedgeGatherer(HedgeErrorGatherer* HedgeErrorGatherer_);
    virtual ~PNLHedgeGatherer() {}

    virtual void gatherResult(const Parameters& p);
    virtual string getName() const;
    virtual void clear();
};


#endif // !RESULT_GATHERER