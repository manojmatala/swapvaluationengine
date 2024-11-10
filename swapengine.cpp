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
    float threemonth, sixmonth, ninemonth, oneyear;
};

class swapratecalculator{
public:
//Takes the rates for a day and initialzes the discount factors for the days
swapratecalculator(RatePoint &asof_rates):discount_factors{calculateDiscountFactors(asof_rates)} {};

RatePoint getDiscountFactors() const {
        return discount_factors;
    }

private:
RatePoint discount_factors;
RatePoint calculateDiscountFactors(const RatePoint& asof_rates) {

    // Main formuala : (1-d4) / (d1+d2+d3+d4)
        RatePoint discount_factors;
        discount_factors.threemonth = 1 / (1 + (asof_rates.threemonth/ 100) * 0.25f);
        discount_factors.sixmonth = 1 / (1 + (asof_rates.sixmonth / 100)*0.5);
        discount_factors.ninemonth = 1 / (1 + (asof_rates.ninemonth / 100) * 0.75f);
        discount_factors.oneyear = 1 / (1 + (asof_rates.oneyear / 100) * 1);
        return discount_factors;
    }

};


vector <double> SOFR_curve(double sofr_today){
    vector <double> sofr_curve_rates ;
    double days{90};
    double cumulative_rate_period, cumulative_rate_period_per,annualized_rate;
    double daily_rate = sofr_today/(100*360);
    for (int i=1;i<=4;++i){
      cumulative_rate_period = pow( (1+ daily_rate), days);
      cumulative_rate_period_per = (cumulative_rate_period-1);
      annualized_rate = cumulative_rate_period_per*(360/days);
      sofr_curve_rates.push_back(annualized_rate*100);
      //cout << "SOFR " << days << " rates is: " << cumulative_rate_period << "\t" << "Cumulative rate per period " <<
      //cumulative_rate_period_per << "Factor: " << (360/days) << endl; 
      days += 90;
      
    }

    
    return (sofr_curve_rates);
}



int main() {
    RatePoint asof_rates,asof_rates_sofr;
    rapidcsv::Document ytm_curve("C:/Users/manoj/OneDrive/scripts/inputs/daily-treasury-rates.csv", rapidcsv::LabelParams(), rapidcsv::SeparatorParams(','));
    //source: https://home.treasury.gov/resource-center/data-chart-center/interest-rates/TextView?type=daily_treasury_yield_curve&field_tdr_date_value=2024

    std::vector<std::string> mycolumns = ytm_curve.GetColumnNames();
    
    //print columns if neded
    /*for (const auto& col : mycolumns) {
            std::cout << col << "\t";
        }*/


    // Read data
        std::vector<std::string> dates = ytm_curve.GetColumn<std::string>("Date");
        std::vector<float> threemonthrates = ytm_curve.GetColumn<float>("3 Mo");
        std::vector<float> sixmonthrates = ytm_curve.GetColumn<float>("6 Mo");
        std::vector<float> oneyearrates = ytm_curve.GetColumn<float>("1 Yr");
        //std::vector<float> oneandhalfyearRates = ytm_curve.GetColumn<float>("1.5 Yr");
        std::vector<float> twoyearrates = ytm_curve.GetColumn<float>("2 Yr");
        

 
       
        std::string mydate = "10/18/2024";
        std::cout << "Using date: " << mydate << std::endl;

        // Display header row for output
        std::cout << "\nDate\t\t3M\t6M\t9M\t1 Yr\t2 Yr\n";
        std::cout << "---------------GSec-Curve----------------------------\n";
        // References: https://www.cmegroup.com/market-data/cme-group-benchmark-administration/term-sofr.html 
        
        auto it = std::find(dates.begin(), dates.end(), mydate);
        float ninemonthrate;
               
        if (it != dates.end()) {
            size_t row = std::distance(dates.begin(), it);
            ninemonthrate = sixmonthrates[row] + (oneyearrates[row] - sixmonthrates[row]) / 2;
            
            std::cout << std::fixed << std::setprecision(4);
            std::cout << dates[row] << "\t" 
                      << threemonthrates[row] << "\t"
                      << sixmonthrates[row] << "\t" 
                      << ninemonthrate << "\t" 
                      << oneyearrates[row] << std::endl;
            
            asof_rates = {threemonthrates[row], sixmonthrates[row], ninemonthrate,
                          oneyearrates[row] };
        } else {
            std::cout << "Date not found in the data." << std::endl;
        }

    
    std::cout << "\nAsof Rates (inputs):\n";
     
    std::cout  << "3 month: " << std::setw(10) << asof_rates.threemonth << "%\n"
              << "6 month: " << std::setw(10) << asof_rates.sixmonth << "%\n"
              << "9 month: " << std::setw(10) << asof_rates.ninemonth << "%\n"
              << "1 year:  " << std::setw(10) << asof_rates.oneyear << "%\n";
    
    swapratecalculator gsecdiscountfactors(asof_rates);

    RatePoint discount_factors = gsecdiscountfactors.getDiscountFactors();
    std::cout << "-------------------------------------------\n";
     std::cout << "Discount Factors Calculated Gsec:\n";
    std::cout << "3 Month: " << std::setw(10) << discount_factors.threemonth << "\n"
              << "6 Month: " << std::setw(10) << discount_factors.sixmonth << "\n"
              << "9 Month: " << std::setw(10) << discount_factors.ninemonth << "\n"
              << "1 Year : " << std::setw(10) << discount_factors.oneyear << std::endl;
    
    float swap_rate_semi,swap_rate_annualized, denominator;
    denominator = (discount_factors.threemonth+discount_factors.sixmonth+discount_factors.ninemonth+discount_factors.oneyear);
    
   std::cout << "-----------------Building SOFR curve--------------------------\n";

    cout << "Current SOFR rate: as of (10/27/2024) : 4.81% " << endl;
    double sofr_today= 4.81;
 
    //Building sofr curve over 1 year
    vector <double> sofr_rates = SOFR_curve(sofr_today);
    int count{3};
    for (auto i: sofr_rates){
        cout <<"SOFR " << count << "M: "<< i <<"%" << "\n";
        count +=3;
    }
    asof_rates_sofr = {sofr_rates[0], sofr_rates[1], sofr_rates[2],
                          sofr_rates[3] };
    swapratecalculator swapdiscountfactors(asof_rates_sofr);
    RatePoint discount_factors_swap = swapdiscountfactors.getDiscountFactors();
                          
    std::cout << "\n-----------------Final Output--------------------------\n";
    double swap_denominator;
    swap_denominator = (discount_factors_swap.threemonth+discount_factors_swap.sixmonth+discount_factors_swap.ninemonth+discount_factors_swap.oneyear);

    double swap_rate_quarterly = (1-discount_factors_swap.oneyear)/(swap_denominator);
    swap_rate_annualized = swap_rate_quarterly*4;
    cout << "Swap rate_quarterly: " << swap_rate_quarterly*100 << "%" <<endl;
    cout << "Swap rate_annualized: " << swap_rate_annualized*100 << "%" << endl;
    
    
    
    return 0;
}