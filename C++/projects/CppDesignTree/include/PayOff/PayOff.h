#ifndef PAYOFF3_H
#define PAYOFF3_H

// NOTE Make PayOff abstract Class further reusable by adding virtual copy function

class PayOff
{
public: 
    PayOff() = default;
    virtual ~PayOff() = default;
    virtual double operator()(double spot) const=0;

    // NOTE return ptr pointed to an cloned *this allocated by new
    virtual PayOff* clone() const=0;

private:
};

class PayOffCall: public PayOff
{
public:
    PayOffCall(double Strike_);
    virtual double operator()(double spot) const;
    virtual ~PayOffCall() = default;
    virtual PayOff* clone() const;

private:
    double Strike;
};

class PayOffPut: public PayOff
{
public:
    PayOffPut(double Strike_);
    virtual double operator()(double spot) const;
    virtual ~PayOffPut() = default;
    virtual PayOff* clone() const;

private:
    double Strike;
};

#endif 