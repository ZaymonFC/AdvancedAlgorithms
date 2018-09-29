#include "stdafx.h"
#include "Graph.h"


Graph::Graph(int vertexCount)
{
	edges = 0;
	vertices = vertexCount + 1;
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
}

auto Graph::AddEdgeLists(int node1, int node2) -> void
{
	nodes.at(node1).emplace_back(node2);
	nodes.at(node2).emplace_back(node1);
	edges++;
}

auto Graph::ConvertToCompliment() -> void
{
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
