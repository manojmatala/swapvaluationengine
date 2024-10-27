// The purpose of this program is to calculate the price of swap
// Date of creation: 10/06/2024
// Last update     : 10/27/2024
// Authors: Arya, Manoj Matala

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <iterator>
#include <fstream>
#include <cmath>
#include <numeric>
#include <iomanip>
#include "rapidcsv.h"
using namespace std;

struct RatePoint {
    float sixMonth, oneYear, oneAndHalfYear, twoYear;
};

class swapratecalculator{
public:
//Takes the rates for a day and initialzes the discount factors for the days
swapratecalculator(RatePoint& asof_rates):discount_factors{calculateDiscountFactors(asof_rates)} {};

RatePoint getDiscountFactors() const {
        return discount_factors;
    }

private:
RatePoint discount_factors;
RatePoint calculateDiscountFactors(const RatePoint& asof_rates) {

    // Main formuala : (1-d4) / (d1+d2+d3+d4)
        RatePoint discount_factors;
        discount_factors.sixMonth = 1 / (1 + (asof_rates.sixMonth / 100) * 0.5f);
        discount_factors.oneYear = 1 / (1 + (asof_rates.oneYear / 100));
        discount_factors.oneAndHalfYear = 1 / (1 + (asof_rates.oneAndHalfYear / 100) * 1.5f);
        discount_factors.twoYear = 1 / (1 + (asof_rates.twoYear / 100) * 2);
        return discount_factors;
    }

};



int main() {
    RatePoint asof_rates;
    rapidcsv::Document ytm_curve("C:/Users/manoj/OneDrive/scripts/inputs/rates.csv", rapidcsv::LabelParams(), rapidcsv::SeparatorParams(','));

    std::vector<std::string> mycolumns = ytm_curve.GetColumnNames();
    
    //print columns if neded
    /*for (const auto& col : mycolumns) {
            std::cout << col << "\t";
        }*/


    // Read data
        std::vector<std::string> dates = ytm_curve.GetColumn<std::string>("Date");
        std::vector<float> sixmonthrates = ytm_curve.GetColumn<float>("6 Mo");
        std::vector<float> oneyearrates = ytm_curve.GetColumn<float>("1 Yr");
        std::vector<float> oneandhalfyearRates = ytm_curve.GetColumn<float>("1.5 Yr");
        std::vector<float> twoyearrates = ytm_curve.GetColumn<float>("2 Yr");
       
        std::string mydate = "10/18/2024";
        std::cout << "Using date: " << mydate << std::endl;

        // Display header row for output
        std::cout << "\nDate\t\t6M\t1 Yr\t1.5 Yr\t2 Yr\n";
        std::cout << "-------------------------------------------\n";
        
        auto it = std::find(dates.begin(), dates.end(), mydate);
        if (it != dates.end()) {
            size_t row = std::distance(dates.begin(), it);
            std::cout << std::fixed << std::setprecision(4);
            std::cout << dates[row] << "\t" 
                      << sixmonthrates[row] << "\t" 
                      << oneyearrates[row] << "\t" 
                      << oneandhalfyearRates[row] << "\t"
                      << twoyearrates[row] << std::endl;
            
            asof_rates = {sixmonthrates[row], oneyearrates[row], 
                          oneandhalfyearRates[row], twoyearrates[row]};
        } else {
            std::cout << "Date not found in the data." << std::endl;
        }

    
    std::cout << "\nAsof Rates (inputs):\n";
     
    std::cout  << " 6 month: " << std::setw(10) << asof_rates.sixMonth << "%\n"
              << "1.0 Year: " << std::setw(10) << asof_rates.oneYear << "%\n"
              << "1.5 Year: " << std::setw(10) << asof_rates.oneAndHalfYear << "%\n"
              << "2.0 Year: " << std::setw(10) << asof_rates.twoYear << "%\n";
    
    swapratecalculator swapdiscountfactors(asof_rates);

    RatePoint discount_factors = swapdiscountfactors.getDiscountFactors();
    std::cout << "-------------------------------------------\n";
     std::cout << "Discount Factors Calculated:\n";
    std::cout << " 6 Month: " << std::setw(10) << discount_factors.sixMonth << "\n"
              << "1.0 Year: " << std::setw(10) << discount_factors.oneYear << "\n"
              << "1.5 Year: " << std::setw(10) << discount_factors.oneAndHalfYear << "\n"
              << "2.0 Year: " << std::setw(10) << discount_factors.twoYear << std::endl;
    
    float swap_rate_semi,swap_rate_annualized, denominator;
    denominator = (discount_factors.sixMonth+discount_factors.oneYear+discount_factors.oneAndHalfYear+discount_factors.twoYear);
    
    std::cout << "-----------------Final Output--------------------------\n";
    swap_rate_semi = (1-discount_factors.twoYear)/(denominator);
    swap_rate_annualized = swap_rate_semi*2;
    cout << "Swap rate_semiannual: " << swap_rate_semi*100 << "%" <<endl;
    cout << "Swap rate_annualized: " << swap_rate_annualized*100 << "%" << endl;
    return 0;
}