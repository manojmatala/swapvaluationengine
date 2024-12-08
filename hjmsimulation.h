#ifndef HJMSIMULATION_H
#define HJMSIMULATION_H

#include <vector>
#include <string>


std::vector<std::vector<double>> generate_sofr_simulations(
    double sofr_today, double volatility, double delta_t, int steps, int num_of_simulations, unsigned seed = 95);


void write_sofr_to_csv(const std::vector<std::vector<double>>& sofr_simulations, const std::string& filename);

#endif
