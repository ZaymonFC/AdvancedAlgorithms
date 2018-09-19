import queue

def BFS(graph, source, sink, parent):
    node_count = len(graph)

    # Mark all nodes as not visited
    visited = [False] * node_count

    # Create a queue
    q = queue.Queue()
    q.put(source)
    visited[source] = True
    
    while not q.empty():
        u = q.get()

        # Get all adjacent vertices to u, if not visited mark visited and enqueue
        for ind, val in enumerate(graph[u]):
            if visited[ind] is False and val > 0:
                q.put(ind)
                visited[ind] = True
                parent[ind] = u
    return True if visited[sink] else False

def EdmondKarp(graph, source, sink):
    node_count = len(graph)
    
    # Array to be filled by BFS to store path
    parent = [-1] * node_count
    max_flow = 0

    while BFS(graph, source, sink, parent):
        # Find min residual cap of edges along the path filled by BFS
        path_flow = float("Inf")
        s = sink
        while(s is not source):
            path_flow = min(path_flow, graph[parent[s]][s])
            s = parent[s]
        
        max_flow += path_flow

        # Update the residual capacities of the edges and reverse edges along the path
        v = sink
        while (v is not source):
            u = parent[v]
            graph[u][v] -= path_flow
            graph[v][u] += path_flow
            v = parent[v]
    
    return max_flow

# Create the graph
graph = [[0, 16, 13, 0, 0, 0],
        [0, 0, 10, 12, 0, 0],
        [0, 4, 0, 0, 14, 0],
        [0, 0, 9, 0, 0, 20],
        [0, 0, 0, 7, 0, 4],
        [0, 0, 0, 0, 0, 0]]

source, sink = 0, 5
print('The maximum possible flow is: ', EdmondKarp(graph, source, sink))