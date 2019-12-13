#include "parse.h"
#include "date.h"
#include <regex>
#include <string>
#include <vector>

using namespace std;

Date parse_date(string date)
{
    if(date == "")
        return Date(0, 0, 0);
    
    try
    {
        // parse date in mm/dd/yyyy format
        regex sep("/");
        sregex_token_iterator tokens(date.cbegin(), date.cend(), sep, -1);
        sregex_token_iterator end;

        int y, m, d;
        if (tokens != end)
            m = stoul(*tokens++);
        if(tokens != end) 
            d = stoul(*tokens++);
        if(tokens != end) 
            y = stoul(*tokens++);
        if (m<=12 && d<=31 && tokens == end)
            return Date(d, m, y); // parse success
        else
            throw "Not a valid date of mm/dd/yyyy format";
    }
    catch (const exception &e)
    {
        throw "Not a valid date of mm/dd/yyyy format";
    }
};

// This function parse double entry in reading csv files
double parse_double(string token)
{
    try
    {
        return stod(token);
    }
    catch (exception)
    {
        throw "Not a valid double entry";
    }
};

int find_index(double tm, vector<double> tm_vec)
{
    int begin = 0;
    int mid = tm_vec.size() / 2;
    int end = tm_vec.size() - 1;
    if(tm > tm_vec[begin] || tm < tm_vec[end])
        throw "time to maturity out of range";

    while(end - begin > 1)
    {
        if(tm_vec[mid]  >= tm)
        {
            begin = mid;
            mid = (begin + end) / 2;
        }
        else if(tm_vec[mid] < tm)
        {
            end = mid;
            mid = (begin + end) / 2;
        }
    }
    return (tm_vec[begin] + tm_vec[end]) / 2 > tm ? end : begin;
}
