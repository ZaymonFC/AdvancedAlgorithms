// BFGS-Solver.cpp : Defines the entry point for the console application.
//

#include <random>
#include <vector>
#include <chrono>
#include <fstream>
#include <iostream>
#include <numeric>

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

	static auto StdVToE(std::vector<double> x) -> Eigen::VectorXd
	{
		Eigen::Map<Eigen::VectorXd> ret(&x[0], x.size());
		return ret;
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
	std::vector<std::vector<double>> values;
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

	auto CalculatePositions(Eigen::VectorXd x) -> std::vector<Point>
	{
		auto positions = std::vector<Point>();
		positions.reserve(length);

		auto psiK = 0.0;

		auto xComp = 0.0;
		auto yComp = 0.0;

		for (auto i = 0; i < length; i++)
		{
			psiK += std::fmod(x[i], (2 * PI) - PI);
			xComp += cos(psiK);
			yComp += sin(psiK);

			positions.emplace_back(xComp, yComp);
		}

		return positions;
	}

	auto EvaluateCost(const std::vector<Point> positions) -> double
	{
		auto cost = 0.0;

		for (auto i = 0; i < length; ++i)
		{
			const auto p_i = positions.at(i);
			const auto x = p_i.x;
			const auto y = p_i.y;

			for (auto j = i + 1; j < length; j++)
			{
				const auto p_j = positions.at(j);
				const auto dx = p_j.x - x;
				const auto dy = p_j.y - y;

				const auto r_ij_2 = 1.0 / (dx * dx + dy * dy);
				const auto r_ij_6 = r_ij_2 * r_ij_2 * r_ij_2;
				const auto r_ij_12 = r_ij_6 * r_ij_6;

				cost += r_ij_12 - 2 * r_ij_6;
				values[i][j] = r_ij_12 * r_ij_2 - r_ij_6 * r_ij_2;
			}
		}
		return cost;
	}

	auto CalculateGradients(std::vector<Point> positions, Eigen::VectorXd &grad)
	{
		auto gradients = std::vector<double>();
		gradients.reserve(length);

		for (auto m = 0; m < length; m++)
		{
			auto total = 0.0;
			const auto p_m = positions.at(m);
			const auto x = p_m.x;
			const auto y = p_m.y;

			for (auto i = 0; i < m; i++)
			{
				const auto p_i = positions.at(i);
				for (auto j = m + 1; j < length; j++)
				{
					const auto p_j = positions.at(j);
					total += values[i][j] * ((p_i.x - p_j.x) * (y - p_j.y) + (p_i.y - p_j.y) * (p_j.x - x));
				}
			}
			// Emplace the value of dm/dV for m
			gradients.emplace_back(-12.0 * total);
		}

		return gradients;
	}

	auto WriteFile(std::string filename) -> void
	{
		std::ofstream fp(filename);
		auto positions = CalculatePositions(Conversion::StdVToE(angles));

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
		values = std::vector<std::vector<double>>(length, std::vector<double>(length));

		const auto positions = CalculatePositions(x);
		const auto cost = EvaluateCost(positions);
		CalculateGradients(positions, grad);

		return cost;
	}
};

bool pairCompair(const std::pair<double, Sequence> & firstElement, const std::pair<double, Sequence> & secondElement)
{
	return firstElement.first < secondElement.first;
}

auto BFGS_Score(Sequence sequence, LBFGSpp::LBFGSSolver<double> &solver) -> double
{
	// Construct Eigen Xd
	auto s = Sequence(sequence.length);
	auto x = Conversion::StdVToE(sequence.angles);
	// Variable for the resulting score
	double fx;
	solver.minimize(s, x, fx);
	return fx;
}

auto BFGS_Sequence(Sequence sequence, LBFGSpp::LBFGSSolver<double> &solver) -> Sequence
{
	// Construct Eigen Xd
	auto s = Sequence(sequence.length);
	auto x = Conversion::StdVToE(sequence.angles);
	// Variable for the resulting score
	double fx;
	solver.minimize(s, x, fx);
	const auto finalSequence = Conversion::EToStdV(x);
	return Sequence(finalSequence);
}

auto Optimizer(int sequenceLength) -> void 
{
	auto shuntAttempts = 25;

	// Setup BFGS Params
	LBFGSpp::LBFGSParam<double> parameters;
	parameters.epsilon = 1e-1;
	parameters.max_iterations = 1;
	parameters.max_step = std::numeric_limits<double>::max();

	// Create the solver
	LBFGSpp::LBFGSSolver<double> solver(parameters);

	// -- Cross Entropy Parameters
	const auto populationSize = 250;
	const auto parameterBlend = 0.95;
	const auto eliteDistribution = 0.12;

	auto distributions = std::vector<WeightedDistribution>(sequenceLength, { 0, 3 });

	// Create initial vector of sequences
	auto sequences = std::vector<Sequence>();
	for (auto i = 0; i < populationSize; ++i)
	{
		sequences.emplace_back(distributions);
	}

	auto lastBest = std::numeric_limits<double>::max();
	auto bestScore = std::numeric_limits<double>::max();
	auto bestSequence = Sequence();

	for (;;)
	{
		// Sort population by cost ------------------------------------------------------
		auto scoredPopulation = std::vector<std::pair<double, Sequence>>();
		scoredPopulation.reserve(populationSize);

		for (auto element : sequences)
		{
			scoredPopulation.emplace_back(BFGS_Score(element, solver), element);
		}

		std::sort(scoredPopulation.begin(), scoredPopulation.end(), pairCompair);



		// Calculate Progression --------------------------------------------------------
		const auto diff = std::abs(lastBest - scoredPopulation.at(0).first);
		printf("Score: %lf | Diff: %lf\n", scoredPopulation.at(0).first, diff);

//		if (std::isnan(scoredPopulation.at(0).first)) return -1337;

		lastBest = scoredPopulation.at(0).first;

		if (lastBest < bestScore)
		{
			bestScore = lastBest;
			bestSequence = Sequence(scoredPopulation.at(0).second.angles);
		}


		// Update distribution parameters (mean, std deviation) for each angle based ----
		// off the values of the elite sample
		for (auto angle = 0; angle < sequenceLength; ++angle)
		{
			auto values = std::vector<double>();
			values.reserve(populationSize * eliteDistribution);

			// Loop through the elite sample
			for (auto element = 0; element < populationSize * eliteDistribution; ++element)
			{
				values.emplace_back(scoredPopulation.at(element).second.angles.at(angle));
			}

			const auto sum = std::accumulate(values.begin(), values.end(), 0.0);
			const auto mean = sum / values.size();

			auto diffFromMean = std::vector<double>(values.size());
			std::transform(values.begin(), values.end(), diffFromMean.begin(), [mean](double x) {return x - mean; });

			const auto sq_sum = std::inner_product(diffFromMean.begin(), diffFromMean.end(), diffFromMean.begin(), 0.0);
			const auto standardDeviation = std::sqrt(sq_sum / values.size());

			distributions.at(angle).UpdateParameters(mean, standardDeviation, parameterBlend);
		}
		// Exit conditions
		if (diff < 0.05)
		{
			if (shuntAttempts > 0)
			{
		
				for (auto & distribution : distributions)
				{
					if (rand() % 100 > 40)
					{
						const auto randMean = 0;
						const auto randSDev = Randf(1, 3);
						distribution.UpdateParameters(randMean, randSDev);
					}
				}

				shuntAttempts--;
			} else {
				// Update BFGS Params
				parameters.epsilon = 1e-7;
				parameters.max_iterations = 100;

				printf("\nSurpased Epsilon\n");
				const auto finalSequence = BFGS_Sequence(bestSequence, solver);
				printf("N: %d Best Score Found: %lf\n", sequenceLength, BFGS_Score(finalSequence, solver));
				return;
//				finalSequence.WriteFile("Molecule.txt");
			}
		}

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


int main(int argc, char ** argv)
{
	if (argc != 2) {
		std::cout << "Please enter the correct commandline arguments" << std::endl;
		return -1;
	}
	
	Optimizer(atoi(argv[1]));
    return 0;
}

