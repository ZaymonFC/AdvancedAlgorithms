#include "stdafx.h"
#include "Graph.h"


Graph::Graph(int vertexCount)
{
	edges = 0;
	vertices = vertexCount;
	// Initilialise Adjacency Matrix
	matrix = std::vector<std::vector<bool>>(vertices, std::vector<bool>(vertices, false));

	// Inititialise Adjacency Lists
	nodes = std::vector<std::vector<int>>();
	nodes.reserve(vertices);

	for (auto i = 0; i < vertices; i++)
	{
		nodes.emplace_back(std::vector<int>());
	}
}

auto Graph::AddEdgeMatrix(int node1, int node2) -> void
{
	matrix.at(node1).at(node2) = true;
	matrix.at(node2).at(node1) = true;
	edges++;
}

auto Graph::AddEdgeLists(int node1, int node2) -> void
{
	nodes.at(node1).emplace_back(node2);
	nodes.at(node2).emplace_back(node1);
	edges++;
}

auto Graph::ConvertToCompliment() -> void
{
	edges = 0;
	for (auto i = 0; i < vertices; i++)
	{
		for (auto j = i + 1; j < vertices; j++)
		{
			if (!matrix.at(i).at(j))
			{
				AddEdgeLists(i, j);
			}
			matrix.at(i).at(j) = !matrix.at(i).at(j);
		}
	}
}

bool Graph::IsVertexCover(const QuickSet& vSet)
{
	for (auto i = 0; i < vertices; i++)
	{
		for (auto j = i + 1; j < vertices; j++)
		{
			if (matrix.at(i).at(j))
			{
				if (vSet.Contains(i) || vSet.Contains(j)) continue;
				return false;
			}
		}
	}
	return true;
}

int Graph::CalculateDegree(int nodeId, const QuickSet& vSet)
{
	auto degree = nodes.at(nodeId).size();
	for (auto target : nodes.at(nodeId))
	{
		if (vSet.Contains(target)) degree--;
	}

	return degree;
}

std::vector<std::pair<int, int>> Graph::GetDegreeList(const QuickSet& vSet)
{
	std::vector<std::pair<int, int>> degrees;
	for (int i = 0; i < vertices; i++) 
	{
		if (vSet.Contains(i)) continue;
		degrees.emplace_back(CalculateDegree(i, vSet), i);
	}

	return degrees;
}
