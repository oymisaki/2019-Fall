#include <iostream>
// #include "OptionPrice.h"
#include "exotic.h"

using namespace std;

int main()
{

    // OptionPrice op(100, 30, 0.01, 100, 0.01);
    // cout << "Binominal Option Price: " << op.binominal_option_price(100.0, 0.05, 62.0, 1.2840, 0.77881, 0.5, 4.0, 4, OptionType::call) << endl;
    // cout << "Black Scholes Option Price: " << op.black_scholes_option_price(op.get_strike(), op.get_spot(), op.get_risk_free_rate(), op.get_time_to_maturity(), op.get_volatility(), OptionType::put) << endl;
    // cout << "Binominal American Option Price: " << op.binominal_american_option_price(100.0, 0.05, 100.0, 1.2840, 0.77881, 0.5, 4.0, 4, OptionType::call) << endl;
    cout << "Exotic Option Price: " << exotic_option_price(60.0, 0.025, 62.0, 1.05943423696, 0.9439000224, 5.0/12) << endl;
   
    return 0;
}