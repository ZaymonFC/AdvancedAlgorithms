// Djikstra's Algorithm
#include <limits>
#include <cstdio>

// Number of vertices in graph
constexpr int vertex_count = 9;
constexpr int max_const_expr = 9999999;

// A utility function to print the constructed distance array
auto PrintSolution(int dist[], int n) -> void
{
	printf("Vertex\tDistance from Source\n");
	for (auto i = 0; i < vertex_count; i++)
	{
		printf("%d\t%d\n", i, dist[i]);	
	}
}

auto MinimumDistance(int dist[9], bool sptSet[9]) -> int 
{
	auto min = max_const_expr;
	auto minIndex = -1;

	for (auto vertex = 0; vertex < vertex_count; ++vertex)
	{
		if (!sptSet[vertex] && dist[vertex] <= min)
		{
			min = dist[vertex];
			minIndex = vertex;
		}
	}

	return minIndex;
}

auto Djikstra(const int graph[vertex_count][vertex_count], int src)
{
	int dist[vertex_count]; // Output array (dist[i] shortest distance from src to i)
	bool sptSet[vertex_count]; // spcSet[i] == true if V i is included in shortest path tree

	// Initialise all distances as infinite and stpSet[] as false
	for (auto &element : dist)
	{
		element = max_const_expr;
	}
	for (auto &element: sptSet)
	{
		element = false;
	}

	dist[src] = 0;

	// Find shortest path for all vertices
	for (auto count = 0; count < vertex_count-1; ++count)
	{
		// Pick the minimum distance vertex from the set of vertices not yet processed, u == src in iteration 1
		const auto minVertex = MinimumDistance(dist, sptSet);

		if (minVertex == -1) continue;

		// Mark the picked vertex as processed
		sptSet[minVertex] = true;

		// Update the value of the adjacent vertices from the picked vertex
		for(auto vertex = 0; vertex < vertex_count; ++vertex)
		{
			// Check that the vertex is not in the shortest path tree
			// Check if there is an edge from u to vertex
			if (!sptSet[vertex] && graph[minVertex][vertex] != 0)
			{
				if (dist[minVertex] + graph[minVertex][vertex] < dist[vertex])
				{
					dist[vertex] = dist[minVertex] + graph[minVertex][vertex];
				}
			}
		}
	}

	PrintSolution(dist, vertex_count);
}


int main()
{
	const int graph[vertex_count][vertex_count] = { { 0, 4, 0, 0, 0, 0, 0, 8, 0 },
		{ 4, 0, 8, 0, 0, 0, 0, 11, 0 },
		{ 0, 8, 0, 7, 0, 4, 0, 0, 2 },
		{ 0, 0, 7, 0, 9, 14, 0, 0, 0 },
		{ 0, 0, 0, 9, 0, 10, 0, 0, 0 },
		{ 0, 0, 4, 14, 10, 0, 2, 0, 0 },
		{ 0, 0, 0, 0, 0, 2, 0, 1, 6 },
		{ 8, 11, 0, 0, 0, 0, 1, 0, 7 },
		{ 0, 0, 2, 0, 0, 0, 6, 7, 0 }};

	Djikstra(graph, 0);

    return 0;
}

