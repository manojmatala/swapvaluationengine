#include <vector>
#include <random>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <cmath>
using namespace std;

std::vector<std::vector<double>> generate_sofr_simulations(
    double sofr_today, double volatility, double delta_t, int steps, int num_of_simulations, unsigned seed) {
    std::default_random_engine generator(seed);
    std::normal_distribution<double> distribution(0.0, 1.0);
    std::vector<std::vector<double>> sofr_simulations;

    for (int sim = 1; sim <= num_of_simulations; ++sim) {
        std::vector<double> sofr_curve_this_simulation{sofr_today};
        double forward_rate = sofr_today;

        for (int i = 1; i <= steps; ++i) {
            double Z = distribution(generator);
            double drift = 0.5 * volatility * volatility * (steps - i + 1) * delta_t;
            forward_rate += drift * delta_t + volatility * sqrt(delta_t) * Z;
            sofr_curve_this_simulation.push_back(forward_rate);
        }
        sofr_simulations.push_back(sofr_curve_this_simulation);
    }

    return sofr_simulations;
}

void write_sofr_to_csv(const std::vector<std::vector<double>>& sofr_simulations, const std::string& filename) {
    std::ofstream writefile(filename);
    if (!writefile.is_open()) {
        throw std::runtime_error(" Could not open the file for writing!");
    }

    writefile << std::fixed << std::setprecision(4);

    for (int i = 0; i < sofr_simulations.size(); ++i) {
        writefile << "Simulation " << i + 1 << ":,";
        std::cout << "Simulation " << i + 1 << ":,";
        for (const auto& rate : sofr_simulations[i]) {
            writefile << rate << ",";
            std::cout << rate << ",";
            
        }
        writefile << "\n";
        std::cout << "\n";
    }

    writefile.close();
}


