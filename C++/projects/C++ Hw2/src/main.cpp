#include <iostream>
#include <string>
#include <math.h>
#include <fstream>
#include <vector>
#include <regex>
#include "bond.h"
#include "date.h"
#include "parse.h"

using namespace std;

/* ----------- Below is the answer function to question 3 ----------- */
double answer_question_3(const vector<double> &tm_vec, const vector<double> &i_vec)
{
    Date issued(3, 8, 2015);
    Date obs_dates[] = {Date(1, 1,2016), Date(1, 1,2017), Date(1, 1,2018), Date(1, 1,2019), Date(1, 1,2020)};
    Date expiration(31, 12, 2020);

    Bond bond("01/01/2021", 0.5, 0.05);  
    double prices[] = {0.0, 0.0, 0.0, 0.0, 0.0};

    // Calculate prices of bond on each date 
    for (int i = 0; i < 5; i++)
    {
        double time2maturity = static_cast<double>(get_difference(obs_dates[i], expiration)) / 365;
        // use bisection method to search for the required time to maturity
        int idx = find_index(time2maturity, tm_vec);
        prices[i] = bond.price(tm_vec[idx], i_vec[idx]);
    }
    
    double time2maturity = static_cast<double>(get_difference(issued, expiration)) / 365;
    int idx = find_index(time2maturity, tm_vec);

    double payment = 0;
    for (int i = 0; i < 5; i++)
        payment += prices[i];
    payment /= 5;

    return payment * exp(-time2maturity * i_vec[idx]);
}

/* ----------- Below is the definition of Class Bond ----------- */

// Default Constructor
Bond::Bond() : d(0, 0, 0), e(""), f(0), c(0) {}

// Constructor with parameters
Bond::Bond(string time, double freq, double coupon_rate) : f(freq), c(coupon_rate)
{
    try
    {
        this->d = parse_date(time);
        this->e = time;
    }
    catch (const char *msg)
    {
        e = "";
        cerr << msg << endl;
    }
}

// Copy Constructor
Bond::Bond(const Bond &b)
{
    this->d = b.d;
    this->e = b.e;
    this->f = b.f;
    this->c = b.c;
}

string Bond::toString() const
{
    string ret = "Bond(" + this->e + ", " + to_string(this->f) + ", " + to_string(this->c) + ")";
    return ret;
}

// This price member function
double Bond::price(const double& tm, const double& i) const
{
    if (tm < 0)
        throw "Time to Maturity Less Than 0";

    double n = 1;
    double pv = 0;
    // Calculate the pv using the example in Excel file
    for (; tm > this->f * n; n++)
    {
        pv += 100 * this->c * this->f * exp(-i * this->f * n);
    }
    pv += (100 + 100 * this->c *(tm - this->f * --n)) * exp(-i * tm);
    return pv;
}

/* ----------- END of the definition of Class Bond ----------- */

// This function automatically run test cases
int run_test_cases()
{
    cout << "******** Running Test Cases *********" <<endl;
    Bond b;
    Bond b2("11/19/2035", 0.5, 0.08);
    Bond b3(b2);

    cout <<"Test default Constructor: "<< b.toString() << endl;
    cout <<"Test Constructor: "<< b2.toString() << endl;
    cout <<"Test Copy Constructor: "<< b3.toString() << endl;
    cout <<"Test Price Function: "<< b2.price(4.2, 0.07) << endl;
    cout << "***** All Tests Cases Finished ******\n" <<endl;
    return 0;
}

int main()
{
    run_test_cases();

    vector<double> tm_vec;
    vector<double> i_vec;

    // reading data file
    const char *path = "./Bond_Ex3.csv";

    ifstream infile(path);
    
    string line;
    cout << "************ Reading File ***********" <<endl;
    std::printf("Good day!\nReading file in path %s\n", path);
    for (int line_num = 0; getline(infile, line); ++line_num)
    {
        // parse line into string tokens separated by comma, mind the "" escaper
        regex separator(",");
        sregex_token_iterator tokens(line.cbegin(), line.cend(), separator, -1);

        try
        {
            tm_vec.push_back(parse_double(*tokens++));
            i_vec.push_back(parse_double(*tokens));
        }
        catch (const char *msg)
        {
            // delete invalid data stored, skip invalid data line
            tm_vec.size() > i_vec.size() ? tm_vec.pop_back()
                                         : (tm_vec.size() < i_vec.size() ? i_vec.pop_back() : []() {}());
            std::printf("Skipped invalid entry at line %d. %s\n", line_num, msg);
        }
    }
    cout << "********** Reading Finished *********\n" <<endl;

    cout << "******** Answer for Question 3 ******" <<endl;
    
    double payment = answer_question_3(tm_vec, i_vec);
    string con = payment > 98 ? "good": "bad";

    cout <<"Payment of Question 3 is: $"<< payment << ".\nSo it is a "<< con << " investment!"<<endl;
    cout << "**************** End ****************" <<endl;
    return 0;
}