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

int main()
{



    std::ifstream swap_df("C:/Users/manoj/OneDrive/scripts/inputs/swapvalues.txt");

    //Checking if the swap valuation is opening or not 
    if (!swap_df){
        std::cerr << "Error opening the file" << std::endl;
        return 1;
    }
    std::istream_iterator<double> start(swap_df),end;
    std::vector<double> swap_valuation_df(start,end);

    //reading contents of the file and printing 
    for(auto &myiterator: swap_valuation_df){
        std::cout << myiterator << "\t";
    }
    //calculating the swap valuation over different Montecarlo scenarios
    double swap_summation = std::accumulate(swap_valuation_df.begin(),swap_valuation_df.end(),0.0);
    std::cout << swap_summation << "\n";
    int num_of_montecarlo_scenarios = swap_valuation_df.size();

        if(1)
    {
    //To be done this week 
    //write the logic to value swap over 10,000 scenarios using monetcarlo simulations;  
    }
    double average_swap_price = (static_cast<double> (swap_summation)/num_of_montecarlo_scenarios);
    std:: cout << "========================\n";
    std:: cout << "# of Montecarlo scenarios: " << num_of_montecarlo_scenarios << std::endl;
    std:: cout << "Average swap valuation:$" <<average_swap_price ;
    std:: cout << "\n =========================\n";

}

