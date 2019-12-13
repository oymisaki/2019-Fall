#include <iostream>
#include "OptionPrice.h"

using namespace std;

int main()
{
    double k, s, r, t, sigma;
    char flag;
    int periods;
    cout << "******************************************** input *******************************************" << endl;

    while (cout << "Enter Strike Price (Double): \n" && cin >> k &&
           cout << "Enter Spot (Double): \n" && cin >> s &&
           cout << "Enter Interest Rate (Double): \n" && cin >> r &&
           cout << "Enter Sigma (Double): \n" && cin >> sigma &&
           cout << "Enter Time to Maturity (Double): \n" && cin >> t &&
           cout << "Enter Binominal Tree Periods (Integer): \n" && cin >> periods &&
           cout << "Enter Option Type Flag ('p' 'P' or 'c' 'C'): \n" && cin >> flag)
    {

        // handling both lower and upper case flag
        while (static_cast<char>(tolower(flag)) != 'p' && static_cast<char>(tolower(flag)) != 'c')
        {
            cerr << "Unexpected Input: " << flag <<endl;
            cout << "Enter Option Type Flag ('p' 'P' or 'c' 'C'): \n";
            cin >> flag;
        }

        cout << "******************************************* output *******************************************" << endl;
        OptionType type = static_cast<char>(tolower(flag)) == 'c' ? OptionType::call : OptionType::put;
        OptionPrice op(k, s, r, t, sigma, type);
        cout << "Binominal European Option Price: " << op.binominal_option_price(periods) << endl;
        cout << "Black Scholes Option Price: " << op.black_scholes_option_price() << endl;
        cout << "Binominal American Option Price: " << op.binominal_american_option_price(periods) << endl;
        cout << "******************************************** input *******************************************" << endl;
    }

    return 0;
}