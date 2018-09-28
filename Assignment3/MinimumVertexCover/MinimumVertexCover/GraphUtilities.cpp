#include "stdafx.h"
#include "GraphUtilities.h"
#include <fstream>
#include <sstream>

auto GraphUtilities::LoadGraph(std::string fileName) -> Graph
{
	std::ifstream fs(fileName);
	std::string str;

	auto vertices = 0;

	while (std::getline(fs, str))
	{
		if (str[0] == 'p')
		{
			auto tokens = Split(str, ' ');
			vertices = std::stoi(tokens[2]);
			break;
		}
	}
	
	auto graph = Graph(vertices);
	while (std::getline(fs, str))
	{
		if (str[0] == 'e')
		{
			const auto tokens = Split(str, ' ');
			const auto node1 = std::stoi(tokens.at(1));
			const auto node2 = std::stoi(tokens.at(2));
			graph.AddEdge(node1, node2);
		}
	}

	graph.ConvertToCompliment();
	return graph;
}


std::vector<std::string> GraphUtilities::Split(const std::string& s, const char delimiter)
{
	std::vector<std::string> tokens;
	std::string token;
	std::stringstream tokenStream(s);
	while (std::getline(tokenStream, token, delimiter))
	{
		tokens.push_back(token);
	}
	return tokens;
}
