// CrossEntropy-Molecule.cpp : Defines the entry point for the console application.
//
#include "stdafx.h"
#include <vector>
#include <random>

class WeightedDistribution {
public:
	// Uniform Random
	std::random_device rd;
	// Mersenne Twister Random Engine
	std::mt19937 gen;

	// Normal Distribution
	std::normal_distribution<double> normal;

	WeightedDistribution() = default;
	WeightedDistribution(double mean, double standardDeviation)
	{
		gen = std::mt19937 { rd };
		normal = std::normal_distribution<double>(mean, standardDeviation);
	}

	auto GenerateValue() -> double
	{
		return normal(gen);
	}

	auto UpdateParameters(double mean, double standardDeviation) -> void
	{
		normal = std::normal_distribution<double>(mean, standardDeviation);
	}
};

//struct Sequence {
//	int length;
//	std::vector<double> angles;
//	
//	Sequence() = default;
//	explicit Sequence(std::vector<double> angles) : angles(std::move(angles))
//	{
//		length = angles.size();
//	}
//};

//class SequenceGenerator
//{
//	int sequenceLength;
//	int 
//
//	SequenceGenerator() = default;
//};

int main()
{
	// new up a generator
	auto generator = WeightedDistribution(10.5, 5.0);

    return 0;
}

