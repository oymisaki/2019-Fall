#ifndef TEST_H
#define TEST_H

#include <boost/current_function.hpp>
#include <iostream>
using namespace std;

// self designed simple unit test modules

void testTable();

void testRandomNormal();

void testDiffusionStockSamplePath();

void testBSModel();

void testRootSolver();

void testVanillaOption();

void testResultGatherer();

void testDayCounter();

void testRoutine();

template <typename T, typename T1>
void testClassFuncNoParameter(T &x, T1 fp)
{
    try
    {
        (x.*fp)();
        cout << "Success"<<endl;
    }
    catch (const exception& ex)
    {
        cout << "Failure: " << ex.what() <<endl;
    } 
    catch (const std::string& ex) 
    {
        cout << "Failure: " << ex <<endl;
    }
    catch(...)
    {
        cout << "Failure: " << "Unknown Error" <<endl;
    }
}

template <typename T, typename T1, typename T2>
void testClassFuncOneParameter(T &x, T1 fp, T2 param)
{
    try
    {
        (x.*fp)(param);
        cout << "Success" << endl;
    }
    catch (const exception& ex)
    {
        cout << "Failure: " << ex.what() <<endl;
    } 
    catch (const std::string& ex) 
    {
        cout << "Failure: " << ex <<endl;
    }
    catch(...)
    {
        cout << "Failure: " << "Unknown Error" <<endl;
    }
}

template <typename T, typename T1, typename T2, typename T3>
void testClassFuncTwoParameter(T &x, T1 fp, T2 param1, T3 param2)
{
    try
    {
        (x.*fp)(param1, param2);
        cout << "Success" << endl;
    }
    catch (const exception& ex)
    {
        cout << "Failure: " << ex.what() <<endl;
    } 
    catch (const std::string& ex) 
    {
        cout << "Failure: " << ex <<endl;
    }
    catch(...)
    {
        cout << "Failure: " << "Unknown Error" <<endl;
    }
}

template <typename T, typename T1, typename T2, typename T3, typename T4>
void testClassFuncThreeParameter(T &x, T1 fp, T2 param1, T3 param2, T4 param3)
{
    try
    {
        (x.*fp)(param1, param2, param3);
        cout << "Success" << endl;
    }
    catch (const exception& ex)
    {
        cout << "Failure: " << ex.what() <<endl;
    } 
    catch (const std::string& ex) 
    {
        cout << "Failure: " << ex <<endl;
    }
    catch(...)
    {
        cout << "Failure: " << "Unknown Error" <<endl;
    }
}

template <typename T, typename T1, typename T2, typename T3, typename T4, typename T5>
void testClassFuncFourParameter(T &x, T1 fp, T2 param1, T3 param2, T4 param3, T5 param4)
{
    try
    {
        (x.*fp)(param1, param2, param3, param4);
        cout << "Success" << endl;
    }
    catch (const exception& ex)
    {
        cout << "Failure: " << ex.what() <<endl;
    } 
    catch (const std::string& ex) 
    {
        cout << "Failure: " << ex <<endl;
    }
    catch(...)
    {
        cout << "Failure: " << "Unknown Error" <<endl;
    }
}

template <typename T, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6>
void testClassFuncFiveParameter(T &x, T1 fp, T2 param1, T3 param2, T4 param3, T5 param4, T6 param5)
{
    try
    {
        (x.*fp)(param1, param2, param3, param4, param5);
        cout << "Success" << endl;
    }
    catch (const exception& ex)
    {
        cout << "Failure: " << ex.what() <<endl;
    } 
    catch (const std::string& ex) 
    {
        cout << "Failure: " << ex <<endl;
    }
    catch(...)
    {
        cout << "Failure: " << "Unknown Error" <<endl;
    }
}

#endif //!TEST_H
