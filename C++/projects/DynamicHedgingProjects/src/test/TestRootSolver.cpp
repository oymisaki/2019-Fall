#include "Test.h"
#include "RootSolver.h"

class CubRoot
{
private:

public:
    double operator()(double x)
    {
        return x * x * x - 8;
    }
};

class DerivCubRoot
{
public:
    double operator()(double x)
    {
        return 3 * x * x;
    }
};

void testRootSolver()
{
    cout << "============================================" << endl;
    cout << "Testing: " << BOOST_CURRENT_FUNCTION << endl;
    CubRoot func;
    DerivCubRoot deriv;
    DerivFunctor<CubRoot, DerivCubRoot> functor(func, deriv);
    try
    {
        cout<<"RootSolver::NewtonRaphson: ";
        cout<<((NewtonRaphson(functor, 3.0, 0.0, 10.0,10, 20) == 2.0) ? "Success": "Failure")<<endl;
    }
    catch(const std::exception& e)
    {
        cout<<"Failure"<<endl;
    }
    
}