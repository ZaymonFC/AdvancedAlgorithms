def ConstructVCGreedy(Graph g, QuickSet vSet):
	const int index <- index between 0 .. |V|
	vSet.Insert(index)

	while(!g.IsVertexCover(vSet)):
		degrees <- g.GetDegreeList(vSet)
		sort degrees
		vSet <-+ highestDegreeNode[degrees]


def MVC_Search(Graph g, int swapLimit, int operationsPerPhase, int iterationCap):
	QuickSet vSet <- QuickSet(g.vertices)
	ConstructVCGreedy(g, vSet)

	list<int> penalty <- list of -1, length = |V|
	
	iterations <- 0
	bestCover <- vSet.Size()

	while iterations < iterationCap:
		for operation < operationsPerPhase:
			const int depth <- random between 0..10

			// Reduce the number of nodes in the list
			for i in range 0...depth:
				u <- rand() % (vSet.Size() - 1)

				while (!vSet.Contains(u)) {
					u <- rand() % (vSet.Size() - 1)
				remove u from vSet
				AddToPenalty(penalty, u)
				UpdatePenalty(penalty)

				if vertex cover break
			if vertex cover break

			// Rebuild - Stopping early if we find a better cover
			surface <- 0
			for i in range 0...depth:
				degree <- g.GetDegreeList(vSet)
				sort degree

				index <- 0
				while penalty[degree[index].node]:
					index++
					if index <- degree.size():
						index <- random in range 0... |V| - |vSet|
						break

				vSet <-+ degree[index].node
				AddToPenalty(penalty, degree.at(index).second)
				UpdatePenalty(penalty)

				if g is vertex cover break
		if g is vertex cover:
			if (vSet.Size() < bestCover) bestCover <- vSet.Size()

		// --- Stabalisation Phase
		if odd iteration:
			const v <- rand() % (vSet.Size() - 1)
			vSet.Remove(v)

		swapCount <- 0

		// Get Back to a vertex Cover with swaps
		while !g.IsVertexCover(vSet) and swapCount < swapLimit:
			degree <- g.GetDegreeList(vSet)
			sort degree

			index <- 0
			while penalty[degree[index].node]:
				index++
				if index <- degree.size():
					index <- rand() % (degree.size() - 1)
					break
			vSet <-+ degree[index].node

			while true:
				const u <- random in range 0...|vSet|
				if penalty.at(u) > 0 continue

			AddToPenalty(penalty, degree.at(index).second)
			UpdatePenalty(penalty)
			swaps--

		if g is vertex cover:
			if vSet.Size() < bestCover:
				bestCover <- vSet.Size()

		if iteration is odd 
			degree <- g.GetDegreeList(vSet)
			sort degree
			vSet <-+ one node from top 5 in degree
		iterations++
	return bestCover

def SearchRunner(Graph g):
	const maximumSwaps <- 100
	const operations <- 50
	const iterations <- 2
	const restarts <- 50

	best <- std::numeric_limits<int>::max()

	for i in range 0...restarts:
		const score <- MVC_Search(g, maximumSwaps, operations, iterations)
		if score < best:
			best <- score
	return best
