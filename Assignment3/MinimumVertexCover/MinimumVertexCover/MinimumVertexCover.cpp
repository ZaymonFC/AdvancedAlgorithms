// MinimumVertexCover.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "GraphUtilities.h"
#include <iostream>
#include <vector>
#include <random>
#include <chrono>

int best = 999;

auto UpdatePenalty(std::vector<int>& penaltyList) -> void 
{
	for (auto& element : penaltyList) element--;
}

auto AddToPenalty(std::vector<int> & penaltyList, int nodeId) -> void
{
	penaltyList.at(nodeId) = 10;
}

void ConstructVCGreedy(Graph& graph, QuickSet& vSet, std::default_random_engine generator, const std::uniform_int_distribution<int> distribution)
{
	// Pick a random starting index
	const int index = distribution(generator);
	vSet.Insert(index);

	// Greedily develop an initial cover
	while(!graph.IsVertexCover(vSet))
	{
		auto degrees = graph.GetDegreeList(vSet);
		sort(degrees.begin(), degrees.end());
		vSet.Insert(degrees.back().second);
	}
}


auto MVC_Search(Graph &graph, int maximumSelections) -> void
{
	std::default_random_engine generator(std::chrono::system_clock::now().time_since_epoch().count());
//	std::default_random_engine generator(1);
	const std::uniform_int_distribution<int> distribution(0, graph.vertices - 1);

	QuickSet vSet = QuickSet(graph.vertices);
	ConstructVCGreedy(graph, vSet, generator, distribution);

	std::cout << "Initial Greedy Vertex Cover Size: " << vSet.Size() << std::endl;

	std::vector<int> penalty = std::vector<int>(graph.vertices, -1);
	
	auto iterations = 0;
	auto bestCover = vSet.Size();
	const int operationsPerPhase = 200;

	// Random Distributions
	const std::uniform_int_distribution<int> distribution2(2, 5);

	while (iterations < 100)
	{
		// ----------------------------------------------------------------------------------------
		// --- Reduce Rebuild Phase
		for (auto operation = 0; operation < operationsPerPhase; operation++)
		{
			// Roll for a number of nodes to remove
			const int depth = distribution2(generator);

			// Reduce the number of nodes in the list
			for (auto i = 0; i < depth; i++)
			{
				auto u = rand() % (vSet.Size() - 1);
				while (!vSet.Contains(u)) {
					u = rand() % (vSet.Size() - 1);
				}

				vSet.Remove(u);

				AddToPenalty(penalty, u);
				UpdatePenalty(penalty);

				if (graph.IsVertexCover(vSet)) break;
			}

			if (graph.IsVertexCover(vSet)) break;

			// Rebuild - Stopping early if we find a better cover
			auto surface = 0;
			for (auto i = 0; i < depth; i++) {
				auto degree = graph.GetDegreeList(vSet);
				std::sort(degree.begin(), degree.end());

				auto index = 0;
				while (penalty.at(degree.at(index).second) > 0) {
					index++;
					if (index == degree.size())
					{
						index = rand() % (degree.size() - 1);
						break;
					}
				}

				vSet.Insert(degree.at(index).second);

				AddToPenalty(penalty, degree.at(index).second);
				UpdatePenalty(penalty);

				if (graph.IsVertexCover(vSet)) break;
			}
		}

		if (graph.IsVertexCover(vSet))
		{
			if (vSet.Size() < bestCover) bestCover = vSet.Size();
		}

		// ----------------------------------------------------------------------------------------
		// --- Stabalise Phase
		if (iterations % 2 == 1)
		{
			const auto v = rand() % (vSet.Size() - 1);
			vSet.Remove(v);
		}
		auto swaps = 0;

		// Get Back to a vertex Cover with swaps
		while(!graph.IsVertexCover(vSet) && swaps < 10000)
		{
			auto degree = graph.GetDegreeList(vSet);
			std::sort(degree.begin(), degree.end());

			auto index = 0;
			while (penalty.at(degree.at(index).second) > 0)
			{
				index++;
				if (index == degree.size())
				{
					index = rand() % (degree.size() - 1);
					break;
				}
			}

			vSet.Insert(degree.at(index).second);

			while (true)
			{
				const auto u = rand() % (vSet.Size() - 1);
				if (penalty.at(u) > 0) continue;
				break;
			}

			AddToPenalty(penalty, degree.at(index).second);
			UpdatePenalty(penalty);
			swaps--;
		}

		if (graph.IsVertexCover(vSet))
		{
			if (vSet.Size() < bestCover) bestCover = vSet.Size();
		}

		if (iterations % 2 == 0 && !graph.IsVertexCover(vSet))
		{
			auto degree = graph.GetDegreeList(vSet);
			std::sort(degree.begin(), degree.end());
			vSet.Insert(degree.at(rand() % std::max(static_cast<int>(degree.size()) - 1, 5)).second);
		}

		std::cout << iterations << " | " << bestCover << std::endl;
		iterations++;
	}


	std::cout << bestCover;

}

int main()
{

//	const auto filename = "brock800_1.clq";
//	const auto filename = "p_hat1500-1.clq";
	const auto filename = "c4000.5.clq";
//	const auto filename = "c2000.9.clq";
	auto graph = GraphUtilities::LoadGraph(filename);

	std::cout << "Read in graph with filename: " << filename << std::endl;
	std::cout << graph.vertices - 1 << " vertices. Compliment has " << graph.edges << " edges." << std::endl;

//	for (auto i = 0; i < 1000; i++)
//	{
//		MVC_Search(graph, 1000000);
//	}

	MVC_Search(graph, 100000);

    return 0;
}
