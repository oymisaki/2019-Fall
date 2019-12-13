#ifndef TREE_EUROPEAN_H
#define TREE_EUROPEAN_H

#include "TreeProduct.h"
#include "PayOffBridge.h"

class TreeEuropean : public TreeProduct
{
public:
    TreeEuropean(double FinalTime,
                 const PayOffBridge &ThePayOff_);

    ~TreeEuropean(){};

    virtual TreeProduct *clone() const;
    virtual double FinalPayOff(double Spot) const;
    virtual double PreFinalValue(double Spot,
                                 double Time,
                                 double DiscountedFutureValue) const;
    virtual ~TreeEuropean() {}

private:
    PayOffBridge ThePayOff;
};

#endif // !TREE_EUROPEAN_H