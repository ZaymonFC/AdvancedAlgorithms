// CrossEntropy-Molecule.cpp : Defines the entry point for the console application.
//
#include "stdafx.h"
#include <vector>
#include <random>
#include <iostream>
#include <numeric>
#include <chrono>
#include <fstream>

#define PI 3.141592653589793238

std::random_device rd{};
//std::mt19937 gen{ rd() };
std::default_random_engine gen{ rd() };
class WeightedDistribution {
	// Mersenne Twister Random Engine
//	std::shared_ptr<std::mt19937> gen;

	// Normal Distribution
	std::normal_distribution<double> normal;
	double mean;
	double standardDeviation;

public:
	WeightedDistribution() = default;
	WeightedDistribution(const double mean, const double standardDeviation) : mean{ mean }, standardDeviation{ standardDeviation }
	{
		// Uniform Random
		std::random_device rd{};

		normal = std::normal_distribution<double>(mean, standardDeviation);
	}

	auto GenerateValue() -> double
	{
		return normal(gen);
	}

	auto UpdateParameters(const double mean, const double standardDeviation, const double parameterBlend = 1) -> void
	{
		const auto keepPercentage = 1 - parameterBlend;
		this->mean = keepPercentage * this->mean + parameterBlend * mean;
		this->standardDeviation = keepPercentage * this->standardDeviation + parameterBlend * standardDeviation;

		normal = std::normal_distribution<double>(this->mean, this->standardDeviation);
	}
};

struct Point
{
	double x;
	double y;
	Point() = default;
	Point(double x, double y) : x{x}, y{y}
	{
	}
};

auto Randf(double min, double max) -> double
{
	auto f = static_cast<double>(rand()) / RAND_MAX;
	return min + f * (max - min);
}

class Sequence
{
public:
	std::vector<double> angles;
	int length;
	
	Sequence() = default;

	Sequence(std::vector<double> angles) : angles(std::move(angles))
	{
		length = this->angles.size();
	}

	Sequence(std::vector<WeightedDistribution> &distributions)
	{
		length = distributions.size();
		angles = std::vector<double>();
		angles.reserve(length);

		for (auto i = 0; i < length; ++i)
		{
			angles.push_back(distributions.at(i).GenerateValue());
		}
	}

	auto CalculatePositions() -> std::vector<Point>
	{
		auto positions = std::vector<Point>();
		positions.reserve(length);

		auto psiK = 0.0;
		
		auto xComp = 0.0;
		auto yComp = 0.0;

		for (auto i = 0; i < length; i++)
		{
			psiK += std::fmod(angles.at(i), (2 * PI) - PI);
			xComp += cos(psiK);
			yComp += sin(psiK);

			positions.emplace_back(xComp, yComp);
		}

		return positions;
	}

	auto EvaluateCost() -> double
	{
		auto positions = CalculatePositions();
		auto cost = 0.0;

		for (auto i = 0; i < length; ++i)
		{
			for (auto j = i + 1; j < length; j++)
			{
				const auto p_i = positions.at(i);
				const auto p_j = positions.at(j);
				const auto r_ij_sqr = abs(pow(p_j.x - p_i.x, 2) + pow(p_j.y - p_i.y, 2));
				
				cost += 1 / pow(r_ij_sqr, 6) - 2 / pow(r_ij_sqr, 3);
			}
		}
		return cost;
	}

	auto PrintPositions() -> void
	{
		auto positions = CalculatePositions();
		for (const auto &position : positions)
		{
			std::cout << '(' <<  position.x << ',' << position.y << ")," << std::endl;
		}
	}

	auto WriteFile(std::string filename) -> void
	{
		std::ofstream fp(filename);
		auto positions = CalculatePositions();

		for (auto position : positions)
		{
			fp << position.x << ',' << position.y << std::endl;
		}
		fp.close();
	}
};

bool pairCompair(const std::pair<double, Sequence> & firstElement, const std::pair<double, Sequence> & secondElement)
{
	return firstElement.first < secondElement.first;
}

auto CrossEntropy(const int sequenceLength) -> void
{
	auto shuntAttempts = 15;
	// Parameters of cross entropy method
	const auto populationSize = 1000;
	const auto parameterBlend = 0.92;
	const auto eliteDistribution = 0.15;

	auto distributions = std::vector<WeightedDistribution>(sequenceLength, {3.14, 4});

	// Create initial vector of sequences
	auto sequences = std::vector<Sequence>();
	for (auto i = 0; i < populationSize; ++i)
	{
		// Passes vector of distributions to create new sequence
		sequences.emplace_back(distributions);
	}

//	for(auto sequence : sequences)
//	{
//		printf("{");
//		for (auto angle : sequence.angles)
//		{
//			printf("%lf,", angle);
//		}
//		printf("}\n");
//	}

	auto lastBest = std::numeric_limits<double>::max();
	auto bestScore = std::numeric_limits<double>::max();
	auto bestSequence = Sequence();

	for (auto iteration = 0; iteration < 3000; ++iteration)
	{
		// Sort population by cost ------------------------------------------------------
		auto scoredPopulation = std::vector<std::pair<double, Sequence>>();
		scoredPopulation.reserve(populationSize);
		for (auto element : sequences)
		{
			scoredPopulation.emplace_back(element.EvaluateCost(), element);
		}
		std::sort(scoredPopulation.begin(), scoredPopulation.end(), pairCompair);

		
		const auto diff = abs(lastBest - scoredPopulation.at(0).first);
		printf("Score: %lf | Diff: %lf\n", scoredPopulation.at(0).first, diff);

		lastBest = scoredPopulation.at(0).first;

		if (lastBest < bestScore)
		{
			bestScore = lastBest;
			bestSequence = Sequence(scoredPopulation.at(0).second.angles);
		}

		if (diff < 0.00001)
		{
			if (shuntAttempts > 0)
			{
				for (auto & distribution : distributions)
				{
					if (rand() % 100 > 25)
					{
						const auto randMean = Randf(-1.57, 1.57);
						const auto randSDev = Randf(1, 6);
						distribution.UpdateParameters(randMean, randSDev);
					}
				}
				shuntAttempts--;
			} 
			else
			{
				printf("\nReached Sigma\n");
				printf("N: %d Best Score Found: %lf\n", sequenceLength, bestScore);
				bestSequence.WriteFile("Molecule.txt");
//				system("python ./DrawMolecule");
				

				break;
			}
		} 
		
		// Update distribution parameters (mean, std deviation) for each angle based ----
		// off the values of the elite sample
		
		// Loop through each angle
		for (auto angle = 0; angle < sequenceLength; ++angle)
		{
			auto values = std::vector<double>();
			values.reserve(populationSize * eliteDistribution);

			// Loop through the elite 10%
			for (auto element = 0; element < populationSize * eliteDistribution; ++element)
			{
				values.emplace_back(scoredPopulation.at(element).second.angles.at(angle));
			}

			const auto sum = std::accumulate(values.begin(), values.end(), 0.0);
			const auto mean = sum / values.size();

			auto diff = std::vector<double>(values.size());
			std::transform(values.begin(), values.end(), diff.begin(), [mean](double x) {return x - mean; });
			
			const auto sq_sum = std::inner_product(diff.begin(), diff.end(), diff.begin(), 0.0);
			const auto standardDeviation = std::sqrt(sq_sum / values.size());

			distributions.at(angle).UpdateParameters(mean, standardDeviation, parameterBlend);
		}
		
		// Every few iterations cleanup the distributions
		// TODO:

		// Generate new population ------------------------------------------------------
		sequences = std::vector<Sequence>();
		sequences.reserve(populationSize);
		for (auto i = 0; i < populationSize; ++i)
		{
			// Passes vector of distributions to create new sequence
			sequences.emplace_back(distributions);
		}
	}
}


int main()
{
	CrossEntropy(20);
    return 0;
}

