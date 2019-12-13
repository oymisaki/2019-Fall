#ifndef BOND_H
#define BOND_H
#include<string>
#include "date.h"

using namespace std;

class Bond
{
private:
    Date d; // Date class store the Date of expiration
    string e; // expiration date in string format
    double f;
    double c;
public:
    Bond();
    ~Bond() = default;
    Bond(string time, double freq, double coupon_rate);
    Bond(const Bond& b);
    string toString() const;
    double price(const double& tm, const double& i) const;
};
#endif