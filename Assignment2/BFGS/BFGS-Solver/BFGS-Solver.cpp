// BFGS-Solver.cpp : Defines the entry point for the console application.
//

#include <random>
#include <vector>
#include <chrono>
#include <fstream>
#include <iostream>

#include <Eigen/Core>
#include <LBFGS.h>

#define PI 3.141592653589793238

// Global Random Device
std::random_device rd{};
std::mt19937 gen{ rd() };

class WeightedDistribution {

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
	Point(double x, double y) : x{ x }, y{ y }
	{
	}
};

auto Randf(const double min, const double max) -> double
{
	const auto f = static_cast<double>(rand()) / RAND_MAX;
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
			std::cout << '(' << position.x << ',' << position.y << ")," << std::endl;
		}
	}

	auto WriteFile(std::string filename) -> void
	{
		std::ofstream fp(filename);
		auto positions = CalculatePositions();

		// Align to 0, 0
		const auto xOffset = positions.at(0).x;
		const auto yOffset = positions.at(0).y;

		for (const auto position : positions)
		{
			fp << position.x - xOffset << ',' << position.y - yOffset << std::endl;
		}
		fp.close();
	}
};

bool pairCompair(const std::pair<double, Sequence> & firstElement, const std::pair<double, Sequence> & secondElement)
{
	return firstElement.first < secondElement.first;
}

struct GeneticUtilities
{
	
};

auto Optimizer(int sequenceLength) -> void
{
	// Parameters
	const auto populationSize = 8;
	
	auto random = WeightedDistribution(0, 3);
	// Create initial population
	
}



int main()
{
    return 0;
}

