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

	Sequence(int n)
	{
		length = n;
		angles = std::vector<double>();
	}

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

	auto EvaluateCost(const std::vector<Point> positions) const -> double
	{
		auto cost = 0.0;

		for (auto i = 0; i < length; ++i)
		{
			for (auto j = i + 1; j < length; j++)
			{
				const auto p_i = positions.at(i);
				const auto p_j = positions.at(j);
				const auto r_ij_squared = abs(pow(p_j.x - p_i.x, 2) + pow(p_j.y - p_i.y, 2));

				cost += 1 / pow(r_ij_squared, 6) - 2 / pow(r_ij_squared, 3);
			}
		}
		return cost;
	}

	auto EvaluateCost() -> double
	{
		return EvaluateCost(CalculatePositions());
	}

	auto CalculateGradients(std::vector<Point> positions) const -> std::vector<double>
	{
		auto gradients = std::vector<double>();
		gradients.reserve(length);

		for (auto m = 0; m < length; m++)
		{
			auto total = 0.0;
			for (auto i = 0; i < m; i++)
			{
				for (auto j = m + 1; j < length; j++)
				{
					const auto p_i = positions.at(i);
					const auto p_j = positions.at(j);
					const auto p_m = positions.at(m);

					const auto r_ij_squared = abs(pow(p_j.x - p_i.x, 2) + pow(p_j.y - p_i.y, 2));

					total += (1 / pow(r_ij_squared, 7) - 1 / pow(r_ij_squared, 4)) * ((p_i.x - p_j.x) * (p_m.y - p_j.y) + (p_i.y - p_j.y) * (p_j.x - p_m.x));
				}
			}
			// Emplace the value of dm/dV for m
			gradients.emplace_back(-12.0 * total);
		}

		return gradients;
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

	double operator()(const Eigen::VectorXd& x, Eigen::VectorXd& grad)
	{
		// Replace the angles in the class
		angles = std::vector<double>();
		for (auto i = 0; i < x.size(); i++)
		{
			angles.emplace_back(x[i]);
		}

		const auto positions = CalculatePositions();
		const auto gradients = CalculateGradients(positions);

		for (auto i = 0; i < grad.size(); i++)
		{
			grad[i] = gradients.at(i);
		}

		return EvaluateCost(positions);
	}
};

bool pairCompair(const std::pair<double, Sequence> & firstElement, const std::pair<double, Sequence> & secondElement)
{
	return firstElement.first < secondElement.first;
}

struct GeneticUtilities
{
	
};

struct Conversion
{
	static auto EToStdV(Eigen::VectorXd x) -> std::vector<double>
	{
		auto ret = std::vector<double>();
		ret.reserve(x.size());
		for (auto i = 0; i < x.size(); ++i)
		{
			ret.emplace_back(x[i]);
		}
		return ret;
	}
};

auto Optimizer(int sequenceLength) -> void
{
	// Setup BFGS Params
	LBFGSpp::LBFGSParam<double> parameters;
	parameters.epsilon = 1e-6;
	parameters.max_iterations = 100;

	// Create the solver
	LBFGSpp::LBFGSSolver<double> solver(parameters);
	
	Sequence s(sequenceLength);

	// Construct Initial guess
	Eigen::VectorXd x = Eigen::VectorXd::Ones(sequenceLength);
	double fx;
	auto number_iterations = solver.minimize(s, x, fx);

	const std::vector<double> angles = Conversion::EToStdV(x);

	std::cout << fx << std::endl;

	Sequence(angles).WriteFile("bfgs_oneiter.txt");
		
}



int main()
{
	Optimizer(55);
    return 0;
}

