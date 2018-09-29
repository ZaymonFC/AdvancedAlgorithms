// MinimumVertexCover.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "GraphUtilities.h"
#include <iostream>


int main()
{
	auto graph = GraphUtilities::LoadGraph("brock800_1.clq");
	std::cout << graph.vertices - 1 << ' ' << graph.edges << std::endl;
    return 0;
}

