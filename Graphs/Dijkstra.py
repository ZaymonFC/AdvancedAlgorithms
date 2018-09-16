def PrintSolution(dist, n):
    print('Vertex\tDistance From Source\n')

    for i in range(len(dist)):
        print(i, dist[i], sep=' ')

def MinimumDistance(dist, sptSet):
    minimum = float('Inf')
    minIndex = -1

    for vertex in range(len(dist)):
        if (not sptSet[vertex]) and dist[vertex] <= minimum:
            minimum = dist[vertex]
            minIndex = vertex
    
    return minIndex

def Djikstra(graph, src):
    dimension = len(graph)
    dist   = [float('Inf') for _ in range(dimension)]
    sptSet = [False for _ in range(dimension)]

    dist[src] = 0

    # Find shortest path for all vertices 0 <= count < |v| - 1
    for count in range(dimension - 1):
        minVertex = MinimumDistance(dist, sptSet)

        # Mark picked vertex as processedD
        sptSet[minVertex] = True

        # Update the value of adjacent vertices from picked vertex
        for vertex in range(dimension):
            if (not sptSet[vertex]) and graph[minVertex][vertex] != 0:
                if dist[minVertex] + graph[minVertex][vertex] < dist[vertex]:
                    dist[vertex] = dist[minVertex] + graph[minVertex][vertex]
    
    PrintSolution(dist, dimension)

graph = [ [ 0, 4, 0, 0, 0, 0, 0, 8, 0 ],
		[ 4, 0, 8, 0, 0, 0, 0, 11, 0 ],
		[ 0, 8, 0, 7, 0, 4, 0, 0, 2 ],
		[ 0, 0, 7, 0, 9, 14, 0, 0, 0 ],
		[ 0, 0, 0, 9, 0, 10, 0, 0, 0 ],
		[ 0, 0, 4, 14, 10, 0, 2, 0, 0 ],
		[ 0, 0, 0, 0, 0, 2, 0, 1, 6 ],
		[ 8, 11, 0, 0, 0, 0, 1, 0, 7 ],
		[ 0, 0, 2, 0, 0, 0, 6, 7, 0 ]]

Djikstra(graph, 0);