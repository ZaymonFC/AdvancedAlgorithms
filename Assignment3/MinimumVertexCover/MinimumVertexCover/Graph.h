#pragma once
#include <vector>
#include "QuickSet.h"

class Graph
{
public:
	int vertices;
	int edges;
	std::vector<std::vector<int>> nodes{};
	std::vector<std::vector<bool>> matrix{};

	Graph(int vertexCount);
	
	// Creational Functions
	auto AddEdgeMatrix(int node1, int node2) -> void;
	auto AddEdgeLists(int node1, int node2) -> void;
	auto ConvertToCompliment() -> void;

	bool IsVertexCover(const QuickSet& vSet);
	int CalculateDegree(int nodeId, const QuickSet& vSet);
	std::vector<std::pair<int, int>> GetDegreeList(const QuickSet& vSet);

	~Graph() = default;
};

