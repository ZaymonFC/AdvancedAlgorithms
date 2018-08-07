#include <vector>
#include <limits>
#include <iostream>

struct Edge {
	int src;
	int dest;
	int weight;

	Edge() = default;

	Edge(int src, int dest, int weight) : src{src}, dest{dest}, weight{weight}
	{
	}
};

struct Graph {
	int V;
	int E;
	std::vector<Edge> edges;

	Graph(int V, int E)
	{
		this->V = V;
		this->E = E;
		this->edges = std::vector<Edge>();
	}

	auto AddEdge(int source, int destination, int weight) {
		edges.emplace_back(source, destination, weight);
	}
};

auto PrintArray(std::vector<int> dist, int n) -> void
{
	std::cout << "Vertex Number\tDistance from Source" << std::endl;
	for (auto i = 0; i < n; i++) {
		std::cout << i << '\t' << dist.at(i) << std::endl;
	}
}

auto BellmanFord(Graph graph, int src) -> void
{
	auto V = graph.V;
	auto E = graph.E;
	// Create the distance vector at int max
	auto dist = std::vector<int>(V, std::numeric_limits<int>::max());
	
	// Source cost is always 0
	dist.at(src) = 0;

	// Relax all edges |V| - 1 times
	// A simple shortest path from src to any vertex is at most |V| - 1
	for (auto i = 0; i <= V - 1; ++i)
	{
		for (auto j = 0; j < E; ++j)
		{
			const int u = graph.edges.at(j).src;
			const int v = graph.edges.at(j).dest;
			const int weight = graph.edges.at(j).weight;

			if (dist.at(u) != std::numeric_limits<int>::max() && dist.at(u) + weight < dist.at(v))
			{
				dist.at(v) = dist[u] + weight;
			}
		}
	}
	PrintArray(dist, V);
}

int main()
{
	auto V = 5;
	auto E = 8;
	auto graph = Graph(V, E);

	// add edge 0-1 (or A-B in above figure)
	graph.AddEdge(0, 1, -1);
	// add edge 0-2 (or A-C in above figure)
	graph.AddEdge(0, 2, 4);
	// add edge 1-2 (or B-C in above figure)
	graph.AddEdge(1, 2, 3);
	// add edge 1-3 (or B-D in above figure)
	graph.AddEdge(1, 3, 2);
	// add edge 1-4 (or A-E in above figure)
	graph.AddEdge(1, 4, 2);
	// add edge 3-2 (or D-C in above figure)
	graph.AddEdge(3, 2, 5);
	// add edge 3-1 (or D-B in above figure)
	graph.AddEdge(3, 1, 1);
	// add edge 4-3 (or E-D in above figure)
	graph.AddEdge(4, 3, -3);

	BellmanFord(graph, 0);

    return 0;
}

