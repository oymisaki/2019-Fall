#include "DayCounter.h"

DayCounterBase::DayCounterBase(string format_)
    : format(format_)
{
}

BusinessDayCounter::BusinessDayCounter(string format_)
    : DayCounterBase(format_)
{
}

// if not use quantlib library 
#define NO_QUANT_LIB
#if defined(NO_QUANT_LIB)

#include <boost/date_time.hpp>
using namespace boost::gregorian;

int BusinessDayCounter::dayCount(string start, string end) const
{
    date _start_date(from_simple_string(start));
    date _end_date(from_simple_string(end)); // counter for weekdays
    int count = 0;
    for (day_iterator iter = _start_date; iter != _end_date; ++iter)
    {
        // increment counter if it's no saturday and no sunday
        if (iter->day_of_week() != boost::date_time::Saturday && iter->day_of_week() != boost::date_time::Sunday)
            ++count;
    }
    return count;
}

vector<string> BusinessDayCounter::getDates(string start, int num) const
{
    date _start_date(from_simple_string(start));
    vector<string> dates;
    int count = 0;
    day_iterator iter = _start_date;
    while (count < num)
    {
        if (iter->day_of_week() != boost::date_time::Saturday && iter->day_of_week() != boost::date_time::Sunday)
        {
            ++count;
            dates.push_back(to_iso_extended_string(*iter));
        }
        else
            ++iter;
    }

    return dates;
}

#else
// if use the quantlib library

#include <ql/utilities/dataparsers.hpp>
#include <ql/time/daycounters/business252.hpp>
#include <ql/time/calendars/unitedstates.hpp>

using namespace QuantLib;

int BusinessDayCounter::dayCount(string start, string end) const
{
    Date s = DateParser::parseFormatted(start, format);
    Date e = DateParser::parseFormatted(end, format);

    Calendar us = UnitedStates();

    return us.businessDaysBetween(s, e) + 1;
}

vector<string> BusinessDayCounter::getDates(string start, int num) const
{
    using QuantLib::io::iso_date;

    Date s = DateParser::parseFormatted(start, format);

    Calendar us = UnitedStates();
    vector<string> dates;
    int count = 0;
    while (count < num)
    {
        if (us.isBusinessDay(s))
        {
            std::ostringstream out;
            out << iso_date(s);
            dates.push_back(out.str());
            s++;
            count++;
        }
        else
            s++;
    }

    return dates;
}

#endif