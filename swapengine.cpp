// The purpose of this program is to calculate the price of swap
// Date of creation: 10/06/2024
// Last update     : 12/08/2024
// Authors: Arya, Manoj Matala

#include "hjmsimulation.h"
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

 RatePoint build_sofr_curve(float sofr_today){
    RatePoint asof_rates_sofr ;
    //Building sofr curve over 1 year
    vector <float> sofr_rates = SOFR_curve(sofr_today);
    int count{3};
    for (auto i: sofr_rates){
        cout <<"SOFR " << count << "M: "<< i <<"%" << "\n";
        count +=3;
    }
    asof_rates_sofr = {sofr_rates[0], sofr_rates[1], sofr_rates[2],
                          sofr_rates[3] };

    return (asof_rates_sofr);
 }

 vector <float> SOFR_curve(float sofr_today){
    vector <float> sofr_curve_rates ;
    float days{90};
    float cumulative_rate_period, cumulative_rate_period_per,annualized_rate;
    float daily_rate = sofr_today/(100*360);
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



RatePoint print ( std::string mydate, std::vector<std::string> dates,std::vector<float> threemonthrates,std::vector<float> sixmonthrates,std::vector<float> oneyearrates,std::vector<float> twoyearrates){
        
        RatePoint asof_rates;
        auto it = std::find(dates.begin(), dates.end(), mydate);
        float ninemonthrate;
        // Display header row for output
        std::cout << "\nDate\t\t3M\t6M\t9M\t1 Yr\n";
        std::cout << "---------------GSec-Curve----------------------------\n";
        // References: https://www.cmegroup.com/market-data/cme-group-benchmark-administration/term-sofr.html 
               
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
         return(asof_rates);
}


int main() {
    std::cout << "Program started successfully!" << std::endl;
    RatePoint asof_rates,asof_rates_sofr,asof_rates_valuation;
    rapidcsv::Document ytm_curve("./inputs/daily-treasury-rates.csv", rapidcsv::LabelParams(), rapidcsv::SeparatorParams(','));
    //source: https://home.treasury.gov/resource-center/data-chart-center/interest-rates/TextView?type=daily_treasury_yield_curve&field_tdr_date_value=2024

    std::vector<std::string> mycolumns = ytm_curve.GetColumnNames();
    

    // Read data vertically column wise. 
    std::vector<std::string> dates = ytm_curve.GetColumn<std::string>("Date");
    std::vector<float> threemonthrates = ytm_curve.GetColumn<float>("3 Mo");
    std::vector<float> sixmonthrates = ytm_curve.GetColumn<float>("6 Mo");
    std::vector<float> oneyearrates = ytm_curve.GetColumn<float>("1 Yr");
    //std::vector<float> oneandhalfyearRates = ytm_curve.GetColumn<float>("1.5 Yr");
    std::vector<float> twoyearrates = ytm_curve.GetColumn<float>("2 Yr");
        

 
       
    std::string mydate = "10/28/2024";
    std::cout << "Using date: " << mydate << std::endl;


        
  
    asof_rates = print(mydate,dates,threemonthrates,sixmonthrates,oneyearrates,twoyearrates);
 
     
   
    swapratecalculator asofdate{asof_rates};

    RatePoint discount_factors = asofdate.getDiscountFactors();
       
    float swap_rate_semi,swap_rate_annualized;
    
   std::cout << "-----------------Building SOFR curve--------------------------\n";

    cout << "Current SOFR rate: as of (10/28/2024) : 4.81% " << endl;
    float  sofr_today= 4.81;
 
  

    asof_rates_sofr = asofdate.build_sofr_curve(sofr_today);
    swapratecalculator swapdiscountfactors{asof_rates_sofr};
    RatePoint discount_factors_swap = swapdiscountfactors.getDiscountFactors();
                          
    std::cout << "\n-----------------Final Output--------------------------\n";
    double swap_denominator;
    swap_denominator = (discount_factors_swap.threemonth+discount_factors_swap.sixmonth+discount_factors_swap.ninemonth+discount_factors_swap.oneyear);

    double swap_rate_quarterly = (1-discount_factors_swap.oneyear)/(swap_denominator);
    swap_rate_annualized = swap_rate_quarterly*4;
    cout << "Swap rate_quarterly: " << swap_rate_quarterly*100 << "%" <<endl;
    cout << "Swap rate_annualized: " << swap_rate_annualized*100 << "%" << endl;
    
    
    std::cout << "\n-----------------------------------------------------------------------------------\n";

    std::string mydate_valuation = "11/01/2024";
    std::cout << "valuation date: " << mydate_valuation << std::endl;
    asof_rates_valuation = print(mydate_valuation,dates,threemonthrates,sixmonthrates,oneyearrates,twoyearrates);

  

    std::cout << "\n\n\n-----------------Building SOFR curve as of valuation date--------------------\n";

    cout << "Current SOFR rate: as of valuation date : 4.92% " << endl;
    //This is an input that can be entered 

    float  sofr_valuation_day= 4.92;
 
    swapratecalculator valuationdate{asof_rates_valuation};


    RatePoint asof_rates_sofr_valuation = valuationdate.build_sofr_curve(sofr_valuation_day);
    RatePoint discount_factors_swap_valuation = valuationdate.getDiscountFactors();

  

    double swap_denominator_val;
    swap_denominator_val = (discount_factors_swap_valuation.threemonth+discount_factors_swap_valuation.sixmonth+discount_factors_swap_valuation.ninemonth+discount_factors_swap_valuation.oneyear);

    double swap_rate_quarterly_valuation = (1-discount_factors_swap_valuation.oneyear)/(swap_denominator_val);
    double swap_rate_annualized_valuation = swap_rate_quarterly_valuation*4;
    cout << "Valuation date: Swap rate_quarterly: " << swap_rate_quarterly_valuation*100 << "%" <<endl;
    cout << "Valuation date: Swap rate_annualized: " << swap_rate_annualized_valuation*100 << "%" << endl;

   cout <<"----------------\n" ;
    cout << "Orginal swap price:" << swap_rate_annualized << "%" << endl;
    cout << "New swap price:" << swap_rate_annualized_valuation*100 <<"%"<< endl;
    
    float Notional = 1000000;
    float PV = swap_denominator_val;
    
    cout << "Net Value_pre pv adjustment: " << (swap_rate_annualized_valuation-swap_rate_annualized)*100 << "%" << endl;
    cout << "Notional $" << Notional << endl;
    cout << "Present value: " << PV << endl;
    
    
    std::cout << std::fixed << std::setprecision(4);
    cout << "Net Value_post pv adjustment: ($) " << (swap_rate_annualized_valuation-swap_rate_annualized)*(Notional)*PV <<"$" << endl;
    
    
    double volatility = 0.002;
    double delta_t = 0.25;
    int steps = 4;
    int num_of_simulations = 100;
    unsigned seed = 95;

    std::vector<std::vector<double>> sofr_simulations = generate_sofr_simulations(sofr_today, volatility, delta_t, steps, num_of_simulations, seed);
    std::string path = "./inputs/sofr_simulations.csv";
    write_sofr_to_csv(sofr_simulations, path);

 
    std::vector<std::vector<double>> discountfactor_sofr_simulations(sofr_simulations.size(), std::vector<double>(steps, 0.0));

    // Calculate discount factors
    for (size_t row = 0; row < sofr_simulations.size(); ++row) {
        for (size_t col = 0; col < sofr_simulations[row].size(); ++col) {
            discountfactor_sofr_simulations[row][col] = 1 / (1 + sofr_simulations[row][col] * (col + 1) * 3 / 1200.0);
        }
    }



        // Calculate swap rate for each simulation
    std::vector<double> swap_rates(sofr_simulations.size(), 0.0);
    double pv_simulation;
    //You should enter this value; 
    //double exercise_price = swap_rate_annualized;
    double exercise_price = 4.7/100;

    std::vector<double> swaption_payoffs;

    for (size_t row = 0; row < discountfactor_sofr_simulations.size(); ++row) {        
        std::vector<double> discount_factors = discountfactor_sofr_simulations[row];      
        double numerator = 1.0 - discount_factors.back();
        double denominator = std::accumulate(discount_factors.begin(), discount_factors.end(), 0.0) * delta_t;
        // Swap rate for this simulation
        swap_rates[row] = numerator / denominator;
        pv_simulation = denominator;
        double payoff = std::max(swap_rates[row] - exercise_price, 0.0) * pv_simulation * Notional;
        swaption_payoffs.push_back(payoff);
    }

    std::cout << "\nSwap Rates for each simulation:\n";
    for (size_t row = 0; row < swap_rates.size(); ++row) {
        std::cout << "Simulation " << row + 1 << ": " << std::fixed << std::setprecision(6) << swap_rates[row] * 100 << "%  " << swaption_payoffs[row] << std::endl;
    }


    double swaption_price = std::accumulate(swaption_payoffs.begin(), swaption_payoffs.end(), 0.0) / swaption_payoffs.size();
    cout << "-------------------\n";
    cout << "Notional ($) " << Notional << endl;
    cout << "Exercise Price: "   << exercise_price*100 << endl;
    cout << "Current underlying rate:" << swap_rate_annualized*100 << "%" << endl; 
    cout << "Option maturity: (T)" << "0" << endl;
    std::cout << "\nSwaption Price: $" << std::fixed << std::setprecision(2) << swaption_price << std::endl;


}
