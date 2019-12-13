#ifndef PARSE_H
#define PARSE_H
#include <string>
#include <vector>
#include "date.h"

using namespace std;

double parse_double(string token);
Date parse_date(string date);
int find_index(double tm, vector<double> tm_vec);

#endif // !PARSE_H
