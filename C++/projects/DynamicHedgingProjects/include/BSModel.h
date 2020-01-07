#ifndef BS_MODEL_H
#define BS_MODEL_H

// base abstract BSModel class  
class BSModel
{
private:
    
public:
    BSModel() {}
    // virtual deconstructor
    virtual ~BSModel() {}
    
    // virtual copy function
    virtual BSModel* clone() const = 0;

    // option prices
    virtual double operator()(double s, double k, double r, double t, double sigma) const = 0;
    // delta
    virtual double delta(double s, double k, double r, double t, double sigma) const = 0;
    // vage
    virtual double vega(double s, double k, double r, double t, double sigma) const;
    // implied volatility
    virtual double impliedVol(double s, double k, double r, double t, double price) const = 0;
};

class BSModelCall: public BSModel
{
private:

public:
    BSModelCall() {}
    virtual ~BSModelCall() {}
    
    virtual BSModel* clone() const;

    virtual double operator()(double s, double k, double r, double t, double sigma) const;
    virtual double delta(double s, double k, double r, double t, double sigma) const;

    virtual double impliedVol(double s, double k, double r, double t, double price) const;
};

class BSModelPut: public BSModel
{
private:

public:
    BSModelPut() {}
    virtual ~BSModelPut() {}
    
    virtual BSModel* clone() const;

    virtual double operator()(double s, double k, double r, double t, double sigma) const;
    virtual double delta(double s, double k, double r, double t, double sigma) const;

    virtual double impliedVol(double s, double k, double r, double t, double price) const;
};




#endif // !BS_MODEL_H