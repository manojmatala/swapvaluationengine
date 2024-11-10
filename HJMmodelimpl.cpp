//This a test program to perform montecarlo simulations using the HJM model


#include <iostream>
#include <vector>
#include <cmath>
#include <random>
#include <chrono> // For seeding with current time

int main() {
    // Model parameters
    double sofr_today = 4.81;      // Initial SOFR today
    double volatility = 0.002;     // Constant volatility, 0.2% annualized
    double delta_t = 0.25;         // Quarterly interval (in years)
    int steps = 4;                 // Number of quarterly steps (1 year)

    // Seed random number generator with current time
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::default_random_engine generator(seed);
    std::normal_distribution<double> distribution(0.0, 1.0);

    // Store simulated forward rates
    std::vector<double> sofr_curve = {sofr_today};

    // HJM simulation over each quarter
    double forward_rate = sofr_today;
    for (int i = 1; i <= steps; ++i) {
        double Z = distribution(generator); // Random normal variable
        double drift = 0.5 * volatility * volatility * (steps - i + 1) * delta_t; // Simplified drift term
        forward_rate += drift * delta_t + volatility * sqrt(delta_t) * Z;
        sofr_curve.push_back(forward_rate);
    }

    // Print the simulated forward SOFR curve
    for (int i = 0; i < sofr_curve.size(); ++i) {
        std::cout << "SOFR at " << i * 3 << " months: " << sofr_curve[i] << "%" << std::endl;
    }

    return 0;
}
