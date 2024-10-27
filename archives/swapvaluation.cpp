// The puprose of this program is to calculate the price of swap
// Date of creation: 10/06/2024
// Authors: Arya, Manoj 

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <iterator>
#include <fstream>
#include <numeric>
#include "rapidcsv.h"
#include <iomanip>
using namespace std;


/*//class for swap valuation: This calculates the MTM value of a swap 
class valuation{
    
}


//Class to calculates the swap rate. Swap rate is the interest rate paid by the fixed rate payer in a interest rate swap 
class swap rate{
/*Typical SWAP payment standards:
            fixed rate payer    makes semiannual payments
            floating rate payer makes quarterly payments. Based on the reference rate that gets reset every quarater.  
            common tenors      : 2,5,10,and 30 years
*/ 



int main() {
        
    std::vector<float> asof_rates;
    try {
        // Update with the correct file path and set delimiter to comma
        rapidcsv::Document ytm_curve("C:/Users/manoj/OneDrive/scripts/inputs/rates.csv", rapidcsv::LabelParams(), rapidcsv::SeparatorParams(','));
        

        std::vector <std::string> mycolumns = ytm_curve.GetColumnNames();
        for (auto& col: mycolumns){
            std::cout << col << "\t" << col.length() << std::endl;
        }

        // Try reading columns by exact names in the header row
        std::vector<std::string> dates = ytm_curve.GetColumn<std::string>("Date");
        std::vector<float> sixmonthrates = ytm_curve.GetColumn<float>("6 Mo");
        std::vector<float> onemonthrates = ytm_curve.GetColumn<float>("1 Mo");
        std::vector<float> oneyearrates = ytm_curve.GetColumn<float>("1 Yr");
        std::vector<float> oneandhalfyearRates = ytm_curve.GetColumn<float>("1.5 Yr");
        std::vector<float> twoyearrates = ytm_curve.GetColumn<float>("2 Yr");
       
        std::cout << "Enter the date of the curve: ";
        std::string mydate;
        mydate="10/25/2024";
        // Display header row for output
        std::cout << "\nDate\t6M\t1 Yr\t1.5 Yr\t2 Yr\n";
        std::cout << "-------------------------------------------\n";
        for (size_t i = 0; i < twoyearrates.size(); ++i) {
            
            if (dates[i]==mydate){
            std::cout << dates[i] << "\t" << sixmonthrates[i] << "\t" << oneyearrates[i] << "\t" << oneandhalfyearRates[i] << "\t"
                    << twoyearrates[i]
                    << std::endl;
            asof_rates.insert(asof_rates.end(), {sixmonthrates[i], oneyearrates[i], oneandhalfyearRates[i], twoyearrates[i]});
            break;
            }
        }

    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }

    /*std::cout << "Asof Rates:\n";
    for (const auto& rate : asof_rates) {
            std::cout << rate << std::endl;
        }*/
    std::cout << "\nAsof Rates:\n";
    for (const auto& rate : asof_rates) {
        std::cout << rate << std::endl;
    }

    return 0;
}