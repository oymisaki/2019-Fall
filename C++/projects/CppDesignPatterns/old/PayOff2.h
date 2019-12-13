#ifndef PAYOFF2_H
#define PAYOFF2_H

// NOTE -------------------------- Abstract PayOff class with better design -------------------------- //

class PayOff
{
public: 
    PayOff() = default;
    virtual ~PayOff() = default;
    virtual double operator()(double spot) const=0;

private:
};

class PayOffCall: public PayOff
{
public:
    PayOffCall(double Strike_);
    virtual double operator()(double spot) const;
    virtual ~PayOffCall() = default;

private:
    double Strike;
};

class PayOffPut: public PayOff
{
public:
    PayOffPut(double Strike_);
    virtual double operator()(double spot) const;
    virtual ~PayOffPut() = default;

private:
    double Strike;
};

#endif 