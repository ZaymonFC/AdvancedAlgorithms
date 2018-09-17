def ApproximateVertexCover(graph, s):
    cover = []
    visited = [False for x in range(s)]

    for u in range(s):
        if not visited[u]:
            for v in graph[u]:
                if not visited[v]:
                    visited[u] = True
                    visited[v] = True
                    cover.append(u)
                    cover.append(v)
                    break
    return cover

graph = {0: {1, 4},
         1: {0, 2, 5},
         2: {1, 3, 5, 6},
         3: {2},
         4: {0},
         5: {1, 2, 6},
         6: {2, 5}}

print(ApproximateVertexCover(graph, 7))
