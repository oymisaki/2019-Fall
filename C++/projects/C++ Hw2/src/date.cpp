#include "date.h"

using namespace std; 
  
Date::Date(int d, int m, int y): d(d), m(m), y(y){}

// To store number of days in all months from January to Dec. 
const int month_days[12] = {31, 28, 31, 30, 31, 30, 
                           31, 31, 30, 31, 30, 31}; 
  
// This function counts number of leap years before the 
// given date 
int count_leap_years(Date d) 
{ 
    int years = d.y; 
  
    // Check if the current year needs to be considered 
    // for the count of leap years or not 
    if (d.m <= 2) 
        years--; 
  
    // An year is a leap year if it is a multiple of 4, 
    // multiple of 400 and not a multiple of 100. 
    return years / 4 - years / 100 + years / 400; 
} 
  
// This function returns number of days between two given 
// dates 
int get_difference(Date dt1, Date dt2) 
{ 
    // COUNT TOTAL NUMBER OF DAYS BEFORE FIRST DATE 'dt1' 
  
    // initialize count using years and day 
    long int n1 = dt1.y*365 + dt1.d; 
  
    // Add days for months in given date 
    for (int i=0; i<dt1.m - 1; i++) 
        n1 += month_days[i]; 
  
    // Since every leap year is of 366 days, 
    // Add a day for every leap year 
    n1 += count_leap_years(dt1); 
  
    // SIMILARLY, COUNT TOTAL NUMBER OF DAYS BEFORE 'dt2' 
  
    long int n2 = dt2.y*365 + dt2.d; 
    for (int i=0; i<dt2.m - 1; i++) 
        n2 += month_days[i]; 
    n2 += count_leap_years(dt2); 
  
    // return difference between two counts 
    return (n2 - n1); 
} 