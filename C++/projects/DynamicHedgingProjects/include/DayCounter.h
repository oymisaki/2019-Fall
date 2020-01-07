#ifndef DATE_COUNTER_H
#define DATE_COUNTER_H

#include <string>
#include <vector>

using namespace std;

// abstract day counter base class

class DayCounterBase
{
protected:
    string format;
public:
    DayCounterBase(string format_);
    DayCounterBase() {}
    virtual ~DayCounterBase() {}

    // counter days between
    virtual int dayCount(string start, string end) const = 0;
    // get dates series from
    virtual vector<string> getDates(string start, int num) const = 0;
};

class BusinessDayCounter: public DayCounterBase
{
public:
    BusinessDayCounter(string format_);
    virtual ~BusinessDayCounter(){}

    virtual int dayCount(string start, string end) const;
    virtual vector<string> getDates(string start, int num) const;
};

#endif // !DATE_COUNTER_H