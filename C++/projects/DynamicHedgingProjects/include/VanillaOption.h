#ifndef VANILLA_OPTION_H
#define VANILLA_OPTION_H

#include <string>
using std::string;

class VanillaOption
{
protected:
    double k;      // Strike Price
    double p;      // Option price
    string expire; // Expiration date
public:
    VanillaOption(double k_, double p_, string exp_);
    virtual ~VanillaOption() {}

    inline double getStrike() const { return k; }
    inline double getPrice() const { return p; }
    inline string getExpire() const { return expire; }

    int getDaysToMaturity(string today) const;

    virtual VanillaOption *clone() const = 0;
    virtual double ImpliedVol(double s, double r, string today) const = 0;
};

class VanillaCallOption : public VanillaOption
{
private:

public:
    VanillaCallOption(double k_, double p_, string exp_);
    virtual ~VanillaCallOption() {}

    virtual VanillaOption *clone() const;

    virtual double ImpliedVol(double s, double r, string today) const;
};

class VanillaPutOption : public VanillaOption
{
private:
 
public:
    VanillaPutOption(double k_, double p_, string exp_);
    virtual ~VanillaPutOption() {}

    virtual VanillaOption *clone() const;

    virtual double ImpliedVol(double s, double r, string today) const;
};
#endif // !VANILLA_OPTION_H