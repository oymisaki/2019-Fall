#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <regex>
#include <iterator>
#include <numeric>

using namespace std;

string parse_date(string date)
{
    try
    {
        // parse date in yyyy-mm format
        regex sep("-");
        sregex_token_iterator tokens(date.cbegin(), date.cend(), sep, -1);
        sregex_token_iterator end;

        stoi(*tokens++);
        if (tokens != end && 1 <= stoi(*tokens++) <= 12 && tokens == end)
            return date; // parse success
        else
            throw "Not a valid date of yyyy-mm format";
    }
    catch (const exception &e)
    {
        throw "Not a valid date of yyyy-mm format";
    }
};

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

double average(vector<double> v)
{
    return accumulate(v.begin(), v.end(), 0) / v.size();
}

double find_rate(vector<double> rate_vec, vector<string> date_vec, string date)
{
    // code for finding the Baa rate for the
    // given date (in yyyy-mm format)
    // and returning the rate for that month
    vector<string>::iterator date_iter = date_vec.begin();
    vector<double>::iterator rate_iter = rate_vec.begin();
    while (date_iter != date_vec.end())
    {
        if (*date_iter == date)
            return *rate_iter;
        ++date_iter;
        ++rate_iter;
    }
    throw "Your date out of range";
}

int main()
{
    vector<double> rate;
    vector<string> date;
    const char* path = "./hw1_H.15_Baa_Data.csv";
    ifstream infile(path);

    // code for loading rate and date vectors from the file hw1_H.15_Baa_Data.csv
    // the headers should be handled properly. do not delete them manually

    string line;
    std::printf("Good day!\nReading file in path %s\n", path);
    for (int line_num = 0; getline(infile, line); ++line_num)
    {
        // parse line into string tokens separated by comma, mind the "" escaper
        regex separator(",(?=(?:[^\"]*\"[^\"]*\")*(?![^\"]*\"))");
        sregex_token_iterator tokens(line.cbegin(), line.cend(), separator, -1);

        try
        {
            date.push_back(parse_date(*tokens++)); // store the token into date vector
            rate.push_back(parse_double(*tokens)); // convert token into type double and store
        }
        catch (const char *msg)
        {
            // delete invalid data stored, skip invalid data line
            date.size() > rate.size() ? date.pop_back()
                                      : (date.size() < rate.size() ? rate.pop_back() : []() {}());
            std::printf("Skipped invalid entry at line %d, %s\n", line_num, msg);
        }
    }

    double average_rate = average(rate);
    string input_date;

    // user interface
    std::printf("Please enter a date in yyyy-mm format: \n");
    for (; cin >> input_date; std::printf("Please enter a date in yyyy-mm format: \n"))
    {
        try
        {
            input_date = parse_date(input_date);
            double diff = find_rate(rate, date, input_date) - average_rate;
            std::printf("The average rate is %.2f. Difference between the rate on %s and the average rate is %.2f \n",
                        average_rate,
                        input_date.c_str(),
                        diff);
        }
        catch (const char *msg)
        {
            cerr << msg << '\n';
        }
    }

    return 0.0; // program end
}
