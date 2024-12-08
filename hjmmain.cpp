//execute this code only for demonostrating the montecarlo generate_sofr_simulations

#include "HJMmodelimpl.cpp"
#include "iostream"
int main() {
    // Model parameters
    double sofr_today = 4.81;
    double volatility = 0.002;
    double delta_t = 0.25;
    int steps = 4;
    int num_of_simulations = 100;
    unsigned seed = 95;

    // Generate SOFR simulations
    auto sofr_simulations = generate_sofr_simulations(sofr_today, volatility, delta_t, steps, num_of_simulations, seed);

    // Write simulations to CSV
    std::string path = "./inputs/sofr_simulations.csv";
    write_sofr_to_csv(sofr_simulations, path);

    std::cout << "Data written to " << path << std::endl;

    return 0;
}
