#pragma once
#include <string>
#include "Graph.h"

class GraphUtilities
{
public:
	static auto LoadGraph(std::string fileName)->Graph;
	static std::vector<std::string> Split(const std::string& s, char delimiter);
};

