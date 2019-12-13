#ifndef TREE_PRODUCT_H
#define TREE_PRODUCT_H

class TreeProduct
{
private:
    double FinalTime;

public:
    TreeProduct(double FinalTime_);
    ~TreeProduct() {}

    virtual double FinalPayOff(double Spot) const = 0;
    virtual double PreFinalValue(double Spot,
                                 double Time,
                                 double DiscountedFutureValue)
        const = 0;
    virtual TreeProduct *clone() const = 0;

    double GetFinalTime() const;
};


#endif // !TREE_PRODUCT_H