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

auto MVC_Search(Graph graph, int maximumSelections) -> void
{
	std::default_random_engine generator(std::chrono::system_clock::now().time_since_epoch().count());
//	std::default_random_engine generator(1);
	const std::uniform_int_distribution<int> distribution(0, graph.vertices - 1);

	QuickSet vSet = QuickSet(graph.vertices);
	std::vector<int> penalty = std::vector<int>(graph.vertices, -1);

	// Pick a random starting index
	int index = distribution(generator);
	vSet.Insert(index);

	// Greedily develop an initial cover
	while(!graph.IsVertexCover(vSet))
	{
		auto degrees = graph.GetDegreeList(vSet);
		sort(degrees.begin(), degrees.end());
		vSet.Insert(degrees.back().second);
	}

	std::cout << "Initial Greedy Vertex Cover Size: " << vSet.Size() << std::endl;

	// Plateu Search for better solutions with penalty


	// Reduce cover size

	// Rebuild

}

int main()
{

	const auto filename = "brock800_1.clq";
	const auto graph = GraphUtilities::LoadGraph(filename);

	std::cout << "Read in graph with filename: " << filename << std::endl;
	std::cout << graph.vertices - 1 << " vertices. Compliment has " << graph.edges << " edges." << std::endl;

//	for (auto i = 0; i < 1000; i++)
//	{
//		MVC_Search(graph, 1000000);
//	}

	MVC_Search(graph, 100000);

    return 0;
}
