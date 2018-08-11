// CrossEntropy-Molecule.cpp : Defines the entry point for the console application.
//
#include "stdafx.h"
#include <vector>
#include <random>
#include <iostream>
#include <valarray>

class WeightedDistribution {
	// Mersenne Twister Random Engine
	std::mt19937 gen;

	// Normal Distribution
	std::normal_distribution<double> normal;

public:
	WeightedDistribution() = default;
	WeightedDistribution(const double mean, const double standardDeviation)
	{
		// Uniform Random
		std::random_device rd{};
		gen = std::mt19937 { rd() };
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
	auto generator = WeightedDistribution(3.14159, 1.2);

	for (int i = 0; i < 10; ++i)
	{
		std::cout << generator.GenerateValue() << std::endl;
	}
	std::cout << "New Params" << std::endl;
	generator.UpdateParameters(100, 0.5);
	for (int i = 0; i < 10; ++i)
	{
		std::cout << generator.GenerateValue() << std::endl;
	}

    return 0;
}

