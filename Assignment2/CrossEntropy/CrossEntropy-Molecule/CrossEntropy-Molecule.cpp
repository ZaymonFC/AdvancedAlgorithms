// CrossEntropy-Molecule.cpp : Defines the entry point for the console application.
//
#include "stdafx.h"

#include <vector>
#include <random>

struct WeightedDistribution {
	// Uniform Random
	std::random_device rd{};
	// Mersenne Twister Random Engine
	std::mt19937 gen{ rd() };

	// Normal Distribution
	std::normal_distribution<double> normal;

	WeightedDistribution() = default;
	WeightedDistribution(const double mean, const double standardDeviation)
	{
		normal = std::normal_distribution<double>(mean, standardDeviation);
	}

	auto GenerateValue() -> double
	{
		return normal(gen);
	}

	auto UpdateParameters(const double mean, const double standardDeviation) -> void
	{
		normal = std::normal_distribution<double>(mean, standardDeviation);
	}
};

class RandomSequencer {
	
};


int main()
{
    return 0;
}

