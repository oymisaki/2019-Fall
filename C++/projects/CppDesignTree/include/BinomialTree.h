#ifndef BINOMIAL_TREE_H
#define BINOMIAL_TREE_H

#include "TreeProduct.h"
#include "Parameters.h"
#include "Arrays.h"
#include <vector>

class SimpleBinomialTree
{
public:
    SimpleBinomialTree(double Spot_,
                       const Parameters &r_,
                       const Parameters &d_,
                       double Volatility_,
                       unsigned long Steps,
                       double Time);
    double GetThePrice(const TreeProduct &TheProduct);

protected:
    void BuildTree();

private:
    double Spot;
    Parameters r; // insterest rate is variable
    Parameters d; // and the dividend 
    double Volatility;
    unsigned long Steps;
    double Time;
    bool TreeBuilt;

    // use vector of vector to store tree
    // use pair to store both spot and option price
    std::vector<std::vector<std::pair<double, double>>>
        TheTree;

    // discount facters
    MJArray Discounts;
};

#endif // !BINOMIAL_TREE_H