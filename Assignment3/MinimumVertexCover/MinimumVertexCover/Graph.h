#pragma once
#include <vector>

class Graph
{
public:
	int vertices;
	int edges;
	std::vector<std::vector<int>> nodes;
	std::vector<std::vector<bool>> matrix;

	Graph(int vertexCount);
	
	// Creational Functions
	auto AddEdgeMatrix(int node1, int node2) -> void;
	auto AddEdgeLists(int node1, int node2) -> void;
	auto ConvertToCompliment() -> void;

	~Graph() = default;
};

