#ifndef STATISTICS_H
#define STATISTICS_H

// ! This is an example a straegy pattern
// ! This technique of using an auxiliary class to decide how part of an
// ! algorithm is implemented is sometimes called the strategy pattern
#include <vector>

class StatisticsMC
{
private:
    
public:
    StatisticsMC() = default;
    virtual ~StatisticsMC() = default;

    virtual void DumpOneResult(double result) = 0;
    virtual std::vector<std::vector<double> > GetResultsSoFar() const=0;
    virtual StatisticsMC* clone() const=0; 
};

class StatisticsMean: public StatisticsMC
{
public:
    StatisticsMean();
    ~StatisticsMean() = default;

    virtual void DumpOneResult(double result);
    virtual std::vector<std::vector<double> > GetResultsSoFar() const;
    virtual StatisticsMC* clone() const; 

private:
    double RunningSum;
    unsigned long PathsDone;
};

#endif // !STATISTICS_H
